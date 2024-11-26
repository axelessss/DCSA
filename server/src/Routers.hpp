#pragma once

#include "HttpService.h"
#include "User.hpp"

namespace route
{
#define BAD 400
#define OK 200
    void RegisterResources(hv::HttpService &router, UserService &database);

    void authenticate(const HttpRequest* req, HttpResponse* resp, UserService &database, bool* isAuth, User* currentUser);

    std::string base64_decode(const std::string& in);
}