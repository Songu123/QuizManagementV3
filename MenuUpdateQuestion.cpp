

//
// Created by MacBook on 14/11/2023.
//

#include "MenuUpdateQuestion.h"

using namespace std;
using namespace mysqlpp;

//HÀM CẬP NHẬT CÂU HỎI
void MenuUpdateQuestion::updateQuestions(int quiz_id) {
    Database::connectToDatabase();
    int question_id;
    bool success = true;
    while (success) {
        try {
            Query q = Database::con.query("SELECT * FROM questions WHERE quizs_id = " + to_string(quiz_id) + "");
            StoreQueryResult sq = q.store();
            if (sq.num_rows() > 0) {
                cout << "DANH SÁCH CÁC CÂU HỎI" << endl;
                for (size_t i = 0; i < sq.size(); i++) {
                    Row row = sq[i];
                    cout << "ID: " << row[0] << ", Content: " << row[1] << ", Type: " << row[2] << ", Point: "
                         << row[3] << endl;
                }
                while (success) {
                    cout << "Nhập id question: ";
                    cin >> question_id;
                    cin.ignore();
                    if (cin.fail()) {
                        cout << "Nhập sai! Vui lòng nhập lại!!!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    } else {
                        success = false;
                        Query query = Database::con.query(
                                "SELECT * FROM questions WHERE id = " + to_string(question_id) + ";");
                        StoreQueryResult sqr = query.store();
                        if (sqr.num_rows() > 0) {
                            MenuUpdateQuestion::updateQuestion(question_id);
                        } else {
                            cout << "Không tìm thấy câu hỏi với ID đã nhập." << endl;
                        }
                    }
                }
            } else {
                success = false;
                cout << "Không có dữ liệu câu hỏi!" << endl;
            }
        } catch (Exception &e) {
            cerr << "Lỗi MySQL: " << e.what() << endl;
        }
    }
}

//HÀM CẬP NHẬT CÂU HỎI
void MenuUpdateQuestion::updateQuestion(int question_id) {
    Database::connectToDatabase();
    string content;
    int point;
    bool ok = true;
    try {
        cout << "Nhập nội dung câu hỏi: ";
        getline(cin, content);
        while (ok) {
            cout << "Nhập điểm của câu: ";
            cin >> point;
            if (cin.fail() || point <= 0) {
                cout << "Nhập sai!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                ok = false;
            }
        }
        Query q = Database::con.query(
                "UPDATE questions SET content = '" + content + "', point = " + to_string(point) + " WHERE id = " +
                to_string(question_id) + ";");
        q.execute();
        MenuUpdateQuestion::menuUpdateAnswer(question_id);
    } catch (Exception &e) {
        cerr << "Lỗi MySQL: " << e.what() << endl;
    }
}

//MENU CẬP NHÂT ĐÁP ÁN
void MenuUpdateQuestion::menuUpdateAnswer(int question_id) {
    while (true) {
        cout << "===================MENU SỬA ĐÁP ÁN==================" << endl;
        cout << "===          1. Sửa đáp án                       ===" << endl;
        cout << "===          0. Quay lại MENU CẬP NHẬT CÂU HỎI   ===" << endl;
        cout << "====================================================" << endl;
        cout << "=============Chọn: ";
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
                    MenuUpdateQuestion::updateAnswer(question_id);
                    break;
                case 0:
                    return;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
            }
        }
    }
}

//HÀM CẬP NHẬT ĐÁP ÁN
void MenuUpdateQuestion::updateAnswer(int question_id) {
    Database::connectToDatabase();
    string content;
    bool correct, ok = true, success = true;
    int answer_id;
    while (success) {
        try {
            Query query = Database::con.query(
                    "SELECT id, questions_id, content, is_true FROM answers WHERE questions_id = " +
                    to_string(question_id) +
                    ";");
            StoreQueryResult sq = query.store();
            if (sq.num_rows() > 0) {
                cout << "DANH SÁCH CÁC ĐÁP ÁN" << endl;
                for (size_t i = 0; i < sq.size(); i++) {
                    Row row = sq[i];
                    cout << "ID: " << row[0] << ", ID question: " << row[1] << ", Content: " << row[2] << ", Is True: "
                         << row[3] << endl;
                }

                cout << "Nhập id: ";
                cin >> answer_id;
                cin.ignore();
                if (cin.fail()) {
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    success = false;
                    Query query1 = Database::con.query("SELECT * FROM answers WHERE id = " + to_string(answer_id) + "");
                    StoreQueryResult sqr = query1.store();
                    if (sqr.num_rows() > 0) {
                        cout << "Nhập nội dung: ";
                        getline(cin, content);
                        while (ok) {
                            cout << "Nhập (1)Đúng, (0)Sai: ";
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
                                        correct = true;
                                        ok = false;
                                        break;
                                    case 0:
                                        correct = false;
                                        ok = false;
                                        break;
                                    default:
                                        cout << "Nhập sai! Vui lòng nhập lại." << endl;

                                }
                                Query q = Database::con.query(
                                        "UPDATE answers SET content = '" + content + "', is_true = " +
                                        to_string(correct) +
                                        " WHERE id = " +
                                        to_string(answer_id) + ";");
                                q.execute();
                            }
                        }
                    } else {
                        cout << "Không có đáp án với ID đã nhập." << endl;
                    }
                }
            } else {
                success = false;
                cout << "Không có dữ liệu đáp án!" << endl;
            }

        } catch (Exception &e) {
            cerr << "Lỗi MySQL: " << e.what() << endl;
        }
    }
}
