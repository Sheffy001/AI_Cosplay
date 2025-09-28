
#include "EntryHandler.h"
#include "AiChatServer.h"
#include "RegisterHandler.h"
#include "LogoutHandler.h"
#include "AiChatServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpServer.h"
#include"SaveHandle.h"
#include"LoginHandle.h"
#include"History.h"
#include"AiChatHandler.h"
#include"Userinfo.h"
#include"RoleStoryHandler.h"
#include"RoleinfoHandler.h"
AiChatServer::AiChatServer(int port,
                           const std::string &name,
                           muduo::net::TcpServer::Option option)
    : httpServer_(port, name, false,option)
{
    initialize();
}

void AiChatServer::setThreadNum(int numThreads)
{
    httpServer_.setThreadNum(numThreads);
}

void AiChatServer::start()
{
    httpServer_.start();
}

void AiChatServer::initialize()
{
    // 初始化数据库连接池
    // MysqlUtil::init("tcp://127.0.0.1:3306", "root", "root", "Gomoku", 10);
    // 初始化会话
    initializeSession();
    // 初始化中间件
    initializeMiddleware();
    // 初始化路由
    initializeRouter();
}

void AiChatServer::initializeSession()
{
    // 创建会话存储
    auto sessionStorage = std::make_unique<MemorySessionStorage>();
    // 创建会话管理器
    auto sessionManager = std::make_unique<SessionManager>(std::move(sessionStorage));
    // 设置会话管理器
    setSessionManager(std::move(sessionManager));
}

void AiChatServer::initializeMiddleware()
{
    // 创建中间件
    auto corsMiddleware = std::make_shared<CorsMiddleware>();
    // 添加中间件
    httpServer_.addMiddleware(corsMiddleware);
}

void AiChatServer::initializeRouter()
{
    httpServer_.Get("/", std::make_shared<EntryHandler>(this));
    httpServer_.Get("/entry", std::make_shared<EntryHandler>(this));
    
    httpServer_.Post("/login", std::make_shared<LoginHandler>(this));
    
    httpServer_.Post("/register", std::make_shared<RegisterHandler>(this));
    httpServer_.Post("/userinfo", std::make_shared<UserinfoHandler>(this));
    httpServer_.Post("/user/logout", std::make_shared<LogoutHandler>(this));
    httpServer_.Post("/history", std::make_shared<History>(this));
    httpServer_.Post("/Save", std::make_shared<SaveHandler>(this));
    httpServer_.Get("/AIchat", std::make_shared<AiChatHandler>(this));
    httpServer_.Post("/role", std::make_shared<RoleinfoHandler>(this));
    httpServer_.Post("/story", std::make_shared<RoleStoryHandler>(this));
}
void AiChatServer::packageResp(const std::string &version, HttpResponse::HttpStatusCode statusCode,
                             const std::string &statusMsg, bool close, const std::string &contentType,
                             int contentLen, const std::string &body, HttpResponse *resp)
{
    if (resp == nullptr)
    {
        LOG_ERROR << "Response pointer is null";
        return;
    }

    try
    {
        resp->setVersion(version);
        resp->setStatusCode(statusCode);
        resp->setStatusMessage(statusMsg);
        resp->setCloseConnection(close);
        resp->setContentType(contentType);
        resp->setContentLength(contentLen);
        resp->setBody(body);

        LOG_INFO << "Response packaged successfully";
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Error in packageResp: " << e.what();
        // 设置一个基本的错误响应
        resp->setStatusCode(HttpResponse::k500InternalServerError);
        resp->setStatusMessage("Internal Server Error");
        resp->setCloseConnection(true);
    }
}




