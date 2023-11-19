#ifndef MENUUSER_H
#define MENUUSER_H

#include <iostream>
#include <string>
#include "Database.h"
#include "MenuQuiz.h"
using namespace mysqlpp;
class MenuUser {
    MenuQuiz menuQuiz;
public:
    int logIn();
    void signUp();
    void menuUser();
};

#endif // MENUUSER_H