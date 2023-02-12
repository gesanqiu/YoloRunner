/*
 * @Description: YoloRunner config parser.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:29
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 17:40:12
 */

#pragma once

#include "Common.h"
#include "YoloChannel.h"

#define CONFIG_PARSE_SUCCESS    0
#define CONFIG_LACK_PARAMETER   1
#define CONFIG_VALUE_ERROR      2

static Json::Reader g_reader;

static bool configVideoAnalyzerConfig(VideoAnalyzerConfig& vaConfig, Json::Value& value)
{

}

static bool configVideoPipelineConfig(VideoPipelineConfig& vpConfig, Json::Value& value)
{
    if (root.isMember("input-config")) {
        Json::Value inputConfig = root["input-config"];
        config.input_type = inputConfig["type"].asInt();    // 0-MP4 / 1-RTSP / 2-USB Camera
        LOG_INFO("Pipeline[{}]: type: {}", config.pipeline_id, config.input_type);

        config.src_uri = inputConfig["stream"]["uri"].asString();
        LOG_INFO("Pipeline[{}]: input: {}", config.pipeline_id, config.src_uri);
        config.file_loop = inputConfig["stream"]["file-loop"].asBool();
        LOG_INFO("Pipeline[{}]: file-loop: {}", config.pipeline_id, config.file_loop);
        config.rtsp_latency = inputConfig["stream"]["rtsp-latency"].asInt();
        LOG_INFO("Pipeline[{}]: rtsp-latency: {}", config.pipeline_id, config.rtsp_latency);
        config.rtp_protocol = inputConfig["stream"]["rtp-protocol"].asInt();
        LOG_INFO("Pipeline[{}]: rtp-protocol: {}", config.pipeline_id, config.rtp_protocol);
        config.src_device = inputConfig["usb-camera"]["device"].asString();
        LOG_INFO("Pipeline[{}]: usb camera device: {}", config.pipeline_id, config.src_device);
        config.src_format = inputConfig["usb-camera"]["format"].asString();
        LOG_INFO("Pipeline[{}]: usb camera output format: {}", config.pipeline_id, config.src_format);
        config.src_framerate_n = inputConfig["usb-camera"]["framerate-n"].asInt();
        LOG_INFO("Pipeline[{}]: usb camera output height: {}", config.pipeline_id, config.src_framerate_n);
        config.src_framerate_d = inputConfig["usb-camera"]["framerate-d"].asInt();
        LOG_INFO("Pipeline[{}]: usb camera output height: {}", config.pipeline_id, config.src_framerate_d);

        config.src_width = inputConfig["width"].asInt();
        LOG_INFO("Pipeline[{}]: input width: {}", config.pipeline_id, config.src_width);
        config.src_height = inputConfig["height"].asInt();
        LOG_INFO("Pipeline[{}]: input height: {}", config.pipeline_id, config.src_height);
        config.src_live_source = inputConfig["live-source"].asBool();
        LOG_INFO("Pipeline[{}]: input live source: {}", config.pipeline_id, config.src_live_source);
        config.src_sync = inputConfig["sync"].asBool();
        LOG_INFO("Pipeline[{}]: input muxer sync: {}", config.pipeline_id, config.src_sync);
        config.src_memory_type = inputConfig["memory-type"].asInt();
        LOG_INFO("Pipeline[{}]: input memory type: {}", config.pipeline_id, config.src_memory_type);
    
        config.bbox_thickness = inputConfig["bbox-thickness"].asInt();
        LOG_INFO("Pipeline[{}]: osd bbox thickness: {}", config.pipeline_id, config.bbox_thickness);
        config.text_thickness = inputConfig["text-thickness"].asInt();
        LOG_INFO("Pipeline[{}]: osd text thickness: {}", config.pipeline_id, config.text_thickness);

        config.bbox_rgb = std::make_tuple(inputConfig["bbox-rgb"][0].asInt(),
                                            inputConfig["bbox-rgb"][1].asInt(),
                                            inputConfig["bbox-rgb"][2].asInt());
        LOG_INFO("Pipeline[{}]: osd bbox rgb: [{}, {}, {}]", config.pipeline_id,
            inputConfig["bbox-rgb"][0].asInt(), inputConfig["bbox-rgb"][1].asInt(), inputConfig["bbox-rgb"][2].asInt());
        config.text_rgb = std::make_tuple(inputConfig["text-rgb"][0].asInt(),
                                            inputConfig["text-rgb"][1].asInt(),
                                            inputConfig["text-rgb"][2].asInt());
        LOG_INFO("Pipeline[{}]: osd text rgb: [{}, {}, {}]", config.pipeline_id,
            inputConfig["text-rgb"][0].asInt(), inputConfig["text-rgb"][1].asInt(), inputConfig["text-rgb"][2].asInt());
    }

    if (root.isMember("output-config")) {
        Json::Value outputConfig = root["output-config"];
        if (outputConfig.isMember("display")) {
            Json::Value displayConfig = outputConfig["display"];
            config.enable_hdmi = displayConfig["enable"].asBool();
            LOG_INFO("Pipeline[{}]: enable-hdmi: {}", config.pipeline_id, config.enable_hdmi);
            config.hdmi_sync = displayConfig["sync"].asBool();
            LOG_INFO("Pipeline[{}]: hdmi-sync: {}", config.pipeline_id, config.hdmi_sync);
            config.window_x = displayConfig["left"].asInt();
            LOG_INFO("Pipeline[{}]: window-x: {}", config.pipeline_id, config.window_x);
            config.window_y = displayConfig["top"].asInt();
            LOG_INFO("Pipeline[{}]: window-y: {}", config.pipeline_id, config.window_y);
            config.window_width = displayConfig["width"].asInt();
            LOG_INFO("Pipeline[{}]: window-width: {}", config.pipeline_id, config.window_width);
            config.window_height = displayConfig["height"].asInt();
            LOG_INFO("Pipeline[{}]: window-height: {}", config.pipeline_id, config.window_height);
        }

        if (outputConfig.isMember("rtmp")) {
            Json::Value rtmpConfig = outputConfig["rtmp"];
            config.enable_rtmp = rtmpConfig["enable"].asBool();
            LOG_INFO("Pipeline[{}]: enable-rtmp: {}", config.pipeline_id, config.enable_rtmp);
            config.enc_bitrate = rtmpConfig["bitrate"].asInt();
            LOG_INFO("Pipeline[{}]: encode-birtate: {}", config.pipeline_id, config.enc_bitrate);
            config.enc_iframe_interval = rtmpConfig["iframeinterval"].asInt();
            LOG_INFO("Pipeline[{}]: encode-iframeinterval: {}", config.pipeline_id, config.enc_iframe_interval);
            config.rtmp_uri = rtmpConfig["uri"].asString();
            LOG_INFO("Pipeline[{}]: rtmp-uri: {}", config.pipeline_id, config.rtmp_uri);
        }

        if (outputConfig.isMember("inference")) {
            Json::Value inferenceConfig = outputConfig["inference"];
            config.enable_appsink = inferenceConfig["enable"].asBool();
            LOG_INFO("Pipeline[{}]: enable-appsink: {}", config.pipeline_id, config.enable_appsink);
            config.cvt_memory_type = inferenceConfig["memory-type"].asInt();
            LOG_INFO("Pipeline[{}]: videoconvert memory type: {}", config.pipeline_id, config.cvt_memory_type);
            config.cvt_format = inferenceConfig["format"].asString();
            LOG_INFO("Pipeline[{}]: videoconvert format: {}", config.pipeline_id, config.cvt_format);
        }
    }
}

