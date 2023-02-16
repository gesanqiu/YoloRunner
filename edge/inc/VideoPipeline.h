/*
 * @Description: Implement of VideoPipeline on DeepStream.
 * @version: 2.1
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2022-07-15 22:07:29
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-16 21:40:36
 */
#pragma once

#include "Common.h"

#define VIDEO_PIPELINE_CREATOR(CLASSNAME) \
    static std::shared_ptr<VideoPipeline> Create##CLASSNAME() { \
        std::shared_ptr<CLASSNAME> tmp; \
        tmp.reset(new CLASSNAME()); \
        return tmp; \
    } \
    static bool Bool##CLASSNAME = edge::ChannelController::RegisterVideoPipelineCreator(#CLASSNAME, std::bind(Create##CLASSNAME));

namespace edge {

class VideoPipeline;
typedef std::function<std::shared_ptr<VideoPipeline>()> VideoPipelineCreator;

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
    virtual ~VideoPipeline() {};

    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual bool Resume() = 0;
    virtual void Destroy() = 0;
    virtual bool Init(const VideoPipelineConfig& config) = 0;
    virtual void SetCallbacks  (PutFrameFunc func, void* args) = 0;
    virtual void SetCallbacks  (GetResultFunc func, void* args) = 0;
    virtual void SetCallbacks  (ProcResultFunc func) = 0;
    virtual void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data) = 0;
    virtual void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data) = 0;
};

}   // namespace edge


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