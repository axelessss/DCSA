#include "Routers.hpp"
#include "User.hpp"
#include "base64/Base64.hpp"
#include "HTTPAuth.hpp"

void route::RegisterResources(hv::HttpService &router, UserService &database)
{
    router.GET("/user/{Id}", [&database](HttpRequest *req, HttpResponse *resp)
    {
        try
        {
            User user;
            if (database.Get(req->GetParam("Id"), user))
            {
                nlohmann::json response
                {
                    {"Login", user.Login},
                    {"Id", user.Id},
                    {"isAdmin", user.isAdmin}
                };

                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                resp->status_code = HTTP_STATUS_OK;
            }
            else
            {
                nlohmann::json response
                {
                    {"Error", "User not found"}
                };

                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
                resp->status_code = HTTP_STATUS_BAD_REQUEST;
            }
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

    router.GET("/users", [&database](HttpRequest *req, HttpResponse *resp)
    {
        try
        {
            auto users = database.Get();

            nlohmann::json responseArray;

            for (const auto &user : users)
            {
                nlohmann::json userJson
                {
                    {"Login", user.Login},
                    {"Id", user.Id},
                    {"isAdmin", user.isAdmin}
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

    router.POST("/user", [&database](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json request;
        nlohmann::json response;
        resp->status_code = HTTP_STATUS_BAD_REQUEST;
        resp->content_type = APPLICATION_JSON;

        try
        {
            request = nlohmann::json::parse(req->body);
            std::string id = database.Create(request["Login"], request["Password"], request["isAdmin"]);
            if (id != "null")
            {
                response =
                {
                    {"Login", request["Login"]},
                    {"Id", id},
                    {"isAdmin", request["isAdmin"]}
                };
                resp->status_code = HTTP_STATUS_OK;
            }

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

    router.Delete("/user/{Id}", [&database](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        resp->content_type = APPLICATION_JSON;
        resp->status_code = HTTP_STATUS_BAD_REQUEST;

        try
        {
            if (HTTPAuth::Basic(req, database) && database.Remove(req->GetParam("Id")))
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

    router.PATCH("/user/{Id}", [&database](HttpRequest *req, HttpResponse *resp)
    {
        nlohmann::json response;
        resp->content_type = APPLICATION_JSON;
        resp->status_code = HTTP_STATUS_BAD_REQUEST;

        try
        {
            if (HTTPAuth::Basic(req, database))
            {
                nlohmann::json request = nlohmann::json::parse(req->body);
                User user = {req->GetParam("Id"), request["Login"], request["Password"]};
                database.Update(user);
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
