//
// Created by MacBook on 07/11/2023.
//
#include <iostream>
#include <map>
#include <string>
#include "QuizManagement.h"
#include "UserManagement.h"

using namespace std;

class MenuAdmin {
    QuizManagement qz;
    UserManagement um;
public:
    void run() {
        while (true) {
            cout << "\n===============MENU ADMIN===============" << endl;
            cout << "====        1. Quản lý đề             ====" << endl;
            cout << "====        2. Quản lý người dùng     ====" << endl;
            cout << "====        0. Thoát                  ====" << endl;
            cout << "==========================================" << endl;
            cout << "============Chọn chức năng: ";
            int choice;
            cin >> choice;
            if (cin.fail()) {
                cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                switch (choice) {
                    case 1:
//                        QUẢN LÝ ĐỀ
                        qz.run();
                        break;
                    case 2:
//                        QUẢN LÝ NGƯỜI DÙNG
                        um.run();
                        break;
                    case 0:
                        cout << "Bạn đã chọn thoát!" << endl;
                        return;
                    default:
                        cout << "Chọn sai! Vui lòng nhập lại!" << endl;
                }
            }
        }
    }
};
