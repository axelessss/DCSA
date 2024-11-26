#include "HTTPAuth.hpp"
#include "base64/Base64.hpp"

bool HTTPAuth::Basic(HttpRequest *req, UserService& userService)
{
    auto basic_auth = req->GetHeader("Authorization");

    if (basic_auth.empty())
    {
        throw std::invalid_argument("empty header");
    }

    auto splited_header = base64::Split(basic_auth, " ");

    if (splited_header.size() != 2)
        throw std::invalid_argument("Wrong header size");
    if (splited_header.front() != "Basic")
        throw std::invalid_argument("Auth is not 'Basic' type");

    auto decode = base64::Decode(splited_header.back());
    auto splited_auth = base64::Split(decode, ":");

    if (splited_auth.size() != 2)
        throw std::invalid_argument("Decoding error");

    auto header_login = splited_auth[0];
    auto header_password = splited_auth[1];

    std::cout << header_login;
    std::cout << header_password;

    auto id = req->GetParam("Id");
    User header_user;

    if (userService.GetByLogin(header_login, header_user))
    {
        if ((header_password == header_user.Password && id == header_user.Id) || header_user.isAdmin)
        {
            return true;
        }
    }

    return false;
}
