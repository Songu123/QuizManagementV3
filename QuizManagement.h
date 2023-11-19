#ifndef QUIZMANAGEMENT_H
#define QUIZMANAGEMENT_H

#include <iostream>
#include <string>
#include <mysql++.h>
#include <mysql.h>
#include "Database.h"
#include "CurrentUser.h"
#include "MenuUpdateQuiz.h"
class QuizManagement {
public:
    MenuUpdateQuiz menuUpdateQuiz;
    void listQuizs();
    void insertAnswers(int id);
    void insertQuestions(int id);
    void insertQuiz();
    void deleteQuiz();
    void run();
};

#endif // QUIZMANAGEMENT_H