/*
 * @Description: Implementation of YoloChannel.
 * @version: 1.1
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:22
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-16 21:48:42
 */

#include "YoloChannel.h"

namespace edge {

YoloChannel::YoloChannel(YoloChannelConfig& config)
{
    m_config = config;
}

YoloChannel::~YoloChannel()
{
    DeInit();
}

bool YoloChannel::Init(std::shared_ptr<VideoPipeline> pipeline, std::shared_ptr<VideoTask> task)
{
    m_vp = pipeline;
    m_vt = task;

    m_vp->Init(m_config.m_vpConfig);
    m_vt->Init(m_config.m_vtConfig);

    m_imageQueue = std::make_shared<SafeQueue<cv::Mat>>();
    m_resultsCache = std::make_shared<DoubleBufCache<std::vector<yolov5::Detection>>>();

    m_vp->SetUserData(m_imageQueue);
    m_vp->SetUserData(m_resultsCache);
    m_vt->SetUserData(m_imageQueue);
    m_vt->SetUserData(m_resultsCache);

    return true;
}

void YoloChannel::DeInit()
{
    if (m_vp) m_vp.reset();
    if (m_vt) m_vt.reset();
}

bool YoloChannel::Start()
{
    m_vp->Start();
    m_vt->Start();

    return true;
}

bool YoloChannel::Stop()
{
    m_vp->Stop();
    m_vt->Stop();

    return true;
}

}   // namespace edge