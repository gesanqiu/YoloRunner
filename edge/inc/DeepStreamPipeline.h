/*
 * @Description: Header of DeepStreamPipeline.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-15 18:53:41
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-16 21:17:57
 */

#pragma once

#include "VideoPipeline.h"
#include "ChannelController.h"

namespace edge {

class DeepStreamPipeline : public VideoPipeline {
public:
    DeepStreamPipeline();
    ~DeepStreamPipeline();

    virtual bool Create        ();
    virtual bool Start         ();
    virtual bool Stop          ();
    virtual bool Resume        ();
    virtual void Destroy       ();
    virtual bool Init(const VideoPipelineConfig& config);
    virtual void SetCallbacks  (PutFrameFunc func, void* args);
    virtual void SetCallbacks  (GetResultFunc func, void* args);
    virtual void SetCallbacks  (ProcResultFunc func);
    virtual void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data);
    virtual void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data);

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

}