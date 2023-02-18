/*
 * @Description: Test program of YoloChannel.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2022-07-15 22:07:33
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-18 18:18:12
 */

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <gflags/gflags.h>

#include "Common.h"
#include "ChannelController.h"

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

    LOG_ERROR("Can't stat config file: {}", value);
    return false;
}

DEFINE_string(config_file, "./pipeline.json", "YoloRunner config file.");
DEFINE_validator(config_file, &validateConfigPath);

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    gst_init(&argc, &argv);

    g_setenv("GST_DEBUG_DUMP_DOT_DIR", "/home/ricardo/workSpace/YoloRunner/edge/build", true);

    edge::ChannelController* controller = new edge::ChannelController();
    std::ifstream in(FLAGS_config_file, std::ios::in | std::ios::binary);
    std::string config((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()); 

    if (!in.is_open()) {
        LOG_ERROR("Failed to open config file: {}", FLAGS_config_file);
        goto exit;
    }

    if (!(g_main_loop = g_main_loop_new(NULL, FALSE))) {
        LOG_ERROR("Failed to new a object with type GMainLoop");
        goto exit;
    }

    while (true) {
        if (!controller->AddChannel(config)) {
            LOG_ERROR("ChannelController add channel failed.");
            goto exit;
        }

        if (!controller->Start()) {
            LOG_ERROR("ChannelController add channel failed.");
            goto exit;
        }

        sleep(10);

        if (!controller->DeInit()) {
            LOG_ERROR("ChannelController add channel failed.");
            goto exit;
        }

        sleep(10);
    }

    g_main_loop_run(g_main_loop);

exit:
    if (nullptr != controller) {
        // yolo_channel->DeInit();
        delete controller;
        controller = nullptr;
    }

    in.close();

    if (g_main_loop) g_main_loop_unref(g_main_loop);

    google::ShutDownCommandLineFlags();
    return 0;
}