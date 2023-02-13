/*
 * @Description: Inference decoded stream with yolov5-tensorrt.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-07 20:31:48
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 18:56:36
 */
#pragma once

#include "Common.h"

struct VideoAnalyzerConfig {
    std::string analyzer_id;
    std::string engine_file;
    std::string classes_file;
    double      score_threshold;
    double      nms_threshold;
};

class VideoAnalyzer {
public:
    VideoAnalyzer(VideoAnalyzerConfig& config);
    ~VideoAnalyzer();
    bool Init();
    bool DeInit();
    bool Start();
    bool Stop();
    void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data);
    void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data);

private:
    VideoAnalyzerConfig m_config;

    bool isRunning;
    void InferenceFrame();
    std::shared_ptr<std::thread> inferThread;

    std::unique_ptr<yolov5::Detector> detector;
    std::unique_ptr<yolov5::Classes> classes;
    std::shared_ptr<SafeQueue<cv::Mat>> consumeQueue;
    std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> resultCache;
};