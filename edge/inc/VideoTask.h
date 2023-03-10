/*
 * @Description: Inference decoded stream with yolov5-tensorrt.
 * @version: 2.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-07 20:31:48
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-16 21:40:45
 */
#pragma once

#include "Common.h"

#define VIDEO_TASK_CREATOR(CLASSNAME) \
    static std::shared_ptr<VideoTask> Create##CLASSNAME() { \
        std::shared_ptr<CLASSNAME> tmp; \
        tmp.reset(new CLASSNAME()); \
        return tmp; \
    } \
    static bool Bool##CLASSNAME = edge::ChannelController::RegisterVideoTaskCreator(#CLASSNAME, std::bind(Create##CLASSNAME));

namespace edge {

class VideoTask;
typedef std::function<std::shared_ptr<VideoTask>()> VideoTaskCreator;

struct VideoTaskConfig {
    std::string analyzer_id;
    std::string engine_file;
    std::string classes_file;
    double      score_threshold;
    double      nms_threshold;
};

class VideoTask {
public:
    virtual ~VideoTask() {};
    virtual bool Init(const VideoTaskConfig& config) = 0;
    virtual bool DeInit() = 0;
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual void SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data) = 0;
    virtual void SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data) = 0;
};

}   // namespace edge