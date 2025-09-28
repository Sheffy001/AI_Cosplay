#pragma once
#include"AiChatServer.h"
#include"RouterHandler.h"
#include"CommonConnectionPool.h"
#include"JsonUtil.h"


class LoginHandler : public RouterHandler 
{
public:
    explicit LoginHandler(AiChatServer* server) : server_(server) {}
    
    void handle(const HttpRequest& req, HttpResponse* resp) override;

private:
    int queryUserId(const string&username, const std::string& password,int&userid);

private:
    AiChatServer*       server_;
    ConnectPool*     dbconn = ConnectPool::getInstance();

};