//
// Created by reedmark on 2022/12/24.
//

#ifndef MYWEBSERVER_SQL_CONNECTION_POOL_H
#define MYWEBSERVER_SQL_CONNECTION_POOL_H

#include <stdio.h>
#include <list>
#include <mysql.h>
#include <error.h>
#include <string>
#include <iostream>
#include <string>
#include "../locker/locker.h"

using namespace std;

class connection_pool {
public:
    MYSQL* GetConnection();
    bool ReleaseConnection(MYSQL* conn);
    int GetFreeConn();
    void DestroyPool();

    static connection_pool* GetInstance();

    void init(string url, string User, string PassWord,
              string DataBaseName, unsigned int Port, unsigned int MaxConn);

    connection_pool();
    ~connection_pool();

private:
    unsigned int MaxConn;   // 最大连接数
    unsigned int CurConn;   // 当前已经使用的连接数
    unsigned int FreeConn;  // 当前空闲的连接数

private:
    locker lock;
    list<MYSQL*> connList;  /* 连接池 */
    sem reserve;

private:
    string url;     /* 主机地址 */
    unsigned int  Port;    /* 数据库端口号 */
    string User;    /* 用户名*/
    string PassWord;    /* 密码 */
    string DatabaseName;    /* 数据库名 */
};

class connectionRAII {
public:
    connectionRAII(MYSQL** con, connection_pool* connPool);
    ~connectionRAII();

private:
    MYSQL* conRAII;
    connection_pool* poolRAII;
};




#endif //MYWEBSERVER_SQL_CONNECTION_POOL_H
