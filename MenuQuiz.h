//
// Created by MacBook on 16/11/2023.
//

#ifndef UNTITLED_MENUQUIZ_H
#define UNTITLED_MENUQUIZ_H
#include <iostream>
#include <string>
#include <mysql++.h>
#include <mysql.h>
#include "Database.h"
#include "CurrentUser.h"
#include "MenuResult.h"

#include <iomanip>
#include <ctime>
#include <sstream>

class MenuQuiz {
    MenuResult menuResult;
public:
//    int checkAnswer(int n, char answerUser);
//    void test(int quiz_id);
    int sumPointQuiz(string time_User);
    void startQuiz();
    void getQuizHistory();
    void menuQuiz();
};


#endif //UNTITLED_MENUQUIZ_H
