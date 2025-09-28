
#include "AiChatServer.h"
#include "RouterHandler.h"
#include "CommonConnectionPool.h"
#include "JsonUtil.h"
#include "RoleinfoHandler.h"

void RoleinfoHandler::handle(const HttpRequest &req, HttpResponse *resp)
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
        std::string type = parsed["type"];
       
        if (type == "single")
        {
            
            std::string roleid = parsed["roleid"];
            json successResp;
            successResp["success"] = true;
            std::string opensay = getInfo(stoi(roleid));
            successResp["roleinfo"] = opensay;
            std::string successBody = successResp.dump(4);

            resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
            return;
        }
        
        if (type == "mult")
        {
            
            vector<json> info;
            getInfo(info);
            
            json successResp;
           
            successResp["success"] = true;
            successResp["roleinfo"] = info;
            std::string successBody = successResp.dump(4);
      

            resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
            resp->setCloseConnection(false);
            resp->setContentType("application/json");
            resp->setContentLength(successBody.size());
            resp->setBody(successBody);
        
            return;
        }
    }
    catch (const std::exception &e)
    {
        // 捕获异常，返回错误信息
        printf("[ERROR] 异常捕获roleinfo: %s\n", e.what());
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
void RoleinfoHandler::getInfo(vector<json> &info)
{
    std::string sql = "SELECT roleid,rolename,voicetype FROM role_story";
    MYSQL_RES *res = dbconn->getConnection()->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))!=nullptr)
        {
            json j;
            j["roleid"] = row[0] ? row[0] : "";
            j["rolename"] = row[1] ? row[1] : "";
            j["voicetype"] = row[2] ? row[2] : "qiniu_zh_female_tmjxxy";
            info.push_back(j);
        }
        mysql_free_result(res);
    }
}
std::string RoleinfoHandler::getInfo(int roleid)
{
    std::string sql = "SELECT opensay FROM role_story WHERE roleid = " + std::to_string(roleid);
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
