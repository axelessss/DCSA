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

    void update_data(User user);
};

class UserService
{
private:
    std::unordered_map<std::string, User> _users;
    std::vector<std::string> _admin_list;
    std::mutex ServiceMutex;
    UserService();

public:
    static UserService& Instance() 
    {
        static UserService instance;
        return instance;
    }
    
    User Create(std::string Login,std::string Password);
    User Update(User user);
    User Get(std::string Id);
    User GetByLogin(std::string Login);
    std::vector<User> Get();
    bool Remove(std::string Id); 
    bool Remove(User User) 
    {
        return Remove(User.Id);
    }


    bool IsAdmin(std::string Id);
    bool IsAdmin(User User)  
    {
        return IsAdmin(User.Id);
    }
};
