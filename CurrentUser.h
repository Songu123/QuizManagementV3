//
// Created by MacBook on 11/11/2023.
//

#ifndef QUIZMANAGEMENT_CURRENTUSER_H
#define QUIZMANAGEMENT_CURRENTUSER_H
#include <iostream>

class CurrentUser {
public:
    // Hàm để đặt ID người dùng hiện tại
    static void setUserID(int userID) {
        currentUserID = userID;
    }

    // Hàm để lấy ID người dùng hiện tại
    static int getUserID() {
        return currentUserID;
    }

private:
    static int currentUserID;
};



#endif //QUIZMANAGEMENT_CURRENTUSER_H
