//
// Created by reedmark on 2022/12/24.
//

#include "mysql.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool() {
    this->CurConn = 0;
    this->FreeConn = 0;
}

connection_pool* connection_pool::GetInstance() {
    static connection_pool connPool;
    return &connPool;
}

void connection_pool::init(std::string url, std::string User, std::string PassWord, std::string DataBaseName, unsigned int Port,
                           unsigned int MaxConn) {
    this->url = url;
    this->Port = Port;
    this->User = User;
    this->PassWord = PassWord;
    this->DatabaseName = DataBaseName;

    lock.lock();
    for (int i = 0; i < MaxConn; ++i) {
        MYSQL* con = NULL;
        con = mysql_init(con);
        if (con == NULL) {
            cout << "Error:" << mysql_error(con);
            exit(1);
        }
        con = mysql_real_connect(con, url.c_str(), User.c_str(),
                                PassWord.c_str(), DataBaseName.c_str(), Port, NULL, 0);

        if (con == NULL) {
            cout << "Error: " << mysql_error(con);
            exit(1);
        }
        connList.push_back(con);
        ++FreeConn;
    }
    /* 此时信号量reserve就是所有空闲连接，后面每来一个
     * wait，就减去1，直到变成0 */
    reserve = sem(FreeConn);
    this->MaxConn = FreeConn;
    lock.unlock();
}

MYSQL* connection_pool::GetConnection() {
    MYSQL* con = NULL;
    if (connList.size() == 0) {
        return NULL;
    }
    reserve.wait();
    lock.lock();
    con = connList.front();
    connList.pop_front();
    --FreeConn;
    ++CurConn;
    lock.unlock();
    return con;
}

bool connection_pool::ReleaseConnection(MYSQL *conn) {
    if (conn == NULL) {
        return false;
    }
    lock.lock();
    connList.push_back(conn);
    ++FreeConn;
    --CurConn;
    lock.unlock();
    reserve.post();
    return true;
}

void connection_pool::DestroyPool() {
    lock.lock();
    if (connList.size() > 0) {
        list<MYSQL *>::iterator it;
        for (it == connList.begin(); it != connList.end(); ++it) {
            MYSQL *con = *it;
            mysql_close(con);
        }
        CurConn = 0;
        FreeConn = 0;
        connList.clear();
        lock.unlock();
    }
    lock.unlock();
}

int connection_pool::GetFreeConn() {
    return this->FreeConn;
}

connection_pool::~connection_pool() {
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL **con, connection_pool *connPool) {
    *con = connPool->GetConnection();
    conRAII = *con;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
    poolRAII->ReleaseConnection(conRAII);
}
