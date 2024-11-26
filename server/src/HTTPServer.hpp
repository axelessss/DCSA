#pragma once

#include "HttpServer.h"
#include "HttpService.h"

#include "Routers.hpp"
#include <memory>

class HttpServer final
{
public:
    using UPtr = std::unique_ptr<HttpServer>;


    HttpServer(const std::string& dbConnInfo);
    HttpServer(const HttpServer &) = delete;
    HttpServer(HttpServer &&) = delete;
    ~HttpServer();

    void Start(int port);

private:
    std::unique_ptr<hv::HttpServer> _server;
    HttpService _router;
    UserService _database;
};