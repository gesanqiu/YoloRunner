/*
 * @Description: Header of YoloChannel.
 * @version: 1.1
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:14
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-15 20:59:40
 */
#pragma once

#include "Common.h"
#include "VideoPipeline.h"
#include "VideoTask.h"

namespace edge {

struct YoloChannelConfig {
    std::string         m_chanelId;
    VideoPipelineConfig m_vpConfig;
    VideoTaskConfig m_vtConfig;
};

class YoloChannel {
public:
    YoloChannel(YoloChannelConfig& config);
    ~YoloChannel();
    bool Init();
    void DeInit();
    bool Start();
    bool Stop();
    
private:
    YoloChannelConfig m_config;
    std::unique_ptr<VideoPipeline> m_vp;
    std::unique_ptr<VideoTask> m_vt;
    std::shared_ptr<SafeQueue<cv::Mat>> m_imageQueue;
    std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> m_resultsCache;
};

}   // namespace edge