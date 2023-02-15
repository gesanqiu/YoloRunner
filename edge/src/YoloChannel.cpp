/*
 * @Description: Implementation of YoloChannel.
 * @version: 1.1
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:22
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-15 21:00:13
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

bool YoloChannel::Init()
{
    m_vp = std::make_unique<VideoPipeline>(m_config.m_vpConfig);
    m_vt = std::make_unique<VideoTask>(m_config.m_vtConfig);

    m_vp->Create();
    m_vt->Init();

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
    if (m_vp) m_vp.reset(nullptr);
    if (m_vt) m_vt.reset(nullptr);
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