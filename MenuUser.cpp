#include "MenuUser.h"
using namespace std;

void MenuUser::menuUser() {
    int choice, role;
    while (true) {
        cout << "===============MENU NGƯỜI DÙNG===========" << endl;
        cout << "===            1. Đăng nhập           ===" << endl;
        cout << "===            0. Thoát               ===" << endl;
        cout << "=========================================" << endl;
        cout << "=====Nhập lựa chọn của bạn: ";
        cin >> choice;
        cin.ignore();
        if(cin.fail()){
            cout << "Nhập sai! Vui lòng nhập lại." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }else {
            switch (choice) {
                case 1:
                    cout << "Bạn đã chọn đăng nhập" << endl;
                    role = MenuUser::logIn();
                    if(role >= 0){
                        cout << "Đăng nhập thành công!" << endl;
                    menuQuiz.menuQuiz();
                    }else {
                        cout << "[Lỗi đăng nhập] Username hoặc Password chưa chính xác!" << endl;
                    }
                    break;
                case 0:
                    cout << "Bạn đã chọn thoát!" << endl;
                    return;
                default:
                    cout << "Bạn đã chọn sai! Vui lòng nhập lại!" << endl;
            }
        }
    }
}

void MenuUser::signUp() {
    string userName, password;
    try{
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
    } catch(Exception &e){
        cout << "Lỗi: " << e.what() << endl;
    }
}

int MenuUser::logIn() {
    int success = -1;
    try {
        string userName, password;
        cout << "Mời bạn nhập Username: ";
        getline(cin, userName);
        cout << "Mời bạn nhập Password: ";
        getline(cin, password);
        Database::connectToDatabase();
        Query q = Database::con.query(
                "SELECT * FROM  users u  WHERE  u.username  = '" + userName + "' AND  u.password  = '" + password +
                "';");
        StoreQueryResult sq = q.store();

        if (sq.num_rows() > 0) {
            Row row = sq[0];
            CurrentUser::setUserID((int) row[0]);
            if (sq[0]["role"] == "admin") {
                success = 1;
            } else success = 0;
        }
    } catch (Exception e) {
        cout << e.what() << endl;
    }
    return success;
}