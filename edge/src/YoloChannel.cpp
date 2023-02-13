/*
 * @Description: 
 * @version: 
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:22
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 19:10:23
 */

#include "YoloChannel.h"

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
    m_va = std::make_unique<VideoAnalyzer>(m_config.m_vaConfig);

    m_vp->Create();
    m_va->Init();

    m_imageQueue = std::make_shared<SafeQueue<cv::Mat>>();
    m_resultsCache = std::make_shared<DoubleBufCache<std::vector<yolov5::Detection>>>();

    m_vp->SetUserData(m_imageQueue);
    m_vp->SetUserData(m_resultsCache);
    m_va->SetUserData(m_imageQueue);
    m_va->SetUserData(m_resultsCache);

    return true;
}

void YoloChannel::DeInit()
{
    if (m_vp) m_vp.reset(nullptr);
    if (m_va) m_va.reset(nullptr);
}

bool YoloChannel::Start()
{
    m_vp->Start();
    m_va->Start();

    return true;
}

bool YoloChannel::Stop()
{
    m_vp->Stop();
    m_va->Stop();

    return true;
}