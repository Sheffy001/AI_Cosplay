#pragma once
#include"AiChatServer.h"
#include"RouterHandler.h"
#include"CommonConnectionPool.h"
#include"JsonUtil.h"


class RoleStoryHandler : public RouterHandler 
{
public:
    explicit RoleStoryHandler(AiChatServer* server) : server_(server) {}
    
    void handle(const HttpRequest& req, HttpResponse* resp) override;
    std::string getStroy(int roleid);

private:
    AiChatServer*       server_;
    ConnectPool*     dbconn = ConnectPool::getInstance();

};