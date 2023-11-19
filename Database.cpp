#include <iostream>
#include <string>
#include <mysql++.h>
#include <mysql.h>
#include "Database.h"

#define dbname "quanlyquiz"
#define server "127.0.0.1:3307"
#define user "root"
#define pass "Songuk21@"

using namespace std;
using namespace mysqlpp;

mysqlpp::Connection Database::con;

void Database::connectToDatabase() {
    try {
        con.connect(dbname, server, user, pass);
        cout << "Connected to database\n";
    } catch (mysqlpp::Exception &e) {
        cout << e.what() << endl;
    }
}