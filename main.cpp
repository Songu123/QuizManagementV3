#include <iostream>
#include <string>
#include <mysql++.h>
#include <mysql.h>
//#include "Database.cpp"
#include "Database.h"
#include "MenuUser.h"
#include "MenuAdmin.cpp"
#include <format>
#include "CurrentUser.h"

using namespace std;
using namespace mysqlpp;

int logIn();
// Khởi tạo biến static trong lớp
//int CurrentUser::currentUserID = -1; // Giá trị mặc định khi chưa có ID nào được đặt

int main() {
    MenuAdmin menuAdmin;
    MenuUser menuUser;
    try {
        Database::connectToDatabase();
        Query q = Database::con.query("SELECT * FROM quizs");
        StoreQueryResult sq = q.store();
        bool display = true;
        while (display) {
            int choose;
            cout << "\n==================MENU=================" << endl;
            cout << "====            1. Admin              ===" << endl;
            cout << "====            2. User               ===" << endl;
            cout << "====            0. Thoát!             ===" << endl;
            cout << "=========================================" << endl;
            cout << "===Mời bạn chọn: ";
            cin >> choose;
            cin.ignore();
            if (cin.fail()) {
                cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                int n;
                switch (choose) {
                    case 1:
                        cout << "Admin" << endl;
                        n = logIn();
                        if (n > 0) {
                            cout << "Đăng nhập thành công!" << endl;
                            menuAdmin.run();
                        } else {
                            cout << "Nhập tên người dùng và mật khẩu chưa chính xác!";
                        }
                        break;
                    case 2:
                        cout << "User" << endl;
                        menuUser.menuUser();
//                        n = logIn();
//                        if (n >= 0) {
//                        } else {
//                            cout << "Nhập tên người dùng và mật khẩu chưa chính xác!";
//                        }
                        break;
                    case 0:
                        cout << "Đã thoát chương trình!" << endl;
                        display = false;
                        break;
                    default:
                        cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
                        break;
                }
            }
        }
    } catch (Exception &e) {
        cout << e.what() << endl;
    }
    return 0;
}

void signUp() {
    string userName, password;
    cout << "Mời bạn tạo Username: " << endl;
    getline(cin, userName);
    cout << "Mời bạn tạo Password: " << endl;
    getline(cin, password);
    Database::connectToDatabase();
    Query q = Database::con.query(
            "INSERT INTO users (username, password) VALUES('" + userName + "','" + password + "');");
//    execute() => Thêm, sửa, xoá dữ liệu
    q.execute();
    cout << "Bạn đã tạo thành công!";
}

int logIn() {
    int success = -1;
    try {
        string userName, password;
        cout << "Mời bạn nhập Username: " << endl;
        getline(cin, userName);
        cout << "Mời bạn nhập Password: " << endl;
        getline(cin, password);
        Database::connectToDatabase();
        Query q = Database::con.query(
                "SELECT * FROM  users u  WHERE  u.username  = '" + userName + "' AND  u.password  = '" + password +
                "';");
        StoreQueryResult sq = q.store();

        if (sq.num_rows() > 0) {
            CurrentUser::setUserID((int) sq[0]["id"]);
            if (sq[0]["role"] == "admin") {
                success = 1;
            } else success = 0;
        }
//        cout << "id admin: " << CurrentUser::getUserID() << endl;
    } catch (Exception e) {
        cout << e.what() << endl;
    }
    return success;
}
