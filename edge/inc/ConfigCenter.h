/*
 * @Description: MediaAI config parser.
 * @version: 2.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:29
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-18 18:39:41
 */

#pragma once

#include "Common.h"
#include "YoloChannel.h"

#define CONFIG_PARSE_SUCCESS    0
#define CONFIG_LACK_MEMBER      1
#define CONFIG_VALUE_TYPE_ERROR 2
#define CONFIG_ALREADY_EXIST    3

namespace edge {

class ConfigCenter {
public:
    static ConfigCenter& getInstance() {
        static ConfigCenter instance;
        return instance;
    }

    int ConfigParse(YoloChannelConfig& config, std::ifstream& configStream);
    int ConfigParse(YoloChannelConfig& config, std::string& configString);

    // YoloChannelConfig GetChannelConfig(std::string& id);
    // VideoPipelineConfig GetVideoPipelineConfig(std::string& id);
    // VideoTaskConfig GetVideoTaskConfig(std::string& id);

    std::string GetChannelList();
    std::string GetChannelConfigString(const std::string& id);
    std::string GetVideoPipelineConfigString(const std::string& id);
    std::string GetVideoTaskConfigString(const std::string& id);
    void DeleteChannelConfig(const std::string& id);

private:
    ConfigCenter() { };
    ~ConfigCenter() { };
    ConfigCenter(const ConfigCenter&);
	ConfigCenter& operator=(const ConfigCenter&);

    int ConfigVideoTaskConfig(VideoTaskConfig& config, Json::Value& value);
    int ConfigVideoPipelineConfig(VideoPipelineConfig& config, Json::Value& value);

    Json::Reader m_reader;
    std::unordered_map<std::string, Json::Value> m_channelConfigNodes;
    std::unordered_map<std::string, Json::Value> m_pipelineConfigNodes;
    std::unordered_map<std::string, Json::Value> m_taskConfigNodes;
    // std::unordered_map<std::string, YoloChannelConfig> m_channelConfigs;
};

}