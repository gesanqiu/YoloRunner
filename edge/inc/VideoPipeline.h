/*
 * @Description: Implement of VideoPipeline on DeepStream.
 * @version: 2.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2022-07-15 22:07:29
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 18:58:55
 */
#pragma once

#include "Common.h"

static int pipeline_id = 0;

typedef enum _VideoType {
    FILE_STREAM = 0,
    RTSP_STREAM = 1,
    USB_CAMERE = 2
}VideoType;

typedef struct _VideoPipelineConfig {
    std::string               pipeline_id;
    int                       input_type { VideoType::FILE_STREAM };
    int                       src_width;
    int                       src_height;
    /*------------------uridecodebin------------------*/
    std::string               src_uri;
    bool                      file_loop;
    int                       rtsp_latency;
    int                       rtp_protocol;
    /*--------------------v4l2src--------------------*/
    std::string               src_device;
    std::string               src_format;
    int                       src_framerate_n;
    int                       src_framerate_d;
    /*------------------nvstreammux-----------------*/
    bool                      src_sync;
    bool                      src_live_source;
    int                       src_memory_type;
    /*-------------nveglglessink branch-------------*/
    bool                      enable_hdmi;
    bool                      hdmi_sync;
    int                       window_x;
    int                       window_y;
    int                       window_width;
    int                       window_height;
    /*----------------rtmpsink branch---------------*/
    // nvvideoconvert of this branch only convert color space to NV12(default behavior) //
    bool                      enable_rtmp;
    int                       enc_bitrate;
    int                       enc_iframe_interval;
    std::string               rtmp_uri;
    /*---------------inference branch---------------*/
    bool                      enable_appsink;
    /*----------------nvvideoconvert----------------*/
    int                       cvt_memory_type;
    std::string               cvt_format;
    int                       cvt_width;
    int                       cvt_height;
    std::string               crop;
    /*----------------------osd----------------------*/
    uint32_t                  bbox_thickness;
    uint32_t                  text_thickness;
    std::tuple<int, int, int> bbox_rgb;
    std::tuple<int, int, int> text_rgb;
}VideoPipelineConfig;

class VideoPipeline {
public:
    VideoPipeline      (const VideoPipelineConfig& config);
    ~VideoPipeline     ();
    bool Create        ();
    bool Start         ();
    bool Stop          ();
    bool Resume        ();
    void Destroy       ();
    void SetCallbacks  (PutFrameFunc func, void* args);
    void SetCallbacks  (GetResultFunc func, void* args);
    void SetCallbacks  (ProcResultFunc func);
    void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data);
    void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data);

private:
    GstElement* CreateUridecodebin();
    GstElement* CreateV4l2src();

public:
    PutFrameFunc        m_putFrameFunc;
    void*               m_putFrameArgs;
    GetResultFunc       m_getResultFunc;
    void*               m_getResultArgs;
    ProcResultFunc      m_procResultFunc;
    std::shared_ptr<SafeQueue<cv::Mat>> m_productQueue;
    std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> m_resultCache;

    uint64_t            m_queue00_src_probe;     /* probe for nvvideoconvert sync ans osd process */
    uint64_t            m_cvt_sink_probe;        /* probe for inference rate control */
    uint64_t            m_cvt_src_probe;         /* probe for convert lock sync */
    uint64_t            m_dec_sink_probe;        /* probe for seek */

    uint64_t            m_prev_accumulated_base;    /* PTS offset for seek */
    uint64_t            m_accumulated_base;         /* PTS offset for seek */

    VideoPipelineConfig m_config;

    volatile int        m_syncCount;
    volatile bool       m_isExited;
    bool                m_isFileLoop;
    GMutex              m_syncMuxtex;
    GCond               m_syncCondition;
    GMutex              m_mutex;
    bool                m_dumped;           /* dump pipeline to dot */

    GstElement*         m_pipeline;
    GstElement*         m_source;           /* uridecodebin or v4l2src */
    GstElement*         m_streammuxer;      /* nvstreamuxer */
    GstElement*         m_capfilter0;       /* image/jpeg */
    GstElement*         m_decoder;          /* nvv4l2decoder or nvjpegdec */
    GstElement*         m_tee0;             /* display branch & inference branch */
    GstElement*         m_queue00;          /* for display branch */ 
    GstElement*         m_fakesink;         /* sync stream when disabled display */
    GstElement*         m_tee1;             /* nveglglessink branch & rtmpsink branch */
    GstElement*         m_queue10;          /* for nveglglessink branch */
    GstElement*         m_nveglglessink;    /* nveglglessink */
    GstElement*         m_queue11;          /* for rtmpsink branch */
    GstElement*         m_nvvideoconvert0;  /* convert RGBA(nvjpegdec) to NV12 */
    GstElement*         m_capfilter1;
    GstElement*         m_encoder;          /* nvv4l2h264enc */
    GstElement*         m_h264parse;        /* h264parse */
    GstElement*         m_flvmux;           /* flvmux */
    GstElement*         m_rtmpsink;         /* rtmpsink */
    GstElement*         m_queue01;          /* for inference branch */
    GstElement*         m_nvvideoconvert1;  /* convert NV12(nvv4l2decoder) to RGBA */
    GstElement*         m_capfilter2;
    GstElement*         m_appsink;          /* for AI inference */
    GstElement*         m_nvvideoconvert2;  /* convert NV12(nvv4l2decoder) to RGBA */
    GstElement*         m_capfilter3;
};


/*

gst-launch-1.0 uridecodebin uri="rtsp://127.0.0.1:554/live/test" ! tee name=tee0 ! queue ! \
tee name=t1 ! queue ! nveglglessink tee1. ! queue ! nvvideoconvert ! video/x-raw(memory:NVMM),format=NV12 ! \
nvv4l2h264enc bitrate=4000000 iframeinterval=30 ! flvmux ! rtmpsink location=rtmp://127.0.0.1:1935/live/test \
tee0. ! queue ! nvvideoconvert ! video/x-raw(memory:NVMM),format=RGBA ! appsink

gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg,format=MJPG,width=1920,height=1080,framerate=30/1 ! \ 
nvjpegdec ! tee name=tee0 ! queue ! tee name=tee1 ! queue ! nveglglessink tee1. ! queue ! nvvideoconvert ! \
video/x-raw(memory:NVMM),format=NV12 ! nvv4l2h264enc bitrate=4000000 iframeinterval=30 ! flvmux ! \
rtmpsink location=rtmp://127.0.0.1:1935/live/test tee0. ! queue ! nvvideoconvert ! video/x-raw(memory:NVMM),format=RGBA ! appsink

*/