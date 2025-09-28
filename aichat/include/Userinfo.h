#pragma once
#include"AiChatServer.h"
#include"RouterHandler.h"
#include"CommonConnectionPool.h"
#include"JsonUtil.h"


class UserinfoHandler : public RouterHandler 
{
public:
    explicit UserinfoHandler(AiChatServer* server) : server_(server) {}
    
    void handle(const HttpRequest& req, HttpResponse* resp) override;


private:
    AiChatServer*       server_;
    ConnectPool*     dbconn = ConnectPool::getInstance();

};