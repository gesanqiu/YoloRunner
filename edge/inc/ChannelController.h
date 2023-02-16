/*
 * @Description: Header of ChannelController.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:44:02
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-16 21:47:47
 */

#pragma once

#include "ConfigParser.h"
#include "YoloChannel.h"
#include "Common.h"

namespace edge {

class ChannelController {
public:
    ChannelController();
    ~ChannelController();

    bool Init();
    bool DeInit();
    bool Start();
    bool StartChannel(std::string& channel_id);
    bool Stop();
    bool StopChannel(std::string& channel_id);
    bool AddChannel(std::ifstream& config_stream);
    bool AddChannel(std::string& config_string);
    bool DeletaChannel(std::string& channel_id);

    static bool RegisterVideoPipelineCreator(std::string pipeline_class, VideoPipelineCreator creator);
    static bool RegisterVideoTaskCreator(std::string task_class, VideoTaskCreator creator);

private:
    class Impl;
    std::shared_ptr<Impl> impl;
};

}