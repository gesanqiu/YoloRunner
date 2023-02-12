/*
 * @Description: Inference decoded stream with libYOLOv5s.so.
 * @version: 2.2
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2022-10-11 11:50:40
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 17:42:34
 */

#include "VideoAnalyzer.h"

void VideoAnalyzer::InferenceFrame()
{
    std::shared_ptr<cv::Mat> image;

    while (isRunning) {
        std::vector<yolov5::Detection> results;
        consumeQueue->consumption(image);

        cv::Mat input;
        cv::cvtColor(*image.get(), input, cv::COLOR_RGBA2RGB);
        detector->detect(input, &results, yolov5::INPUT_RGB);
        resultCache->feed(std::make_shared<std::vector<yolov5::Detection>>(results));
        // LOG_INFO("queue size: {}, detected size: {}", consumeQueue->getCurrentSize(), results.size());
    }
}

VideoAnalyzer::VideoAnalyzer()
{
    isRunning = false;
}

VideoAnalyzer::~VideoAnalyzer()
{
    DeInit();
}

bool VideoAnalyzer::Init(Json::Value& model)
{

    std::string modelName = model["model-name"].asString();
    std::string engineFile = model["engine-file"].asString();
    LOG_INFO("Load model: {}", engineFile);
    std::string classesFile = model["classes-file"].asString();
    LOG_INFO("Load classes: {}", classesFile);

    detector = std::shared_ptr<yolov5::Detector>(new yolov5::Detector());
    yolov5::Result r = detector->init();
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Detector Init() failed.");
        return false;
    }
    r = detector->loadEngine(engineFile);
    if(r != yolov5::RESULT_SUCCESS) {
        LOG_ERROR("yolov5::Detector load engine: {} failed.", engineFile);
        return false;
    }

    yolov5::Classes classes;
    classes.setLogger(detector->logger());
    r = classes.loadFromFile(classesFile);
    detector->setClasses(classes);

    return true;
}

bool VideoAnalyzer::DeInit()
{
    if (inferThread) {
        isRunning = false;
        inferThread->join();
        inferThread = nullptr;
    }

    detector = nullptr;
    return true;
}

bool VideoAnalyzer::Start()
{
    isRunning = true;
    if (!(inferThread = std::make_shared<std::thread>(std::bind(&VideoAnalyzer::InferenceFrame, this)))) {
        LOG_ERROR("Failed to new a std::thread object");
        isRunning = false;
        return false;
    }

    return true;
}

void VideoAnalyzer::SetUserData(std::shared_ptr<SafeQueue<cv::Mat>> user_data)
{
    consumeQueue = user_data;
}

void VideoAnalyzer::SetUserData(std::shared_ptr<DoubleBufCache<std::vector<yolov5::Detection>>> user_data)
{
    resultCache = user_data;
}