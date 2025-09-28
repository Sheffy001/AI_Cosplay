#include "SaveHandle.h"

// 改进的转义函数
std::string improvedEscapeString(const std::string &input)
{
    std::string result;
    result.reserve(input.length() * 2); // 预分配空间

    for (char c : input)
    {
        switch (c)
        {
        case '\'':
            result += "\\'";
            break;
        case '\"':
            result += "\\\"";
            break;
        case '\\':
            result += "\\\\";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        case '\0':
            result += "\\0";
            break;
        case '\x1a':
            result += "\\Z";
            break;
        default:
            result += c;
            break;
        }
    }
    return result;
}

void SaveHandler::handle(const HttpRequest &req, HttpResponse *resp)
{

    auto contentType = req.getHeader("Content-Type");
    if (contentType.empty() || contentType != "application/json" || req.getBody().empty())
    {
        resp->setStatusLine(req.getVersion(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
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
     
        std::string user_msg = parsed.at("user_message").get<std::string>();
        std::string bot_msg = parsed.at("bot_message").get<std::string>();

        // 使用改进的转义函数
        std::string user_msg_escaped = improvedEscapeString(user_msg);
        std::string bot_msg_escaped = improvedEscapeString(bot_msg);

        auto conn = dbconn->getInstance()->getConnection();

        // 分别插入，便于调试
        try
        {
            std::string sql_user = "INSERT INTO messages(userid, chatid, role, message) VALUES(" +
                                   std::to_string(std::stoi(userid)) + ", " +
                                   std::to_string(chatid) +
                                   ", 'user', '" + user_msg_escaped + "')";

            conn->update(sql_user);
        }
        catch (const std::exception &e)
        {
            printf("[ERROR] 用户消息插入失败: %s\n", e.what());
            throw;
        }

        try
        {
            std::string sql_bot = "INSERT INTO messages(userid, chatid, role, message) VALUES(" +
                                  std::to_string(std::stoi(userid)) + ", " +
                                  std::to_string(chatid) +
                                  ", 'assistant', '" + bot_msg_escaped + "')";

            conn->update(sql_bot);
        }
        catch (const std::exception &e)
        {
            printf("[ERROR] 机器人消息插入失败: %s\n", e.what());
            throw;
        }

        // 构造成功响应
        json successResp;
        successResp["success"] = true;
        successResp["userid"] = userid;
        successResp["chatid"] = chatid;

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
        printf("[ERROR] 异常捕获save: %s\n", e.what());

        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = std::string("服务器错误: ") + e.what();
        std::string failureBody = failureResp.dump(4);

        resp->setStatusLine(req.getVersion(), HttpResponse::k500InternalServerError, "Internal Server Error");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
        return;
    }
}
