/*
 * @Description: 
 * @version: 
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-15 18:54:31
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-17 21:55:37
 */

#include "Yolov5Task.h"

namespace edge {

VIDEO_TASK_CREATOR(Yolov5Task);

void Yolov5Task::InferenceFrame()
{
    std::shared_ptr<cv::Mat> image;

    while (isRunning) {
        std::vector<yolov5::Detection> results;
        consumeQueue->consumption(image);

        if (image) {
            cv::Mat input;
            cv::cvtColor(*image.get(), input, cv::COLOR_RGBA2RGB);
            detector->detect(input, &results, yolov5::INPUT_RGB);
            resultCache->feed(std::make_shared<std::vector<yolov5::Detection>>(results));
            // LOG_INFO("queue size: {}, detected size: {}", consumeQueue->getCurrentSize(), results.size());
        }
    }
    LOG_INFO("Inference thread exit.");
}

Yolov5Task::Yolov5Task()
{

}

Yolov5Task::~Yolov5Task()
{
    DeInit();
}

bool Yolov5Task::Init(const VideoTaskConfig& config)
{
    m_config = config;
    isRunning = false;

    detector = std::make_shared<yolov5::Detector>();
    classes = std::make_shared<yolov5::Classes>();

    yolov5::Result r = detector->init();
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Detector Init() failed.");
        return false;
    }

    r = detector->loadEngine(m_config.engine_file);
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Detector load engine: {} failed.", m_config.engine_file);
        return false;
    }

    r = detector->setScoreThreshold(m_config.score_threshold);
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Detector set score threshold failed.");
        return false;
    }

    r = detector->setNmsThreshold(m_config.nms_threshold);
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Detector set nms threshold failed.");
        return false;
    }

    classes->setLogger(detector->logger());
    r = classes->loadFromFile(m_config.classes_file);
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Classes load classes: {} failed.", m_config.classes_file);
        return false;
    }

    r = detector->setClasses(*classes.get());
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Classes set classes failed.");
        return false;
    }

    return true;
}

bool Yolov5Task::DeInit()
{
    if (inferThread) {
        isRunning = false;
        consumeQueue->exit();
        inferThread->join();
        inferThread.reset();
    }

    if (detector) detector.reset();
    if (classes) classes.reset();
    return true;
}

bool Yolov5Task::Start()
{
    isRunning = true;
    if (!(inferThread = std::make_shared<std::thread>(std::bind(&Yolov5Task::InferenceFrame, this)))) {
        LOG_ERROR("Failed to new a std::thread object");
        isRunning = false;
        return false;
    }

    return true;
}

bool Yolov5Task::Stop()
{
    if (inferThread) {
        isRunning = false;
        inferThread->join();
        inferThread.reset();
    }
    return true;
}

void Yolov5Task::SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data)
{
    consumeQueue = user_data;
}

void Yolov5Task::SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data)
{
    resultCache = user_data;
}

}   // namespace edge