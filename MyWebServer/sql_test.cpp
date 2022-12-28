//
// Created by reedmark on 2022/12/26.
//
#include <mysql.h>
#include <iostream>

using namespace std;

int main() {
    MYSQL *con = NULL;
    con = mysql_init(con);
    if (con == NULL) {
        cout << "Init error" << endl;
    }
    con = mysql_real_connect(con, "1.117.149.54", "root", "root", "webserver", 3306, NULL, 0);
    if (con == NULL) {
        cout << "Real connect error" << mysql_error(con)  << endl;
    } else {
        cout << "Success" << endl;
    }
    return 0;
}
