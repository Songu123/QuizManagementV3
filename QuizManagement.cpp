//
// Created by MacBook on 08/11/2023.
//
#include "CurrentUser.h"
#include "QuizManagement.h"
#include "MenuUpdateQuiz.h"
using namespace std;
using namespace mysqlpp;

//    Hàm chạy
void QuizManagement::run(){
    while(true){
        cout << "\n===============QUẢN LÝ QUIZ===========" << endl;
        cout << "====        1. Danh sách các đề     ====" << endl;
        cout << "====        2. Thêm đề              ====" << endl;
        cout << "====        3. Sửa đề               ====" << endl;
        cout << "====        4. Xoá đề               ====" << endl;
        cout << "====        0. Quay lại Admin       ====" << endl;
        cout << "========================================" << endl;
        cout << "============Chọn: ";
        int choose;
        cin >> choose;
        cin.ignore();
        switch (choose){
            case 1:
                cout << "DANH SÁCH CÁC ĐỀ" << endl;
                QuizManagement::listQuizs();
                break;
            case 2:
                cout << "THÊM ĐỀ" << endl;
                QuizManagement::insertQuiz();
                break;
            case 3:
                cout << "SỬA ĐỀ" << endl;
                menuUpdateQuiz.menuUpdateQuiz();
                break;
            case 4:
                cout << "XOÁ ĐỀ" << endl;
                deleteQuiz();
                break;
            case 0:
                return;
            default:
                cout << "Chọn sai! Vui lòng chọn lại" << endl;
                break;
        }
    }
}

void printHorizontalLine(int width) {
    for (int i = 0; i < width; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
}

void QuizManagement::listQuizs(){
    int quizID;
    string nameQuiz;
    try {
        Database::connectToDatabase();
        Query query = Database::con.query("SELECT * from quizs");
        StoreQueryResult sqr = query.store();
        if(sqr){
            cout << "DANH SÁCH CÁC ĐỀ" << endl;
            printHorizontalLine(10 + 100 + 3);
            std::cout << "| " << std::left << std::setw(10) << "ID" << " | " << std::setw(100) << "Tên" << " |" << std::endl;
            printHorizontalLine(10 + 100 + 3);

            for(size_t i = 0; i < sqr.size(); i++){
                Row row = sqr[i];
                quizID = (int) row[0];
                nameQuiz = (string) row[1];
                std::cout << "| " << std::left << std::setw(10) << quizID << " | " << std::setw(100) << nameQuiz << " |" << std::endl;
                printHorizontalLine(10 + 100 + 3);
            }
        } else {
            cout << "Không có dữ liệu!" << endl;
        }
    } catch (const Exception &e) {
        cerr << "Lỗi MySQL: " << e.what() << endl;
    }
}

void QuizManagement::insertAnswers(int id){
    string content;
    bool success, ok = true;
    try {
        cout << "Nhập nội dung đáp án: ";
        getline(cin, content);
        while (ok){
            cout << "Nhập (1)Đúng, (0)Sai: ";
            int choose;
            cin >> choose;
            cin.ignore();
            if(cin.fail()){
                cout << "Nhập sai! Vui lòng nhập lại." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }else{
                switch(choose){
                    case 1:
                        success = true;
                        ok = false;
                        break;
                    case 0:
                        success = false;
                        ok = false;
                        break;
                    default:
                        cout << "Nhập sai! Vui lòng nhập lại." << endl;
                }
            }
        }
        Database::connectToDatabase();
        Query query = Database::con.query("INSERT INTO answers(questions_id, content, is_true) VALUES "
                                          "(" + to_string(id) + ",'" + content + "'," + to_string(success) + ");");
        query.execute();
    }catch (Exception &e) {
        cout << e.what() << endl;
    }
}

void QuizManagement::insertQuestions(int id){
    string content, type;
    int point;
    try {
        cout << "Nhập nội dung câu hỏi: ";
        getline(cin, content);
        cout << "Nhập độ khó: ";
        getline(cin, type);
        bool ok = true;
        while (ok){
            cout << "Nhập điểm của câu: ";
            cin >> point;
            if(cin.fail() || point <= 0){
                cout << "Nhập sai!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }else {
                ok = false;
            }
        }
        cin.ignore();
        Database::connectToDatabase();
        Query query = Database::con.query("INSERT INTO questions(content, type, point, quizs_id) VALUES "
                                          "('" + content + "','" + type + "','" + to_string(point) + "'," + to_string(id) + ")");
        query.execute();
        Query q = Database::con.query("SELECT LAST_INSERT_ID()");
        StoreQueryResult sqr = q.store();
        if (sqr) {
            Row row = sqr[0];
            int questions_id = row[0];
            for (int i = 0; i < 4; i++) {
                cout << "+ Đáp án thứ " << i + 1 << endl;
                insertAnswers(questions_id);
            }
        }else {
            cout << "Nhập câu hỏi không thành công!" << endl;
        }
    }catch (Exception &e) {
        cout << e.what() << endl;
    }
}

void QuizManagement::insertQuiz(){
    string name, description;
    try {
        cout << "Nhập tên đề: ";
        getline(cin, name);
        cout << "Nhập mô tả đề: ";
        getline(cin, description);
        Database::connectToDatabase();
        Query query = Database::con.query("INSERT INTO quizs(name, description, users_id) VALUES ('"+ name + "','"+ description + "','"+to_string(CurrentUser::getUserID())+"');");
        query.execute();

        Query q = Database::con.query("SELECT id FROM quizs ORDER BY id DESC LIMIT 1");
        StoreQueryResult sqr = q.store();
        if(sqr){
            Row row = sqr[0];
            int quizs_id = row[0], n = -1;
            bool correct = true;
            while(correct) {
                cout << "Nhập số lượng câu hỏi: ";
                cin >> n;
                if(cin.fail() || n <= 0){
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    correct = false;
                    cin.ignore();
                }
            }
            for(int i = 0; i < n; i++){
                cout << "- Câu hỏi thứ " << i + 1 << endl;
                insertQuestions(quizs_id);
            }
        }else {
            cout << "Tạo đề không thành công!" << endl;
        }
    } catch (const Exception &e) {
        cerr << "Lỗi MySQL: " << e.what() << endl;
    }
}

void QuizManagement::deleteQuiz() {
    Database::connectToDatabase();
    int id;
    bool ok = true;
    try {
        Query qr = Database::con.query("SELECT * FROM quizs");
        StoreQueryResult sqr = qr.store();
        if(sqr){
            cout << "DANH SÁCH CÁC ĐỀ" << endl;
            for(size_t i = 0; i < sqr.size(); i++){
                Row row = sqr[i];
                cout << "ID: " << row[0] << ", Name: " << row[1] << ", Description: " << row[2] << endl;
            }
        } else {
            cout << "Không có dữ liệu!" << endl;
        }
        while (ok){
            cout << "Vui lòng nhập id đề: ";
            cin >> id;
            if(cin.fail()){
                cout << "Nhập sai!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }else {
                ok = false;
            }
        }
        Query query = Database::con.query("DELETE FROM quizs WHERE id = " + to_string(id) + "");
        query.execute();
    }catch (const Exception &e) {
        cout << "Lỗi: " << e.what() << endl;
    }

}
