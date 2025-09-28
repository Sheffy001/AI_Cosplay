#include "Connection.h"

Connection::Connection()
{
    _conn = mysql_init(nullptr);
    mysql_set_character_set(_conn, "utf8mb4");
}
Connection::~Connection()
{
    if (_conn != nullptr)
    {
        mysql_close(_conn);
    }
}
#include<iostream>
bool Connection::connect(std::string ip,
                         unsigned short port,
                         std::string user,
                         std::string password,
                         std::string dbname)
{
    MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
    if (p != nullptr)
        mysql_query(_conn, "SET NAMES utf8mb4");
    return p != nullptr;
}

bool Connection::update(std::string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_ERROR << "更新失败: " << sql
                  << " | 错误码: " << mysql_errno(_conn)
                  << " | 错误信息: " << mysql_error(_conn);
        return false;
    }
    return true;
}

MYSQL_RES* Connection::query(std::string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_ERROR << "查询失败: " << sql
                  << " | 错误码: " << mysql_errno(_conn)
                  << " | 错误信息: " << mysql_error(_conn);
        return nullptr;
    }
    return mysql_store_result(_conn);
}
