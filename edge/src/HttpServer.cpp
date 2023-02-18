/*
 * @Description: A tiny http server for MediaAI.
 * @version: 1.0
 * @Author: Ricardo Lu<shenglu1202@163.com>
 * @Date: 2023-02-18 16:56:02
 * @LastEditors: Ricardo Lu
 * @LastEditTime: 2023-02-19 02:42:56
 */

#include "HttpServer.h"

namespace edge {

EdgeServer::EdgeServer(std::string ip, uint32_t port)
{
    m_ip = ip;
    m_port = port;
    m_channelController = std::make_shared<ChannelController>();
    m_httpServer = std::make_shared<httplib::Server>();
}

EdgeServer::~EdgeServer()
{
    DeInit();
}

void EdgeServer::Init()
{
    m_channelController->Init();
}

void EdgeServer::DeInit()
{
    if (m_channelController) m_channelController.reset();
    if (m_httpServer) m_httpServer.reset();
}

void EdgeServer::InstallApi()
{
    m_httpServer->Post("/addChannel", [this](const httplib::Request& req, httplib::Response& res) {
        auto body = req.body;
        this->m_channelController->AddChannel(body);
        res.set_content("Add channel success.", "text/plain");
    });

    m_httpServer->Post("/deleteChannel", [this](const httplib::Request& req, httplib::Response& res){
        if (req.has_header("Content-Length")) {
            auto val = req.get_header_value("Content-Length");
            printf("Content-Length: %s\n", val.c_str());
        }

        if (req.has_param("channelId")) {
            auto val = req.get_param_value("channelId");
            this->m_channelController->DeletaChannel(val);
            res.set_content("Delete channel success.", "text/plain");
        } else {
            printf("Request doesn't have parameter 'channelId'");
            res.set_content("Delete channel failed, bad request.", "text/plain");
        }
    });

    m_httpServer->Post("/startChannel", [this](const httplib::Request& req, httplib::Response& res){
        if (req.has_header("Content-Length")) {
            auto val = req.get_header_value("Content-Length");
            printf("Content-Length: %s\n", val.c_str());
        }

        if (req.has_param("channelId")) {
            auto val = req.get_param_value("channelId");
            this->m_channelController->StartChannel(val);
            res.set_content("Start channel success.", "text/plain");
        } else {
            printf("Request doesn't have parameter 'channelId'");
            res.set_content("Start channel failed, bad request.", "text/plain");
        }
    });

    m_httpServer->Post("/stopChannel", [this](const httplib::Request& req, httplib::Response& res){
        if (req.has_header("Content-Length")) {
            auto val = req.get_header_value("Content-Length");
            printf("Content-Length: %s\n", val.c_str());
        }

        if (req.has_param("channelId")) {
            auto val = req.get_param_value("channelId");
            LOG_INFO("Try to stop channel: {}.", val);
            this->m_channelController->StopChannel(val);
            res.set_content("Stop channel success.", "text/plain");
        } else {
            printf("Request doesn't have parameter 'channelId'");
            res.set_content("Stop channel failed, bad request.", "text/plain");
        }
    });

    m_httpServer->Get("/getChannelList", [](const httplib::Request& req, httplib::Response& res){
        auto str = ConfigCenter::getInstance().GetChannelList();
        if (!str.empty()) {
            res.set_content(str, "text/plain");
        } else {
            res.set_content("None channel added right now", "text/plain");
        }
    });

    m_httpServer->Get("/getChannelConfig", [](const httplib::Request& req, httplib::Response& res){
        if (req.has_param("channelId")) {
            auto val = req.get_param_value("channelId");
            LOG_INFO("getChannelConfig: {}", val);
            auto str = ConfigCenter::getInstance().GetChannelConfigString(val);
            if (!str.empty()) {
                res.set_content(str, "text/plain");
            } else {
                res.set_content(str, "text/plain");
            }
        } else {
            LOG_WARN("getChannelConfig() requested without channel id.");
            res.set_content("Get channel config failed, bad request.", "text/plain");
        }
    });

    m_httpServer->listen(m_ip, m_port);
}



}