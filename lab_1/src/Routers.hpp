#ifndef _ROUTERS_HPP_
#define _ROUTERS_HPP_

#include "HttpService.h"

namespace route
{
#define BAD 400
#define OK 200
    void RegisterResources(hv::HttpService &router);
}

#endif