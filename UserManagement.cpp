//
// Created by MacBook on 08/11/2023.
//
#include "UserManagement.h"

using namespace std;
using namespace mysqlpp;

void UserManagement::run() {
    while (true) {
        cout << "\n==================MENU QUẢN LÝ NGƯỜI DÙNG====================" << endl;
        cout << "===                  1. Danh sách người dùng                ===" << endl;
        cout << "===                  2. Tạo người dùng mới                  ===" << endl;
        cout << "===                  3. Cập nhật mật khẩu người dùng        ===" << endl;
        cout << "===                  4. Quản lý kết quả người dùng          ===" << endl;
        cout << "===                  0. Quay lại trang Admin                ===" << endl;
        cout << "===============================================================" << endl;
        cout << "=====================Chọn: ";
        int choose;
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
            cout << "Nhập sai! Vui lòng nhập lại" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            switch (choose) {
                case 1:
                    cout << "\nDANH SÁCH NGƯỜI DÙNG" << endl;
                    listUsers();
                    break;
                case 2:
                    cout << "\nTẠO NGƯỜI DÙNG MỚI" << endl;
                    createUser();
                    break;
                case 3:
                    cout << "\nCHỈNH SỬA NGƯỜI DÙNG" << endl;
                    updateUser();
                    break;
                case 4:
                    cout << "\nQUẢN LÝ KẾT QUẢ NGƯỜI DÙNG" << endl;
                    UserManagement::listResultUser();
                    break;
                case 0:
                    return;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại" << endl;
                    break;
            }
        }
    }
}

// Hàm danh sách người dùng
void UserManagement::listUsers() {
    try {
        Database::connectToDatabase();
        Query q = Database::con.query(
                "SELECT id, username, date_of_birth, CAST(sex AS INT) AS sex, email, password FROM users where role = 'user'");
        StoreQueryResult sq = q.store();

        if (sq) {
            // Print the header of the table
            std::cout << std::left << std::setw(5) << "ID"
                      << std::setw(20) << "Username"
                      << std::setw(15) << "Birthdate"
                      << std::setw(10) << "Gender"
                      << std::setw(30) << "Email" << std::endl;

            // Print a horizontal line
            for (int i = 0; i < 70; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            // Print the data rows
            for (size_t i = 0; i < sq.size(); ++i) {
                Row row = sq[i];
                std::cout << std::setw(5) << row[0]
                          << std::setw(20) << row[1]
                          << std::setw(15) << row[2]
                          << std::setw(10) << row[3]
                          << std::setw(30) << row[4] << std::endl;
            }
        } else {
            std::cout << "Không có dữ liệu người dùng." << std::endl;
        }
    } catch (const Exception &e) {
        std::cerr << "Lỗi MySQL: " << e.what() << std::endl;
    }
}

//  Hàm tạo người dùng mới
void UserManagement::createUser() {
    string username, password, email, birthdate;
    int gender;
    cin.ignore();
    cout << "Nhập username: ";
    getline(cin, username);

    cout << "Nhập password: ";
    getline(cin, password);

    cout << "Nhập email: ";
    getline(cin, email);

    cout << "Nhập birthdate: ";
    getline(cin, birthdate);

    bool ok = true;
    int choose;
    while (ok) {
        cout << "Nhập gender (1)Nam, (2)Nữ: ";
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
            cout << "Nhập sai! Vui lòng nhập lại." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            switch (choose) {
                case 1:
                    gender = 1;
                    ok = false;
                    break;
                case 2:
                    gender = 0;
                    ok = false;
                    break;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
                    break;
            }
        }
    }
    try {
        Database::connectToDatabase();
        Query query = Database::con.query("INSERT INTO users(username, date_of_birth, sex, email, password) VALUES"
                                          "('" + username + "', '" + birthdate + "', " + to_string(gender) + ", '" +
                                          email + "', '" + password + "');");
        query.execute();
    } catch (const Exception &e) {
        cout << "Lỗi MySQL: " << e.what() << endl;
    }
}

//  Hàm cập nhật người dùng
void UserManagement::updateUser() {
    int id, gender;
    string username, password, email, birthdate;
    bool correct = true;
    try {
        listUsers();
        while (correct) {
            cout << "Nhập id người dùng: ";
            cin >> id;
            cin.ignore();
            if (cin.fail()) {
                cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                Query q = Database::con.query("SELECT * FROM users where id = " + to_string(id) + "");
                StoreQueryResult sqr = q.store();
                if (sqr.num_rows() > 0) {
                    correct = false;
                    cout << "Nhập password: ";
                    getline(cin, password);

                    Database::connectToDatabase();
                    Query query = Database::con.query(
                            "UPDATE users SET password = 'son123' WHERE id = " + to_string(id) + "");
                    query.execute();
                } else {
                    cout << "Không có người dùng nào trùng với id đã nhập!" << endl;
                }
            }
        }
    } catch (const Exception &e) {
        cout << "Không thể cập nhật mật khẩu người dùng" << endl;
        cout << "Lỗi MySQL: " << e.what() << endl;
    }
}

void UserManagement::listResultUser() {
    try {
        Database::connectToDatabase();
        Query query_get_Infor_User = Database::con.query(
                "SELECT u.username ,r.quizs_id, q1.name , r.`time`  ,COUNT(a.id)*10 as \"Điểm\"  FROM results r inner join users u on u.id = r.users_id inner join result_detail rd on r.id = rd.results_id"
                " inner join quizs q1 on r.quizs_id = q1.id INNER join questions q on rd.questions_id = q.id "
                "INNER join answers a on a.id = rd.answers_id WHERE a.is_true = 1  GROUP BY r.`time` "
                "ORDER by COUNT(a.is_true) DESC ");
        StoreQueryResult sqr_query_get_Infor_User = query_get_Infor_User.store();

        if (sqr_query_get_Infor_User) {
            // Print the header of the table
            std::cout << std::left << std::setw(5) << "STT"
                      << std::setw(20) << "Username"
                      << std::setw(10) << "Quiz ID"
                      << std::setw(30) << "Quiz Name"
                      << std::setw(20) << "Time"
                      << std::setw(15) << "Điểm" << std::endl;

            // Print a horizontal line
            for (int i = 0; i < 95; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            // Print the data rows
            for (size_t i = 0; i < sqr_query_get_Infor_User.size(); ++i) {
                Row row = sqr_query_get_Infor_User[i];
                std::cout << std::setw(5) << i + 1
                          << std::setw(20) << row[0]
                          << std::setw(10) << row[1]
                          << std::setw(30) << row[2]
                          << std::setw(30) << row[3]
                          << std::setw(20) << row[4] << std::endl;
            }
        } else {
            std::cout << "Không có dữ liệu kết quả người dùng." << std::endl;
        }
    } catch (const Exception &e) {
        std::cout << "Lỗi MySQL: " << e.what() << std::endl;
    }
}
