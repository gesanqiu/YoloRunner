/*
 * @Description: A tiny http server for MediaAI.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-18 16:55:50
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-19 02:42:42
 */

#pragma once

#include <httplib.h>

#include "ChannelController.h"

namespace edge {

class EdgeServer {
public:
    EdgeServer(std::string ip, uint32_t port);
    ~EdgeServer();

    void InstallApi();

    void Init();
    void DeInit();

private:
    std::string m_ip;
    uint32_t    m_port;
    Json::Reader m_reader;
    std::shared_ptr<ChannelController> m_channelController;
    std::shared_ptr<httplib::Server> m_httpServer;
};

}