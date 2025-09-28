#include "History.h"

void History::handle(const HttpRequest &req, HttpResponse *resp)
{
    auto contentType = req.getHeader("Content-Type");
    if (contentType.empty() || contentType != "application/json" || req.getBody().empty())
    {
        resp->setStatusLine(req.getVersion(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        cout << "notif" << endl;
        resp->setContentType("application/json");
        resp->setContentLength(0);
        resp->setBody("");
        return;
    }

    try
    {
        
        json parsed = json::parse(req.getBody());
   
        std::string userid;
        if (parsed.at("userid").is_number())
        {
            userid = std::to_string(parsed.at("userid").get<int>());
        }
        else if (parsed.at("userid").is_string())
        {
            userid = parsed.at("userid").get<std::string>();
        }
        else
        {
            throw std::runtime_error("userid 类型错误");
        }
  
        int chatid;
        if (parsed.at("chatid").is_number())
        {
            chatid = parsed.at("chatid").get<int>();
        }
        else if (parsed.at("chatid").is_string())
        {
            chatid = std::stoi(parsed.at("chatid").get<std::string>());
        }
        else
        {
            throw std::runtime_error("chatid 类型错误");
        }
        
       
        std::vector<json> history_msg;
        getMsg(userid, chatid, history_msg);
        
        json successResp;
        successResp["success"] = true;
        successResp["userid"] = userid;

        successResp["chatid"] = chatid;
        successResp["messages"] = history_msg; // 把消息放到返回体
        
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
        json failureResp;
        printf("[ERROR] 异常捕获history: %s\n", e.what());
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
void History::getMsg(const string &userid, const int chatid, std::vector<json> &v)
{
    char sql[1024] = {};
    // userid 如果是数字 OK，如果是字符串需要加单引号
    sprintf(sql,
            "SELECT role, message, time FROM ("
            " SELECT role, message, time FROM messages "
            " WHERE userid = %s AND chatid = %d "
            " ORDER BY time DESC LIMIT 20"
            ") AS sub ORDER BY time ASC;",
            userid.c_str(), chatid);

    MYSQL_RES *res = dbconn->getInstance()->getConnection()->query(sql);
    if (res != nullptr)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)))
        {
            json j;
            j["role"] = row[0] ? row[0] : "";
            j["message"] = row[1] ? row[1] : "";
            j["time"] = row[2] ? row[2] : "";
            v.push_back(j);
        }
        mysql_free_result(res);
    }
}