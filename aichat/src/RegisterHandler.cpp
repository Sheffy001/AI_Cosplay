#include "RegisterHandler.h"

void RegisterHandler::handle(const HttpRequest& req, HttpResponse* resp)
{
    // 解析body(json格式)
    json parsed = json::parse(req.getBody());
    std::string username = parsed["username"];
    std::string password = parsed["password"];

    // 判断用户是否已经存在，如果存在则注册失败
    int userId = insertUser(username, password);
    if (userId != -1)
    {
        // 插入成功
        // 封装成功响应
        json successResp;   
        successResp["status"] = "success";
        successResp["message"] = "Register successful";
        successResp["userId"] = userId;
        std::string successBody = successResp.dump(4);

        resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(successBody.size());
        resp->setBody(successBody);
    }
    else
    {
        // 插入失败
        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = "username already exists";
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersion(), HttpResponse::k409Conflict, "Conflict");
        resp->setCloseConnection(false);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
    }
}

int RegisterHandler::insertUser(const std::string &username, const std::string &password)
{
    // 判断用户是否存在，如果存在则返回-1，否则返回用户id
    if(UserIsExist(username)==false)return -1;
    char sql[1024] = {0};
    sprintf(sql, "insert into users(username,password,maxchatid) values('%s','%s',%d) ", username.c_str(), password.c_str(), 0);
    return dbconn->getConnection()->update(sql);
}
bool RegisterHandler::UserIsExist(const std::string &username){
    char sql[1024] = {0};
    sprintf(sql, "select * from users where username = %s", username.c_str());

    // std::vector<std::string> params = {username, password};
    MYSQL_RES *res = dbconn->getConnection()->query(sql);

    if (res == nullptr)
    {
        return false;
    }
    
    mysql_free_result(res);
    return true;
}

