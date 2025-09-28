#pragma once
#include "RouterHandler.h"
#include"CommonConnectionPool.h"
#include"AiChatServer.h"
#include"JsonUtil.h"
#include<atomic>

class RegisterHandler : public RouterHandler 
{
public:
    explicit RegisterHandler(AiChatServer* server) : server_(server) {}

    void handle(const HttpRequest& req, HttpResponse* resp) override;
private:
    int insertUser(const std::string& username, const std::string& password);
    bool isUserExist(const std::string& username);
    bool UserIsExist(const std::string &username);
private:
    AiChatServer* server_;
   ConnectPool*  dbconn = ConnectPool::getInstance();
   std::atomic_int64_t count_ = 1005;
};