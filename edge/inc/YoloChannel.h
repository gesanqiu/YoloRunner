/*
 * @Description: 
 * @version: 
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:14
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 17:24:31
 */
#pragma once

#include "Common.h"
#include "VideoPipeline.h"
#include "VideoAnalyzer.h"

struct YoloChannelConfig {
    std::string         m_chanelId;
    VideoPipelineConfig m_vpConfig;
    VideoAnalyzerConfig m_vaConfig;
};

class YoloChannel {
public:
    YoloChannel(YoloChannelConfig& config);
    ~YoloChannel();
    int Init();
    int DeInit();
    bool Start();
    bool Stop();
    
private:
    YoloChannelConfig m_config;
    std::unique_ptr<VideoPipeline> m_vp;
    std::unique_ptr<VideoAnalyzer> m_va;
    std::unique_ptr<SafeQueue<cv::Mat>> m_imageQueue;
    std::shareunique_ptrd_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> m_resultsCache;
};