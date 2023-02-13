/*
 * @Description: Test program of VideoPipeline.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2022-07-15 22:07:33
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-12 18:36:11
 */

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <gflags/gflags.h>

#include "Common.h"
#include "ConfigParser.h"
#include "YoloChannel.h"

static GMainLoop* g_main_loop = NULL;

static bool validateConfigPath(const char* name, const std::string& value) 
{ 
    if (0 == value.compare ("")) {
        LOG_ERROR("You must specify a config file!");
        return false;
    }

    struct stat statbuf;
    if (0 == stat(value.c_str(), &statbuf)) {
        return true;
    }

    LOG_ERROR("Can't stat model file: {}", value);
    return false;
}

DEFINE_string(config_file, "./pipeline.json", "YoloRunner config file.");
DEFINE_validator(config_file, &validateConfigPath);

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    YoloChannelConfig yolo_channel_config;
    std::ifstream in(FLAGS_config_file, std::ios::binary);
    configParse(yolo_channel_config, in);
    YoloChannel* yolo_channel = new YoloChannel(yolo_channel_config);

    gst_init(&argc, &argv);

    g_setenv("GST_DEBUG_DUMP_DOT_DIR", "/home/ricardo/workSpace/YoloRunner/edge/build", true);

    if (!(g_main_loop = g_main_loop_new(NULL, FALSE))) {
        LOG_ERROR("Failed to new a object with type GMainLoop");
        goto exit;
    }

    yolo_channel->Init();
    yolo_channel->Start();

    g_main_loop_run(g_main_loop);

exit:
    if (yolo_channel) {
        yolo_channel->DeInit();
        delete yolo_channel;
        yolo_channel = nullptr;
    }

    if (g_main_loop) g_main_loop_unref(g_main_loop);

    google::ShutDownCommandLineFlags();
    return 0;
}