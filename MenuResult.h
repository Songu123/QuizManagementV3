//
// Created by MacBook on 18/11/2023.
//

#ifndef UNTITLED_MENURESULT_H
#define UNTITLED_MENURESULT_H
#include <iostream>
#include <string>
#include <mysql++.h>
#include <mysql.h>
#include "Database.h"
#include "CurrentUser.h"

#include <iomanip>
#include <ctime>
#include <sstream>
using namespace std;
class MenuResult {
public:
    void resultDetail(string timeUser);
    void resultUser(string timeUser);
    void rankQuiz(int quiz_id);
    void menuResultUser(int quizID, string timeUser);
};


#endif //UNTITLED_MENURESULT_H
