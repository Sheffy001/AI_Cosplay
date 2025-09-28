#include "AiChatServer.h"
#include "RouterHandler.h"
#include "CommonConnectionPool.h"

class SaveHandler : public RouterHandler
{
public:
    explicit SaveHandler(AiChatServer *server) : server_(server) {}

    void handle(const HttpRequest &req, HttpResponse *resp) override;



private:
    AiChatServer *server_;
    ConnectPool* dbconn = ConnectPool::getInstance();
};
