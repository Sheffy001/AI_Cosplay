#pragma once
#include "RouterHandler.h"
#include"CommonConnectionPool.h"
#include"AiChatServer.h"
#include"JsonUtil.h"

class LogoutHandler : public RouterHandler 
{
public:
    explicit LogoutHandler(AiChatServer* server) : server_(server) {}
    void handle(const HttpRequest& req, HttpResponse* resp) override;
private:
    AiChatServer* server_;
    ConnectPool*dbconn = ConnectPool::getInstance();
};