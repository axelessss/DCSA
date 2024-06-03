#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>

struct User final
{
    std::string Id;
    std::string Login;
    std::string Password;
};

class UserService
{
private:
    std::unordered_map<std::string, User> _users;
    std::mutex ServiceMutex;
    UserService();

public:
    static UserService& Instance() 
    {
        static UserService instance;
        return instance;
    }
    
    User Create(std::string Login,std::string Password);
    User Get(std::string Id);

    std::vector<User> Get();

    bool Remove(std::string Id); 
    

    bool Remove(User User)
    {
        return Remove(User.Id);
    }
};
