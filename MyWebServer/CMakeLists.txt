cmake_minimum_required(VERSION 3.23)
project(server)
set(CMAKE_CXX_STANDARD 14)
#声明头文件路径
set(INC_DIR /usr/include/mariadb)
#set(INC_DIR /usr/include/mysql)
#引入头文件
include_directories(${INC_DIR})
#声明链接库路径
set(LINK_DIR /usr/lib/aarch64-linux-gnu/)
#引入库文件
link_directories(${LINK_DIR})
add_executable(server main.cpp ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./locker/locker.h ./log/log.cpp ./log/log.h ./log/block_queue.h ./CGImysql/sql_connection_pool.cpp ./CGImysql/sql_connection_pool.h)
target_link_libraries(server -lpthread -lmariadb)


#cmake_minimum_required(VERSION 3.23)
#project(server)
#set(CMAKE_CXX_STANDARD 14)
##声明头文件路径
#set(INC_DIR /usr/include/mysql)
##引入头文件
#include_directories(${INC_DIR})
##声明链接库路径
#set(LINK_DIR /usr/lib/x86_64-linux-gnu/)
##引入库文件
#link_directories(${LINK_DIR})
#add_executable(server main.cpp ./threadpool/threadpool.h ./http/http_conn.cpp ./http/http_conn.h ./locker/locker.h ./log/log.cpp ./log/log.h ./log/block_queue.h ./CGImysql/sql_connection_pool.cpp ./CGImysql/sql_connection_pool.h)
#target_link_libraries(server -lpthread -lmariadbclient)


add_executable(sql_test sql_test.cpp)
target_link_libraries(sql_test -lpthread -lmariadb)
