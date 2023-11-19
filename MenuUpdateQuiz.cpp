//
// Created by MacBook on 13/11/2023.
//

#include "MenuUpdateQuiz.h"

using namespace std;
using namespace mysqlpp;

//MENU CẬP NHẬT ĐỀ
void MenuUpdateQuiz::menuUpdateQuiz() {
    bool success = true;
    while (success) {
        cout << "MENU CẬP NHẬT QUIZ" << endl;
        cout << "1. Cập nhật đề" << endl;
        cout << "0. Quay lại QUẢN LÝ QUIZ" << endl;
        cout << "Mời chọn: ";
        int choose;
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
            cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            switch (choose) {
                case 1:
                    MenuUpdateQuiz::updateQuiz();
                    break;
                case 0:
                    return;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;

            }
        }
    }
}

//MENU CẬP NHẬT CÂU HỎI
void MenuUpdateQuiz::menuUpdateQuestion(int quiz_id) {
    while (true) {
        cout << "\n===============MENU CẬP NHẬT CÂU HỎI=============" << endl;
        cout << "====          1. Sửa câu hỏi                   ====" << endl;
        cout << "====          0. Quay lại MENU CẬP NHẬT QUIZ   ====" << endl;
        cout << "===================================================" << endl;
        cout << "==============Chọn: ";
        int choose;
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
            cout << "Nhập sai! Vui lòng nhập lại." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            switch (choose) {
                case 1:
                    mu.updateQuestions(quiz_id);
                    break;
                case 0:
                    return;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
            }
        }
    }
}

//HÀM CẬP NHẬT ĐỀ
void MenuUpdateQuiz::updateQuiz() {
    Database::connectToDatabase();
    string name, description;
    bool success = true;
    while (success) {
        try {
            Query qr = Database::con.query("SELECT * FROM quizs");
            StoreQueryResult sqr = qr.store();
            if (sqr.num_rows() > 0) {
                cout << "DANH SÁCH CÁC ĐỀ" << endl;
                for (size_t i = 0; i < sqr.size(); i++) {
                    Row row = sqr[i];
                    cout << "ID: " << row[0] << ", Name: " << row[1] << ", Description: " << row[2] << endl;
                }
                int quiz_id;
                cout << "Nhập id: ";
                cin >> quiz_id;
                cin.ignore();
                if (cin.fail()) {
                    cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    success = false;
                    Query q = Database::con.query("SELECT * FROM quizs where id = " + to_string(quiz_id) + "");
                    StoreQueryResult sqr = q.store();
                    if (sqr.num_rows() > 0) {
                        cout << "Nhập tên đề mới: ";
                        getline(cin, name);
                        cout << "Nhập mô tả mới: ";
                        getline(cin, description);
                        Query query = Database::con.query(
                                "UPDATE quizs SET name = '" + name + "', description = '" + description + "' WHERE id = " +
                                to_string(quiz_id) + ";");
                        query.execute();
                        MenuUpdateQuiz::menuUpdateQuestion(quiz_id);
                    } else {
                        cout << "Không tìm thấy đề với ID đã nhập." << endl;
                    }
                }
            } else {
                success = false;
                cout << "Không có dữ liệu đề!" << endl;
            }
        } catch (Exception &e) {
            cerr << "Lỗi MySQL: " << e.what() << endl;
        }
    }

}





