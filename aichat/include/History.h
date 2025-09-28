#pragma once

#include "RouterHandler.h"
#include "JsonUtil.h"
#include "AiChatServer.h"


class AiChatServer;
class History : public RouterHandler
{
public:
    explicit History(AiChatServer *server):aiserver(server){}

    void handle(const HttpRequest &req, HttpResponse *resp) override;
    void getMsg(const string& userid,const int chatid,std::vector<json>&v);
private:
    AiChatServer *aiserver;
    ConnectPool*dbconn = ConnectPool::getInstance();
};