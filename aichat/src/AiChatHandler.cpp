#include"AiChatHandler.h"

void AiChatHandler::handle(const HttpRequest& req, HttpResponse* resp){
   
   try {
      
        auto session = server_->getSessionManager()->getSession(req, resp);
        if (session->getValue("isLoggedIn") != "true") {
            json errorResp;
            errorResp["status"] = "error";
            errorResp["message"] = "Unauthorized";
            std::string errorBody = errorResp.dump(4);

            server_->packageResp(req.getVersion(), HttpResponse::k401Unauthorized,
                                 "Unauthorized", true, "application/json", errorBody.size(),
                                 errorBody, resp);
            return;
        }

 
        int userId = std::stoi(session->getValue("userId"));
        std::string username = session->getValue("username");
        int maxchatid = std::stoi(session->getValue("maxchatid"));

        std::string reqFile("/home/sheffy/CppProject/AIChat/source/AIchat.html");
        FileUtil fileOperater(reqFile);
        if (!fileOperater.isValid()) {
            LOG_WARN << reqFile << " not exist.";
            fileOperater.resetDefaultFile();
        }

        std::vector<char> buffer(fileOperater.size());
        fileOperater.readFile(buffer);
        std::string htmlContent(buffer.data(), buffer.size());
        resp->setStatusLine(req.getVersion(), HttpResponse::k200Ok, "OK");
        resp->setCloseConnection(false);
        resp->setContentType("text/html");
        resp->setContentLength(htmlContent.size());
        resp->setBody(htmlContent);
    } catch (const std::exception &e) {
        json failureResp;
        failureResp["status"] = "error";
        failureResp["message"] = e.what();
        std::string failureBody = failureResp.dump(4);
        resp->setStatusLine(req.getVersion(), HttpResponse::k400BadRequest, "Bad Request");
        resp->setCloseConnection(true);
        resp->setContentType("application/json");
        resp->setContentLength(failureBody.size());
        resp->setBody(failureBody);
    }
}
