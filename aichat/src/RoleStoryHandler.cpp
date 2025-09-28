#include "RoleStoryHandler.h"

void RoleStoryHandler::handle(const HttpRequest &req, HttpResponse *resp)
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
      
        std::string roleid = parsed["roleid"];
       

        json successResp;
        successResp["success"] = true;
        successResp["story"] = getStroy(stoi(roleid));   //9.28修改
        std::string successBody = successResp.dump(4);
        resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
        return;
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        printf("[ERROR] 异常捕获rolestory: %s\n", e.what());
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
std::string RoleStoryHandler::getStroy(int roleid)
{
    std::string sql = "SELECT story FROM role_story WHERE roleid =" + to_string(roleid);
    MYSQL_RES *res = dbconn->getConnection()->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row != nullptr)
        {
            return row[0];
        }
    }
    return "";
}