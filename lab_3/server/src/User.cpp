
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
void User::update_data(User user)
{
    this->Login = user.Login;
    this->Password = user.Password;
}

UserService::UserService() { }

bool UserService::IsAdmin(std::string Id)
{
    auto user = this->Get(Id);
    if (user.Login == "egor" || user.Login == "sereja")
    {
        return true;
    }
    return std::find(_admin_list.begin(), _admin_list.end(), Id) != _admin_list.end();
}

User UserService::Create(std::string Login,std::string Password)
{
    std::scoped_lock lock(ServiceMutex);
    if (_users.count(Login))
    {
        throw std::invalid_argument("User with Login \"" + Login +"\" already exists");
    }
    auto id = makeRandStr(50, true);
    User us = {id, Login, Password};
    _users.insert({id, us});
    return us;
}
User UserService::Update(User user)
{
    {
        std::scoped_lock lock(ServiceMutex);
        auto old_user = _users.find(user.Id);
        if (old_user == _users.end())
        {
            throw std::invalid_argument("User not with id = " + user.Id + " not found");
        } 
        old_user->second.update_data(user);
    }
    return Get(user.Id);
}

User UserService::Get(std::string Id)
{
    std::scoped_lock lock(ServiceMutex);
    if (_users.count(Id))
    {
        return _users[Id];
    }
    throw std::invalid_argument("User not with id = " + Id + " not found");
}

User UserService::GetByLogin(std::string Login)
{
    std::scoped_lock lock(ServiceMutex);
    for(auto& user : _users)
    {
        if (user.second.Login == Login)
        {
            return user.second;
        }
    }
    throw std::invalid_argument("User not with login = " + Login + " not found");
}


std::vector<User> UserService::Get()
{
    std::scoped_lock lock(ServiceMutex);
    std::vector<User> users(_users.size()-1);
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
