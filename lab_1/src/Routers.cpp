#include "Routers.hpp"
#include "User.hpp"

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

        }
        catch(std::invalid_argument& invalid)
        {
            return BAD;
        }
        catch(std::exception& ex)
        {
            return BAD;
        }

        return OK;
    });

    router.GET("/users", [](HttpRequest *req, HttpResponse *resp)
    {
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
        }
        catch(std::invalid_argument& invalid)
        {
            return BAD;
        }
        catch(std::exception& ex)
        {
            return BAD;
        }

        return OK;
    });

    router.POST("/user", [](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;

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
            resp->content_type = APPLICATION_JSON;
        }
        catch(const std::exception& e)
        {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return BAD;
        }
        
        return OK;
    });

    router.Delete("/user/{Id}", [](HttpRequest *req, HttpResponse *resp)
    {
        return UserService::Instance().Remove(req->GetParam("Id")) > 0 ? OK : BAD;
    });
}