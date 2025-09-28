#pragma once

#include <atomic>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <mutex>

#include "HttpServer.h"
#include "CommonConnectionPool.h"
#include "JsonUtil.h"
#include "History.h"

class LoginHandler;
class EntryHandler;
class RegisterHandler;
class LogoutHandler;
class History;
class SaveHandle;
class AiChatHandler;
class UserInfo;
class RoleStoryHandler;
class RoleinfoHandler;

#define DURING_GAME 1
#define GAME_OVER 2

#define MAX_AIBOT_NUM 4096

class AiChatServer
{
public:
    AiChatServer(int port,
                 const std::string &name,
                 muduo::net::TcpServer::Option option = muduo::net::TcpServer::kNoReusePort);

    void setThreadNum(int numThreads);
    void start();

private:
    void initialize();
    void initializeSession();
    void initializeRouter();
    void initializeMiddleware();

    void setSessionManager(std::unique_ptr<SessionManager> manager)
    {
        httpServer_.setSessionManager(std::move(manager));
    }

    SessionManager *getSessionManager() const
    {
        return httpServer_.getSessionManager();
    }
    void packageResp(const std::string &version, HttpResponse::HttpStatusCode statusCode,
                             const std::string &statusMsg, bool close, const std::string &contentType,
                             int contentLen, const std::string &body, HttpResponse *resp);

private:
    friend class EntryHandler;
    friend class LoginHandler;
    friend class RegisterHandler;
    friend class LogoutHandler;
    friend class History;
    friend class SaveHandler;
    friend class AiChatHandler;
    friend class UserinfoHandler;
    friend class RoleStoryHandler;
    friend class RoleinfoHandler;

private:
    HttpServer httpServer_;
    ConnectPool *dbconn = ConnectPool::getInstance();
  
};