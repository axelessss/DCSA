#include "HTTPAuth.hpp"
#include "Routers.hpp"
#include "User.hpp"
#include "base64/Base64.hpp"

void route::RegisterResources(hv::HttpService &router)
{
    router.GET("/user/{Id}", [](HttpRequest *req, HttpResponse *resp)
    {
        try
        {
            auto user = UserService::Instance().Get(req->GetParam("Id"));
            nlohmann::json response
            {
                {"Login", user.Login},
                {"Id", user.Id}
            };

            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            resp->status_code = HTTP_STATUS_OK;

        }
        catch(const std::invalid_argument& invalid)
        {
            resp->status_code = HTTP_STATUS_BAD_REQUEST;
        }
        catch(const std::exception& ex)
        {
            resp->status_code = HTTP_STATUS_BAD_REQUEST;
        }

        return resp->status_code;
    });

    router.GET("/users", [](HttpRequest *req, HttpResponse *resp)
    {
        std::cout << "2eqw";
        try
        {
            auto users = UserService::Instance().Get();

            nlohmann::json responseArray;

            for(const auto& user : users) 
            {
                nlohmann::json userJson
                {
                    {"Login", user.Login},
                    {"Id", user.Id}
                };
                responseArray.push_back(userJson);
            }
            resp->SetBody(responseArray.dump());
            resp->content_type = APPLICATION_JSON;
            resp->status_code = HTTP_STATUS_OK;
        }
        catch(const std::invalid_argument& invalid)
        {
            resp->status_code = HTTP_STATUS_BAD_REQUEST;
        }
        catch(const std::exception& ex)
        {
            resp->status_code = HTTP_STATUS_BAD_REQUEST;
        }

        return resp->status_code;
    });

    router.POST("/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;
        resp->status_code = HTTP_STATUS_BAD_REQUEST;
        resp->content_type = APPLICATION_JSON;
        try
        {
            request = nlohmann::json::parse(req->body);
            auto user = UserService::Instance().Create(request["Login"], request["Password"]);
            response =
            {
                {"Login", user.Login},
                {"Id", user.Id}
            };

            resp->SetBody(response.dump());
            resp->status_code = HTTP_STATUS_OK;

        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
        }
        return resp->status_code;
    });

    router.Delete("/user/{Id}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        resp->content_type = APPLICATION_JSON;
        resp->status_code = HTTP_STATUS_BAD_REQUEST;

        try
        {
            if (HTTPAuth::Basic(req,UserService::Instance()) && UserService::Instance().Remove(req->GetParam("Id")))
            {
                resp->status_code = HTTP_STATUS_OK;
                response["Id"] = req->GetParam("Id");
                response["Success"] = "true";
            }
        }
        catch (const std::invalid_argument& ex)
        {
            response["Error"] = ex.what();
            response["Success"] = "false";
        }
        catch(const std::exception& ex)
        {
            response["Error"] = ex.what();
            response["Success"] = "false";
        } 

        resp->SetBody(response.dump());
        return resp->status_code;
    });

    router.PATCH("/user/{Id}", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        resp->content_type = APPLICATION_JSON;
        resp->status_code = HTTP_STATUS_BAD_REQUEST;
        
        try
        {
            if (HTTPAuth::Basic(req,UserService::Instance()))
            {
                nlohmann::json request = nlohmann::json::parse(req->body);
                User user = {req->GetParam("Id"), request["Login"], request["Password"]};
                UserService::Instance().Update(user);
                response =
                {
                    {"Login", user.Login},
                    {"Id", user.Id}
                };
                resp->SetBody(response.dump());
                resp->status_code = HTTP_STATUS_OK;
            }
        }
        catch (const std::invalid_argument& ex)
        {
            response["Error"] = ex.what();
        }
        catch(const std::exception& ex)
        {
            response["Error"] = ex.what();
        } 
        return resp->status_code;
    });
}
