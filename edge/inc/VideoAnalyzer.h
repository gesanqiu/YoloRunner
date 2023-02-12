/*
 * @Description: Inference decoded stream with yolov5-tensorrt.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-07 20:31:48
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 17:42:23
 */
#pragma once

#include "Common.h"

struct VideoAnalyzerConfig {

};

class VideoAnalyzer {
public:
    VideoAnalyzer();
    ~VideoAnalyzer();
    bool Init(Json::Value& model);
    bool DeInit();
    bool Start();
    void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data);
    void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data);

private:
    VideoAnalyzerConfig m_config;

    bool isRunning;
    void InferenceFrame();
    std::shared_ptr<std::thread> inferThread;

    std::shared_ptr<yolov5::Detector> detector;
    std::shared_ptr<SafeQueue<cv::Mat>> consumeQueue;
    std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> resultCache;
};