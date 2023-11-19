//
// Created by MacBook on 13/11/2023.
//

#ifndef QUIZMANAGEMENT_MENUUPDATEQUIZ_H
#define QUIZMANAGEMENT_MENUUPDATEQUIZ_H
#include <iostream>
#include <string>
#include <mysql++.h>
#include <mysql.h>
#include "Database.h"
#include "CurrentUser.h"
#include "MenuUpdateQuestion.h"

class MenuUpdateQuiz {
    MenuUpdateQuestion mu;
public:
    void updateQuiz();
    void menuUpdateQuestion(int quiz_id);
    void menuUpdateQuiz();
};


#endif //QUIZMANAGEMENT_MENUUPDATEQUIZ_H
