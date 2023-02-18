/*
 * @Description: Test program of YoloChannel.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2022-07-15 22:07:33
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-19 03:07:59
 */

#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <gflags/gflags.h>

#include "Common.h"
#include "ChannelController.h"
#include "HttpServer.h"

static GMainLoop* g_main_loop = NULL;

DEFINE_string(ip, "localhost", "Http Server listen ip.");
DEFINE_uint32(port, 18080, "Http Server listen port.");

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);

    gst_init(&argc, &argv);

    g_setenv("GST_DEBUG_DUMP_DOT_DIR", "/home/ricardo/workSpace/YoloRunner/edge/build", true);

    edge::EdgeServer* svr = new edge::EdgeServer(FLAGS_ip, FLAGS_port);
    svr->InstallApi();

    g_main_loop_run(g_main_loop);

exit:

    if (g_main_loop) g_main_loop_unref(g_main_loop);

    google::ShutDownCommandLineFlags();
    return 0;
}