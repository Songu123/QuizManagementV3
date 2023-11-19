cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(DoAn main.cpp
        Database.cpp
        Database.h
        MenuUser.cpp
        MenuUser.h
        CurrentUser.cpp
        CurrentUser.h
        QuizManagement.cpp
        QuizManagement.h
        UserManagement.cpp
        UserManagement.h
        MenuUpdateQuestion.h
        MenuUpdateQuestion.cpp
        MenuUpdateQuiz.cpp
        MenuQuiz.cpp
        MenuQuiz.h
        MenuResult.cpp
        MenuResult.h
)
include_directories(/usr/local/include/mysql++)
include_directories(/usr/local/include/mysql)

find_library(MYSQLPP_LIBRARY mysqlpp)
find_library(MYSQLCLIENT_LIBRARY mysqlclient)
target_link_libraries(DoAn ${MYSQLPP_LIBRARY} ${MYSQLCLIENT_LIBRARY})