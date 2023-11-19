//
// Created by MacBook on 08/11/2023.
//

#ifndef QUIZMANAGEMENT_DATABASE_H
#define QUIZMANAGEMENT_DATABASE_H

#include <mysql++.h>
#include <mysql.h>

class Connection;

class Database {
public:
    static mysqlpp::Connection con;

    static void connectToDatabase();
};

#endif //QUIZMANAGEMENT_DATABASE_H
