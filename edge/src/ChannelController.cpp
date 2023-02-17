/*
 * @Description: Implementation of ChannelController.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:44:07
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-17 21:53:13
 */

#include "ChannelController.h"
#include "ConfigParser.h"

namespace edge {

static std::shared_ptr<std::map<std::string, VideoPipelineCreator>> pipeline_creators;
static std::shared_ptr<std::map<std::string, VideoTaskCreator>> task_creators;

class ChannelController::Impl {
public:
    Impl() {
        m_channelList = std::make_shared<std::map<std::string, std::shared_ptr<YoloChannel>>>();
    }
    ~Impl() { DeInit(); }

    bool Init() {
        return true;
    }

    bool DeInit() {
        for (auto& [k, v] : *m_channelList) {
            LOG_INFO("Deinit pipeline: {}", k);
            v.reset();
        }
        m_channelList->clear();
        return true;
    }

    bool Start() {
        for (const auto& [k, v] : *m_channelList) {
            if (!v->Start()) {
                LOG_WARN("Start Channel[{}] failed.", k);
                continue;
            }
            LOG_INFO("Start Channel[{}]  succeed.", k);
        }
        return true;
    }

    bool StartChannel(std::string& channel_id) {
        if (const auto& it = m_channelList->find(channel_id); it == m_channelList->end()) {
            LOG_ERROR("There is no channel named: {} in application, start failed.", channel_id);
            return false;
        } else if (!it->second->Start()) {
            LOG_WARN("Start Channel[{}] failed.", channel_id);
            return false;
        }
        return true;
    }

    bool Stop() {
        for (const auto& [k, v] : *m_channelList) {
            if (!v->Stop()) {
                LOG_WARN("Stop Channel[{}] failed.", k);
                continue;
            }
            LOG_INFO("Stop Channel[{}]  succeed.", k);
        }
        return true;
    }

    bool StopChannel(std::string& channel_id) {
        if (const auto& it = m_channelList->find(channel_id); it == m_channelList->end()) {
            LOG_ERROR("There is no channel named: {} in application, stop failed.", channel_id);
            return false;
        } else if (!it->second->Stop()) {
            LOG_WARN("Stop Channel[{}] failed.", channel_id);
            return false;
        }
        return true;
    }

    bool AddChannel(std::string& config_string) {
        YoloChannelConfig config;
        std::shared_ptr<YoloChannel> channel = nullptr;
        if (CONFIG_PARSE_SUCCESS != ConfigParse(config, config_string)) {
            LOG_ERROR("Parse config file error.");
            return false;
        }

        if (pipeline_creators->find(config.m_vpConfig.pipeline_id) == pipeline_creators->end()) {
            LOG_ERROR("Can't find pipeline type: {}", config.m_vpConfig.pipeline_id);
            return false;
        } else {
            LOG_INFO("Found pipeline type: {}", config.m_vpConfig.pipeline_id);
        }

        if (task_creators->find(config.m_vtConfig.analyzer_id) == task_creators->end()) {
            LOG_ERROR("Can't find task type: {}", config.m_vtConfig.analyzer_id);
            return false;
        } else {
            LOG_INFO("Found task type: {}", config.m_vtConfig.analyzer_id);
        }
        std::shared_ptr<VideoPipeline> pipeline = (*pipeline_creators)[config.m_vpConfig.pipeline_id]();
        std::shared_ptr<VideoTask> task = (*task_creators)[config.m_vtConfig.analyzer_id]();

        channel.reset(new YoloChannel(config));
        if (!channel->Init(pipeline, task)) {
            LOG_ERROR("Channel[{}] init failed.", config.m_chanelId);
            return false;
        }
        (*m_channelList)[config.m_chanelId] = channel;
        return true;
    }

