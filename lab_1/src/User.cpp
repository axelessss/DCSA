
#include "User.hpp"
#include <string>
#include <random>
#include <vector>

std::string makeRandStr(int sz, bool printable) {
    static constexpr char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string ret;
    ret.resize(sz);
    std::mt19937 rng(std::random_device{}());
    for (int i = 0; i < sz; ++i) {
        if (printable) {
            uint32_t x = rng() % (sizeof(CCH) - 1);
            ret[i] = CCH[x];
        } else {
            ret[i] = rng() % 0xFF;
        }
    }
    return ret;
}

UserService::UserService() { }

User UserService::Create(std::string Login,std::string Password)
{
    std::scoped_lock lock(ServiceMutex);
    if (_users.count(Login))
        throw std::invalid_argument("User with Login \"" + Login +"\" already exists");
    auto id = makeRandStr(50, true);
    User us = {id, Login, Password};
    _users.insert({id, us});
    return us;
}

User UserService::Get(std::string Id)
{
    std::scoped_lock lock(ServiceMutex);
    if (_users.count(Id))
        return _users[Id];

    throw std::invalid_argument("User not with id = " + Id + " not found");
}

std::vector<User> UserService::Get()
{
    std::scoped_lock lock(ServiceMutex);
    std::vector<User> users(_users.size());
    for (const auto& user: _users)
    {
        users.push_back(user.second);
    }
    return users;
}

bool UserService::Remove(std::string Id)
{
    std::scoped_lock lock(ServiceMutex);
    return _users.erase(Id); 
}