static bool configParse(YoloChannelConfig& config, std::string& configString)
{
    LOG_INFO("Parse configurations from json string.");

    Json::Value root;
    g_reader.parse(configString, root);

    if (root.isMember("name")) config.m_chanelId = root["name"].asString();
    else {
        LOG_ERROR("Parse channel config error: lack parameter 'name'.");
        return false;
    }

    if (root.isMember("VideoPipeline-Config")) {
        configVideoPipelineConfig(config.m_vpConfig, root["VideoPipeline-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoPipeline-Config'.");
        return false;
    }

    if (root.isMember("VideoAnalyzer-Config")) {
        configVideoAnalyzerConfig(config.m_vaConfig, root["VideoAnalyzer-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoAnalyzer-Config'.");
        return false;
    }

    return true;
}

static bool configParse(YoloChannelConfig& config, std::ifstream& configStream)
{
    LOG_INFO("Parse configurations from ifstream.");

    Json::Value root;
    g_reader.parse(in, root);

    if (root.isMember("name")) config.m_chanelId = root["name"].asString();
    else {
        LOG_ERROR("Parse channel config error: lack parameter 'name'.");
        return false;
    }

    if (root.isMember("VideoPipeline-Config")) {
        configVideoPipelineConfig(config.m_vpConfig, root["VideoPipeline-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoPipeline-Config'.");
        return false;
    }

    if (root.isMember("VideoAnalyzer-Config")) {
        configVideoAnalyzerConfig(config.m_vaConfig, root["VideoAnalyzer-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoAnalyzer-Config'.");
        return false;
    }

    return true;
}