    bool AddChannel(std::ifstream& config_stream) {
        YoloChannelConfig config;
        std::shared_ptr<YoloChannel> channel = nullptr;
        if (CONFIG_PARSE_SUCCESS != ConfigParse(config, config_stream)) {
            LOG_ERROR("Parse config file error.");
            return false;
        }

        if (pipeline_creators->find(config.m_vpConfig.pipeline_id) == pipeline_creators->end()) {
            LOG_ERROR("Can't find pipeline type: {}", config.m_vpConfig.pipeline_id);
            return false;
        } else {
            LOG_INFO("Found pipeline type: {}", config.m_vpConfig.pipeline_id);
        }

        if (task_creators->find(config.m_vtConfig.analyzer_id) == task_creators->end()) {
            LOG_ERROR("Can't find task type: {}", config.m_vtConfig.analyzer_id);
            return false;
        } else {
            LOG_INFO("Found task type: {}", config.m_vtConfig.analyzer_id);
        }
        std::shared_ptr<VideoPipeline> pipeline = (*pipeline_creators)[config.m_vpConfig.pipeline_id]();
        std::shared_ptr<VideoTask> task = (*task_creators)[config.m_vtConfig.analyzer_id]();

        channel.reset(new YoloChannel(config));
        if (!channel->Init(pipeline, task)) {
            LOG_ERROR("Channel[{}] init failed.", config.m_chanelId);
            return false;
        }

        (*m_channelList)[config.m_chanelId] = channel;
        return true;
    }

    bool DeletaChannel(std::string& channel_id) {
        if (const auto& it = m_channelList->find(channel_id); it == m_channelList->end()) {
            LOG_ERROR("There is no channel named: {} in application, delete failed.", channel_id);
            return false;
        }
        (*m_channelList)[channel_id]->DeInit();
        (*m_channelList)[channel_id].reset();
        m_channelList->erase(channel_id);
        return true;
    }

    static bool RegisterVideoPipelineCreator(std::string pipeline_class, VideoPipelineCreator creator) {
        if (nullptr == pipeline_creators) {
            pipeline_creators = std::make_shared<std::map<std::string, VideoPipelineCreator>>();
        }

        if (pipeline_creators->find(pipeline_class) != pipeline_creators->end()) {
            LOG_WARN("Creator already registed, exit.");
            return true;
        }
        pipeline_creators->operator[](pipeline_class) = creator;
        return true;
    }

    static bool RegisterVideoTaskCreator(std::string task_class, VideoTaskCreator creator) {
        if (nullptr == task_creators) {
            task_creators = std::make_shared<std::map<std::string, VideoTaskCreator>>();
        }

        if (task_creators->find(task_class) != task_creators->end()) {
            LOG_WARN("Creator already registed, exit.");
            return true;
        }
        task_creators->operator[](task_class) = creator;
        return true;
    }

private:
    std::shared_ptr<std::map<std::string, std::shared_ptr<YoloChannel>>> m_channelList;
};

ChannelController::ChannelController() : impl(new Impl)
{

}

ChannelController::~ChannelController()
{

}

bool ChannelController::Init()
{
    return impl->Init();
}

bool ChannelController::DeInit()
{
    return impl->DeInit();
}

bool ChannelController::Start()
{
    return impl->Start();
}

bool ChannelController::StartChannel(std::string& channel_id)
{
    return impl->StartChannel(channel_id);
}

bool ChannelController::Stop()
{
    return impl->Stop();
}

bool ChannelController::StopChannel(std::string& channel_id)
{
    return impl->StopChannel(channel_id);
}

bool ChannelController::AddChannel(std::ifstream& config_stream)
{
    return impl->AddChannel(config_stream);
}

bool ChannelController::AddChannel(std::string& config_string)
{
    return impl->AddChannel(config_string);
}

bool ChannelController::DeletaChannel(std::string& channel_id)
{
    return impl->DeletaChannel(channel_id);
}

bool ChannelController::RegisterVideoPipelineCreator(std::string pipeline_class, VideoPipelineCreator creator)
{
    return Impl::RegisterVideoPipelineCreator(pipeline_class, creator);
}

bool ChannelController::RegisterVideoTaskCreator(std::string task_class, VideoTaskCreator creator)
{
    return Impl::RegisterVideoTaskCreator(task_class, creator);
}

}