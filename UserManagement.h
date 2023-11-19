//
// Created by MacBook on 16/11/2023.
//

#ifndef UNTITLED_USERMANAGEMENT_H
#define UNTITLED_USERMANAGEMENT_H

#include <iostream>
#include <string>

#include "Database.h"
#include <mysql++.h>

class UserManagement {
public:
    void listUsers();
    void createUser();
    void updateUser();
    void listResultUser();
    void run();
};


#endif //UNTITLED_USERMANAGEMENT_H
