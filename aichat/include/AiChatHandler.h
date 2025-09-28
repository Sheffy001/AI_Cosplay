#pragma once
#include"AiChatServer.h"
#include"RouterHandler.h"
#include"CommonConnectionPool.h"
#include"JsonUtil.h"
#include"FileUtil.h"

class AiChatServer;
class AiChatHandler : public RouterHandler
{
public:
    explicit AiChatHandler(AiChatServer* server) : server_(server) {}

    void handle(const HttpRequest& req, HttpResponse* resp) override;
private:
    AiChatServer* server_;
};