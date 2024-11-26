#include "User.hpp"
#include <random>
#include <stdexcept>
#include <algorithm>

// Генерация случайной строки
std::string UserService::makeRandStr(int sz, bool printable)
{
    static constexpr char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string ret;
    ret.resize(sz);
    std::mt19937 rng(std::random_device{}());
    for (int i = 0; i < sz; ++i)
    {
        if (printable)
        {
            uint32_t x = rng() % (sizeof(CCH) - 1);
            ret[i] = CCH[x];
        }
        else
        {
            ret[i] = rng() % 0xFF;
        }
    }
    return ret;
}

// Конструктор UserService
UserService::UserService(const std::string& connInfo) : _conn(connInfo) {}

// Создание пользователя
std::string UserService::Create(const std::string Login, const std::string Password, bool isAdmin)
{
    std::scoped_lock lock(ServiceMutex);

    try
    {
        pqxx::work txn(_conn);

        // Проверяем, существует ли пользователь с таким логином
        pqxx::result res = txn.exec_params("SELECT id FROM users WHERE login = $1", Login);
        if (res.empty())
        {
            // Генерируем уникальный ID пользователя
            std::string id = makeRandStr(50, true);
            std::string password = base64::Encode(Password);
            // Добавляем пользователя в базу данных
            txn.exec_params("INSERT INTO users (id, login, password, isadmin) VALUES ($1, $2, $3, $4)",
                            id, Login, password, isAdmin);
            txn.commit();
            return id;
        }
        return "null";
    } catch (const std::exception& e)
    {
        return "null";
    }
}

// Обновление данных пользователя
bool UserService::Update(const User& user)
{
    std::scoped_lock lock(ServiceMutex);

    try
    {
        pqxx::work txn(_conn);

        // Проверяем, существует ли пользователь с таким ID
        pqxx::result res = txn.exec_params("SELECT id FROM users WHERE id = $1", user.Id);
        if (!res.empty())
        {
            std::string password = base64::Encode(user.Password);
            txn.exec_params("UPDATE users SET login = $1, password = $2, isAdmin = $3 WHERE id = $4",
                             user.Login, password, user.isAdmin, user.Id);
            txn.commit();
            return true;
        }
        return false;
        // Обновляем данные
    } catch (const std::exception& e)
    {
        return false;
    }
}

// Получение пользователя по ID
bool UserService::Get(const std::string Id, User& user)
{
    std::scoped_lock lock(ServiceMutex);

    try
    {
        pqxx::work txn(_conn);
        pqxx::result res = txn.exec_params("SELECT id, login, isAdmin FROM users WHERE id = $1", Id);

        if (!res.empty())
        {
            user.Id = res["id"].as<std::string>();
            user.Login = res["login"].as<std::string>();
            user.isAdmin = res["isAdmin"].as<bool>();
            return true;
        }
        return false;
    } catch (const std::exception& e)
    {
        return false;
    }
}

// Получение пользователя по логину
bool UserService::GetByLogin(const std::string Login, User& user)
{
    std::scoped_lock lock(ServiceMutex);

    try
    {
        pqxx::work txn(_conn);

        pqxx::result res = txn.exec_params("SELECT id, login, isAdmin FROM users WHERE login = $1", Login);
        if (res.empty())
        {
            return false;
        }
        user.Id = res["id"].as<std::string>();
        user.Login = res["login"].as<std::string>();
        user.isAdmin = res["isAdmin"].as<bool>();
    } catch (const std::exception& e)
    {
        return false;
    }
    return true;
}

// Получение всех пользователей
std::vector<User> UserService::Get()
{
    std::scoped_lock lock(ServiceMutex);

    std::vector<User> users;
    try
    {
        pqxx::work txn(_conn);

        pqxx::result res = txn.exec("SELECT id, login, isAdmin FROM users");

        for (const auto& row : res)
        {
            User user;
            user.Id = row["id"].as<std::string>();
            user.Login = row["login"].as<std::string>();
            user.isAdmin = row["isAdmin"].as<bool>();
            users.push_back(user);
        }
    } catch (const std::exception& e) { }
    return users;
}

// Удаление пользователя
bool UserService::Remove(const std::string Id)
{
    std::scoped_lock lock(ServiceMutex);

    pqxx::work txn(_conn);

    pqxx::result res = txn.exec_params("DELETE FROM users WHERE id = $1 RETURNING id", Id);
    txn.commit();

    return !res.empty();
}
