#pragma once
#include"AiChatServer.h"
#include"RouterHandler.h"
#include"CommonConnectionPool.h"
#include"JsonUtil.h"


class RoleinfoHandler : public RouterHandler 
{
public:
    explicit RoleinfoHandler(AiChatServer* server) : server_(server) {}
    
    void handle(const HttpRequest& req, HttpResponse* resp) override;
    void getInfo(vector<json>&info);
    std::string getInfo(int roleid);
private:
    AiChatServer*       server_;
    ConnectPool*     dbconn = ConnectPool::getInstance();

};