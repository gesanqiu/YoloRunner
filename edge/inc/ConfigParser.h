/*
 * @Description: YoloRunner config parser.
 * @version: 1.1
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-12 12:43:29
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-17 21:42:54
 */

#pragma once

#include "Common.h"
#include "YoloChannel.h"

#define CONFIG_PARSE_SUCCESS    0
#define CONFIG_LACK_MEMBER      1
#define CONFIG_VALUE_TYPE_ERROR 2

static Json::Reader g_reader;

using namespace edge;

static int ConfigVideoTaskConfig(VideoTaskConfig& config, Json::Value& value)
{
    if (value.isMember("name")) {
        if (value["name"].isString()) {
            config.analyzer_id = value["name"].asString();
            LOG_INFO("VideoAnalyzer ID: {}", config.analyzer_id);
        } else {
            LOG_ERROR("VideoAnalyzer ID error type.");
            return CONFIG_VALUE_TYPE_ERROR;
        }
    } else {
        LOG_ERROR("Parse VideoAnalyzer config error: lack member 'name'.");
        return CONFIG_LACK_MEMBER;
    }

    if (value.isMember("model-config")) {
        Json::Value modelConfig = value["model-config"];

        if (modelConfig.isMember("engine-file")) {
            if (modelConfig["engine-file"].isString()) {
                config.engine_file = modelConfig["engine-file"].asString();
                LOG_INFO("VideoAnalyzer[{}]: engine file: {}", config.analyzer_id, config.engine_file);
            } else {
                LOG_ERROR("VideoAnalyzer[{}]: engine file error type.", config.analyzer_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse model config error: lack member 'engine-file'.");
            return CONFIG_LACK_MEMBER;
        }

        if (modelConfig.isMember("classes-file")) {
            if (modelConfig["classes-file"].isString()) {
                config.classes_file = modelConfig["classes-file"].asString();
                LOG_INFO("VideoAnalyzer[{}]: classes file: {}", config.analyzer_id, config.classes_file);
            } else {
                LOG_ERROR("VideoAnalyzer[{}]: classes-file error type.", config.analyzer_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse model config error: lack member 'classes-file'.");
            return CONFIG_LACK_MEMBER;
        }

        if (modelConfig.isMember("score-threshold")) {
            if (modelConfig["score-threshold"].isDouble()) {
                config.score_threshold = modelConfig["score-threshold"].asDouble();
                LOG_INFO("VideoAnalyzer[{}]: score threshold: {}", config.analyzer_id, config.score_threshold);
            } else {
                LOG_ERROR("VideoAnalyzer[{}]: score-threshold error type.", config.analyzer_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse model config error: lack member 'score-threshold'.");
            return CONFIG_LACK_MEMBER;
        }

        if (modelConfig.isMember("nms-threshold")) {
            if (modelConfig["nms-threshold"].isDouble()) {
                config.nms_threshold = modelConfig["nms-threshold"].asDouble();
                LOG_INFO("VideoAnalyzer[{}]: nms threshould: {}", config.analyzer_id, config.nms_threshold);
            } else {
                LOG_ERROR("VideoAnalyzer[{}]: nms-threshold error type.", config.analyzer_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse model config error: lack member 'nms-threshold'.");
            return CONFIG_LACK_MEMBER;
        }
    } else {
        LOG_ERROR("Parse VideoAnalyzer config error: lack member 'model-config'.");
        return CONFIG_LACK_MEMBER;
    }

    return CONFIG_PARSE_SUCCESS;
}

static int ConfigVideoPipelineConfig(VideoPipelineConfig& config, Json::Value& value)
{
    if (value.isMember("name")) {
        if (value["name"].isString()) {
            config.pipeline_id = value["name"].asString();
            LOG_INFO("VideoPipeline ID: {}", config.pipeline_id);
        } else {
            LOG_ERROR("VideoPipeline ID error type.");
            return CONFIG_VALUE_TYPE_ERROR;
        }
    } else {
        LOG_ERROR("Parse VideoPipeline config error: lack member 'name'.");
        return CONFIG_LACK_MEMBER;
    }

    if (value.isMember("input-config")) {
        Json::Value inputConfig = value["input-config"];

        if (inputConfig.isMember("type")) {
            if (inputConfig["type"].isInt()) {
                config.input_type = inputConfig["type"].asInt();    // 0-MP4 / 1-RTSP / 2-USB-Camera
                LOG_INFO("VideoPipeline[{}]: input type: {}", config.pipeline_id, config.input_type);
                if (config.input_type == 2) {
                    if (inputConfig.isMember("usb-camera")) {
                        Json::Value usbCameraConfig = inputConfig["usb-camera"];

                        if (usbCameraConfig.isMember("device")) {
                            if (usbCameraConfig["device"].isString()) {
                                config.src_device = usbCameraConfig["device"].asString();
                                LOG_INFO("VideoPipeline[{}]: input usb-camera device: {}", config.pipeline_id, config.src_device);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input usb-camera device error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse usb-camera error: lack member 'device'.");
                            return CONFIG_LACK_MEMBER;
                        }

                        if (usbCameraConfig.isMember("format")) {
                            if (usbCameraConfig["format"].isString()) {
                                config.src_format = usbCameraConfig["format"].asString();
                                LOG_INFO("VideoPipeline[{}]: input usb-camera output format: {}", config.pipeline_id, config.src_format);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input usb-camera format error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse usb-camera error: lack member 'format'.");
                            return CONFIG_LACK_MEMBER;
                        }

                        if (usbCameraConfig.isMember("framerate-n")) {
                            if (usbCameraConfig["framerate-n"].isInt()) {
                                config.src_framerate_n = usbCameraConfig["framerate-n"].asInt();
                                LOG_INFO("VideoPipeline[{}]: input usb-camera output framerate-n: {}", config.pipeline_id, config.src_framerate_n);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input usb-camera framerate-n error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse usb-camera error: lack member 'framerate-n'.");
                            return CONFIG_LACK_MEMBER;
                        }

                        if (usbCameraConfig.isMember("framerate-d")) {
                            if (usbCameraConfig["framerate-d"].isInt()) {
                                config.src_framerate_d = usbCameraConfig["framerate-d"].asInt();
                                LOG_INFO("VideoPipeline[{}]: input usb-camera output framerate-d: {}", config.pipeline_id, config.src_framerate_d);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input usb-camera framerate-d error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse usb-camera error: lack member 'framerate-d'.");
                            return CONFIG_LACK_MEMBER;
                        }
                    } else {
                        LOG_ERROR("Parse input-config error: lack member 'usb-camera'.");
                        return CONFIG_LACK_MEMBER;
                    }   
                } else {
                    if (inputConfig.isMember("stream")) {
                        Json::Value streamConfig = inputConfig["stream"];

                        if (streamConfig.isMember("uri")) {
                            if (streamConfig["uri"].isString()) {
                                config.src_uri = streamConfig["uri"].asString();
                                LOG_INFO("VideoPipeline[{}]: input stream uri: {}", config.pipeline_id, config.src_uri);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input stream uri error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse stream error: lack member 'uri'.");
                            return CONFIG_LACK_MEMBER;
                        }

                        if (streamConfig.isMember("file-loop")) {
                            if (streamConfig["file-loop"].isBool()) {
                                config.file_loop = streamConfig["file-loop"].asBool();
                                LOG_INFO("VideoPipeline[{}]: input stream file-loop: {}", config.pipeline_id, config.file_loop);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input stream file-loop error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse stream error: lack member 'file-loop'.");
                            return CONFIG_LACK_MEMBER;
                        }

                        if (streamConfig.isMember("rtsp-latency")) {
                            if (streamConfig["rtsp-latency"].isInt()) {
                                config.rtsp_latency = streamConfig["rtsp-latency"].asInt();
                                LOG_INFO("VideoPipeline[{}]: input stream rtsp-latency: {}", config.pipeline_id, config.rtsp_latency);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input stream rtsp-latency error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse stream error: lack member 'rtsp-latency'.");
                            return CONFIG_LACK_MEMBER;
                        }

                        if (streamConfig.isMember("rtp-protocol")) {
                            if (streamConfig["rtp-protocol"].isInt()) {
                                config.rtp_protocol = streamConfig["rtp-protocol"].asInt();
                                LOG_INFO("VideoPipeline[{}]: input stream rtp-protocol: {}", config.pipeline_id, config.rtp_protocol);
                            } else {
                                LOG_ERROR("VideoPipeline[{}]: input stream rtp-protocol error type.", config.pipeline_id);
                                return CONFIG_VALUE_TYPE_ERROR;
                            }
                        } else {
                            LOG_ERROR("Parse stream error: lack member 'rtp-protocol'.");
                            return CONFIG_LACK_MEMBER;
                        }
                    } else {
                        LOG_ERROR("Parse input-config error: lack member 'stream'.");
                        return CONFIG_LACK_MEMBER;
                    }
                }
            } else {
                LOG_ERROR("VideoPipeline[{}]: input type error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse input-config error: lack member 'type'.");
            return CONFIG_LACK_MEMBER;
        }        

        if (inputConfig.isMember("width")) {
            if (inputConfig["width"].isInt()) {
                config.src_width = inputConfig["width"].asInt();
                LOG_INFO("VideoPipeline[{}]: input width: {}", config.pipeline_id, config.src_width);
            } else {
                LOG_ERROR("VideoPipeline[{}]: input width error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse input-config error: lack member 'width'.");
            return CONFIG_LACK_MEMBER;
        }

        if (inputConfig.isMember("height")) {
            if (inputConfig["height"].isInt()) {
                config.src_height = inputConfig["height"].asInt();
                LOG_INFO("VideoPipeline[{}]: input height: {}", config.pipeline_id, config.src_height);
            } else {
                LOG_ERROR("VideoPipeline[{}]: input height error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse input-config error: lack member 'height'.");
            return CONFIG_LACK_MEMBER;
        }

        if (inputConfig.isMember("live-source")) {
            if (inputConfig["live-source"].isBool()) {
                config.src_live_source = inputConfig["live-source"].asBool();
                LOG_INFO("VideoPipeline[{}]: input live-source: {}", config.pipeline_id, config.src_live_source);
            } else {
                LOG_ERROR("VideoPipeline[{}]: input live-source error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse input-config error: lack member 'live-source'.");
            return CONFIG_LACK_MEMBER;
        }

        if (inputConfig.isMember("sync")) {
            if (inputConfig["sync"].isBool()) {
                config.src_sync = inputConfig["sync"].asBool();
                LOG_INFO("VideoPipeline[{}]: input sync: {}", config.pipeline_id, config.src_sync);
            } else {
                LOG_ERROR("VideoPipeline[{}]: input sync error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse input-config error: lack member 'sync'.");
            return CONFIG_LACK_MEMBER;
        }

        if (inputConfig.isMember("memory-type")) {
            if (inputConfig["memory-type"].isInt()) {
                config.src_memory_type = inputConfig["memory-type"].asInt();
                LOG_INFO("VideoPipeline[{}]: input memory-type: {}", config.pipeline_id, config.src_memory_type);
            } else {
                LOG_ERROR("VideoPipeline[{}]: input memory-type error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse input-config error: lack member 'memory-type'.");
            return CONFIG_LACK_MEMBER;
        }

        
    } else {
        LOG_ERROR("Parse VideoPipeline config error: lack member 'input-config'.");
        return CONFIG_LACK_MEMBER;
    }

    if (value.isMember("output-config")) {
        Json::Value outputConfig = value["output-config"];
        if (outputConfig.isMember("display")) {
            Json::Value displayConfig = outputConfig["display"];

            if (displayConfig.isMember("enable")) {
                if (displayConfig["enable"].isBool()) {
                    config.enable_hdmi = displayConfig["enable"].asBool();
                    LOG_INFO("VideoPipeline[{}]: display enable-hdmi: {}", config.pipeline_id, config.enable_hdmi);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: display enable-hdmi error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse display config error: lack member 'enable'.");
                return CONFIG_LACK_MEMBER;
            }

            if (displayConfig.isMember("sync")) {
                if (displayConfig["sync"].isBool()) {
                    config.hdmi_sync = displayConfig["sync"].asBool();
                    LOG_INFO("VideoPipeline[{}]: display hdmi-sync: {}", config.pipeline_id, config.hdmi_sync);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: display enable-sync error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse display config error: lack member 'sync'.");
                return CONFIG_LACK_MEMBER;
            }

            if (displayConfig.isMember("window-x")) {
                if (displayConfig["window-x"].isInt()) {
                    config.window_x = displayConfig["window-x"].asInt();
                    LOG_INFO("VideoPipeline[{}]: window-x: {}", config.pipeline_id, config.window_x);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: display window-x error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse display config error: lack member 'window-x'.");
                return CONFIG_LACK_MEMBER;
            }

            if (displayConfig.isMember("window-y")) {
                if (displayConfig["window-y"].isInt()) {
                    config.window_y = displayConfig["window-y"].asInt();
                    LOG_INFO("VideoPipeline[{}]: window-y: {}", config.pipeline_id, config.window_y);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: display window-y error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse display config error: lack member 'window-y'.");
                return CONFIG_LACK_MEMBER;
            }

            if (displayConfig.isMember("window-width")) {
                if (displayConfig["window-width"].isInt()) {
                    config.window_width = displayConfig["window-width"].asInt();
                    LOG_INFO("VideoPipeline[{}]: window-width: {}", config.pipeline_id, config.window_width);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: display window-width error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse display config error: lack member 'window-width'.");
                return CONFIG_LACK_MEMBER;
            }

            if (displayConfig.isMember("window-height")) {
                if (displayConfig["window-height"].isInt()) {
                    config.window_height = displayConfig["window-height"].asInt();
            LOG_INFO("VideoPipeline[{}]: window-height: {}", config.pipeline_id, config.window_height);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: display window-height error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse display config error: lack member 'window-height'.");
                return CONFIG_LACK_MEMBER;
            }
        }  else {
            LOG_ERROR("Parse output-config error: lack member 'display'.");
            return CONFIG_LACK_MEMBER;
        }

        if (outputConfig.isMember("rtmp")) {
            Json::Value rtmpConfig = outputConfig["rtmp"];

            if (rtmpConfig.isMember("enable")) {
                if (rtmpConfig["enable"].isBool()) {
                    config.enable_rtmp = rtmpConfig["enable"].asBool();
                    LOG_INFO("VideoPipeline[{}]: enable-rtmp: {}", config.pipeline_id, config.enable_rtmp);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: enable-rtmp error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse rtmp config error: lack member 'enable'.");
                return CONFIG_LACK_MEMBER;
            }

            if (rtmpConfig.isMember("bitrate")) {
                if (rtmpConfig["bitrate"].isInt()) {
                    config.enc_bitrate = rtmpConfig["bitrate"].asInt();
                    LOG_INFO("VideoPipeline[{}]: encode-birtate: {}", config.pipeline_id, config.enc_bitrate);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: encode-birtate error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse rtmp config error: lack member 'bitrate'.");
                return CONFIG_LACK_MEMBER;
            }

            if (rtmpConfig.isMember("iframeinterval")) {
                if (rtmpConfig["iframeinterval"].isInt()) {
                    config.enc_iframe_interval = rtmpConfig["iframeinterval"].asInt();
                    LOG_INFO("VideoPipeline[{}]: encode-iframeinterval: {}", config.pipeline_id, config.enc_iframe_interval);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: encode-iframeinterval error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse rtmp config error: lack member 'iframeinterval'.");
                return CONFIG_LACK_MEMBER;
            }

            if (rtmpConfig.isMember("uri")) {
                if (rtmpConfig["uri"].isString()) {
                    config.rtmp_uri = rtmpConfig["uri"].asString();
                    LOG_INFO("VideoPipeline[{}]: rtmp-uri: {}", config.pipeline_id, config.rtmp_uri);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: rtmp-uri error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse rtmp config error: lack member 'uri'.");
                return CONFIG_LACK_MEMBER;
            }
        } else {
            LOG_ERROR("Parse output-config error: lack member 'rtmp'.");
            return CONFIG_LACK_MEMBER;
        }

        if (outputConfig.isMember("inference")) {
            Json::Value inferenceConfig = outputConfig["inference"];

            if (inferenceConfig.isMember("enable")) {
                if (inferenceConfig["enable"].isBool()) {
                    config.enable_appsink = inferenceConfig["enable"].asBool();
                    LOG_INFO("VideoPipeline[{}]: enable-inference: {}", config.pipeline_id, config.enable_appsink);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: enable-inference error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse inference config error: lack member 'enable'.");
                return CONFIG_LACK_MEMBER;
            }
    
            if (inferenceConfig.isMember("memory-type")) {
                if (inferenceConfig["memory-type"].isInt()) {
                    config.cvt_memory_type = inferenceConfig["memory-type"].asInt();
                    LOG_INFO("VideoPipeline[{}]: inference nvvideoconvert memory-type: {}", config.pipeline_id, config.cvt_memory_type);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: inference memory-type error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse inference config error: lack member 'memory-type'.");
                return CONFIG_LACK_MEMBER;
            }

            if (inferenceConfig.isMember("format")) {
                if (inferenceConfig["format"].isString()) {
                    config.cvt_format = inferenceConfig["format"].asString();
                    LOG_INFO("VideoPipeline[{}]: inference nvvideoconvert output format: {}", config.pipeline_id, config.cvt_format);
                } else {
                    LOG_ERROR("VideoPipeline[{}]: inference nvvideoconvert output format error type.", config.pipeline_id);
                    return CONFIG_VALUE_TYPE_ERROR;
                }
            } else {
                LOG_ERROR("Parse inference config error: lack member 'format'.");
                return CONFIG_LACK_MEMBER;
            }
        }else {
            LOG_ERROR("Parse output-config error: lack member 'inference'.");
            return CONFIG_LACK_MEMBER;
        }

        if (outputConfig.isMember("bbox-thickness")) {
            if (outputConfig["bbox-thickness"].isInt()) {
                config.bbox_thickness = outputConfig["bbox-thickness"].asInt();
                LOG_INFO("VideoPipeline[{}]: input osd bbox thickness: {}", config.pipeline_id, config.bbox_thickness);
            } else {
                LOG_ERROR("VideoPipeline[{}]: input osd bbox-thickness error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse output-config error: lack member 'bbox-thickness'.");
            return CONFIG_LACK_MEMBER;
        }
        
        if (outputConfig.isMember("text-thickness")) {
            if (outputConfig["text-thickness"].isInt()) {
                config.text_thickness = outputConfig["text-thickness"].asInt();
                LOG_INFO("VideoPipeline[{}]: output osd text-thickness: {}", config.pipeline_id, config.text_thickness);
            } else {
                LOG_ERROR("VideoPipeline[{}]: output osd text-thickness error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse output-config error: lack member 'text-thickness'.");
            return CONFIG_LACK_MEMBER;
        }

        if (outputConfig.isMember("bbox-rgb")) {
            if (outputConfig["bbox-rgb"].isArray()) {
                config.bbox_rgb = std::make_tuple(outputConfig["bbox-rgb"][0].asInt(),
                                            outputConfig["bbox-rgb"][1].asInt(),
                                            outputConfig["bbox-rgb"][2].asInt());
                LOG_INFO("VideoPipeline[{}]: osd bbox-rgb: [{}, {}, {}]", config.pipeline_id,
                    outputConfig["bbox-rgb"][0].asInt(), outputConfig["bbox-rgb"][1].asInt(), outputConfig["bbox-rgb"][2].asInt());
            } else {
                LOG_ERROR("VideoPipeline[{}]: output osd bbox-rgb error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse output-config error: lack member 'bbox-rgb'.");
            return CONFIG_LACK_MEMBER;
        }

        if (outputConfig.isMember("text-rgb")) {
            if (outputConfig["text-rgb"].isArray()) {
                config.text_rgb = std::make_tuple(outputConfig["text-rgb"][0].asInt(),
                                            outputConfig["text-rgb"][1].asInt(),
                                            outputConfig["text-rgb"][2].asInt());
                LOG_INFO("VideoPipeline[{}]: osd text rgb: [{}, {}, {}]", config.pipeline_id,
                    outputConfig["text-rgb"][0].asInt(), outputConfig["text-rgb"][1].asInt(), outputConfig["text-rgb"][2].asInt());
            } else {
                LOG_ERROR("VideoPipeline[{}]: output osd text-rgb error type.", config.pipeline_id);
                return CONFIG_VALUE_TYPE_ERROR;
            }
        } else {
            LOG_ERROR("Parse output-config error: lack member 'text-rgb'.");
            return CONFIG_LACK_MEMBER;
        }
    } else {
        LOG_ERROR("Parse VideoPipeline config error: lack member 'output-config'.");
        return CONFIG_LACK_MEMBER;
    }

    return CONFIG_PARSE_SUCCESS;
}

static int ConfigParse(YoloChannelConfig& config, std::string& configString)
{
    LOG_INFO("Parse configurations from json string.");

    Json::Value root;
    g_reader.parse(configString, root);

    if (root.isMember("name")) {
        if (root["name"].isString()) {
            config.m_chanelId = root["name"].asString();
            LOG_INFO("Channel ID: {}", config.m_chanelId);
        } else {
            LOG_ERROR("Channel ID error type.");
            return CONFIG_VALUE_TYPE_ERROR;
        }
    } else {
        LOG_ERROR("Parse channel config error: lack member 'name'.");
        return CONFIG_LACK_MEMBER;
    }

    if (root.isMember("VideoPipeline-Config")) {
        ConfigVideoPipelineConfig(config.m_vpConfig, root["VideoPipeline-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoPipeline-Config'.");
        return CONFIG_LACK_MEMBER;
    }

    if (root.isMember("VideoAnalyzer-Config")) {
        ConfigVideoTaskConfig(config.m_vtConfig, root["VideoAnalyzer-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoAnalyzer-Config'.");
        return CONFIG_LACK_MEMBER;
    }

    return CONFIG_PARSE_SUCCESS;
}

static int ConfigParse(YoloChannelConfig& config, std::ifstream& configStream)
{
    LOG_INFO("Parse configurations from ifstream.");

    Json::Value root;
    g_reader.parse(configStream, root);

    if (root.isMember("name")) {
        if (root["name"].isString()) {
            config.m_chanelId = root["name"].asString();
            LOG_INFO("Channel ID: {}", config.m_chanelId);
        } else {
            LOG_ERROR("Channel ID error type.");
            return CONFIG_VALUE_TYPE_ERROR;
        }
    } else {
        LOG_ERROR("Parse channel config error: lack member 'name'.");
        return CONFIG_LACK_MEMBER;
    }

    if (root.isMember("VideoPipeline-Config")) {
        ConfigVideoPipelineConfig(config.m_vpConfig, root["VideoPipeline-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoPipeline-Config'.");
        return CONFIG_LACK_MEMBER;
    }

    if (root.isMember("VideoAnalyzer-Config")) {
        ConfigVideoTaskConfig(config.m_vtConfig, root["VideoAnalyzer-Config"]);
    } else {
        LOG_ERROR("Parse channel config error: lack member 'VideoAnalyzer-Config'.");
        return CONFIG_LACK_MEMBER;
    }

    return CONFIG_PARSE_SUCCESS;
}