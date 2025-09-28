#include "LoginHandle.h"

void LoginHandler::handle(const HttpRequest &req, HttpResponse *resp)
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
        json parsed = json::parse(req.getBody());
        std::string username = parsed["username"];
        std::string password = parsed["password"];
        // 验证用户是否存在
        int userid;
        int maxchatid = queryUserId(username, password,userid);
        
        if (maxchatid != -1)
        {
            
            auto session = server_->getSessionManager()->getSession(req, resp);
            
            session->setValue("userId", std::to_string(userid));
            session->setValue("username", username);
            session->setValue("maxchatid",std::to_string(maxchatid));
            session->setValue("isLoggedIn", "true");

            json successResp;
            successResp["success"] = true;
            successResp["userId"] = std::to_string(userid);
            successResp["username"] = username;
            successResp["maxchatid"] = maxchatid;
            std::string successBody = successResp.dump(4);

            resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
            return;
        }
        else // 账号密码错误，请重新登录
        {
            // 封装json数据
            json failureResp;
            failureResp["status"] = "error";
            failureResp["message"] = "Invalid username or password";
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

int LoginHandler::queryUserId(const string &username, const std::string &password,int&userid)
{
    char sql[1024] = {0};
    sprintf(sql, "select userid,maxchatid from users where username = '%s' && password = '%s'", username.c_str(), password.c_str());

    // std::vector<std::string> params = {username, password};
    MYSQL_RES *res = dbconn->getConnection()->query(sql);

    if (res == nullptr)
    {
        return -1;
    }
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != nullptr)
    {
        userid = atoi(row[0]);
        return atoi(row[1]);
    }
    mysql_free_result(res);
    return -1;
}
