#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <pqxx/pqxx>
#include "Base64.hpp"

struct User final
{
    std::string Id;
    std::string Login;
    std::string Password;
    bool isAdmin;
    void update_data(User user);
};

class UserService
{
private:
    std::mutex ServiceMutex;
    pqxx::connection _conn;

public:
    UserService(const std::string& connInfo);

    std::string Create(std::string Login, std::string Password, bool isAdmin);
    bool Update(const User& user);
    bool Get(std::string Id, User& user);
    bool GetByLogin(std::string Login, User& user);
    std::vector<User> Get();
    bool Remove(std::string Id);
    bool Remove(User User)
    {
        return Remove(User.Id);
    }
};
