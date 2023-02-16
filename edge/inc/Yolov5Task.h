/*
 * @Description: Header of Yolov5Task.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-15 18:54:21
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-16 22:12:41
 */

#pragma once

#include "VideoTask.h"
#include "ChannelController.h"

namespace edge {

class Yolov5Task : public VideoTask {
public:
    Yolov5Task();
    ~Yolov5Task();

    virtual bool Init(const VideoTaskConfig& config);
    virtual bool DeInit();
    virtual bool Start();
    virtual bool Stop();
    virtual void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data);
    virtual void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data);

private:
    VideoTaskConfig m_config;

    bool isRunning;
    void InferenceFrame();
    std::shared_ptr<std::thread> inferThread;

    std::shared_ptr<yolov5::Detector> detector;
    std::shared_ptr<yolov5::Classes> classes;
    std::shared_ptr<SafeQueue<cv::Mat>> consumeQueue;
    std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> resultCache;

};

}