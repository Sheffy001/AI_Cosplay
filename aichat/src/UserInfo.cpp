#include "Userinfo.h"

void UserinfoHandler::handle(const HttpRequest &req, HttpResponse *resp)
{
    auto contentType = req.getHeader("Content-Type");
    if (contentType.empty() || contentType != "application/json" || req.getBody().empty())
    {
        LOG_INFO << "content" << req.getBody();
        resp->setStatusLine(req.getVersion(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }
    // JSON 解析使用 try catch 捕获异常
    try
    {

        auto session = server_->getSessionManager()->getSession(req, resp);
        if (!session->isExpired())
        {
            json successResp;
            successResp["success"] = true;
            successResp["userId"] = session->getValue("userId");
            successResp["username"] = session->getValue("username");
            successResp["maxchatid"] = session->getValue("maxchatid");
            std::string successBody = successResp.dump(4);

            resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
            return;
        }
        else{
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Invalid Session";
            std::string failureBody = failureResp.dump(4);

            resp->setStatusLine(req.getVersion(), HttpResponse::k401Unauthorized, "Unauthorized");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(failureBody.size());
            resp->setBody(failureBody);
            return;
        }
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        printf("[ERROR] 异常捕获: %s\n", e.what());
        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersion(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}