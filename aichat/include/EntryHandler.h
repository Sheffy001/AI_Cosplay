#pragma once
#include"RouterHandler.h"
#include"FileUtil.h"
#include"JsonUtil.h"
#include"AiChatServer.h"
class FileServer;



class EntryHandler:public RouterHandler
{
public:
    explicit EntryHandler(AiChatServer* server)  {}

    void handle(const HttpRequest& req, HttpResponse* resp) override;

};