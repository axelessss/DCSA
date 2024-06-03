#pragma once

#include "User.hpp"
#include "HttpService.h"

namespace HTTPAuth 
{
    bool Basic(HttpRequest *req, UserService& userService);
}