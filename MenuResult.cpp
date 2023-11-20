//
// Created by MacBook on 18/11/2023.
//

#include "MenuResult.h"
using namespace std;
using namespace mysqlpp;
void MenuResult::menuResultUser(int quizID, string timeUser) {
    MenuResult::resultUser(timeUser);
    int choose;
    while(true){
        cout << "\n=============MENU KẾT QUẢ CỦA BẠN=========" << endl;
        cout << "===      1. Xem kết quả chi tiết.        ===" << endl;
        cout << "===      2. Xem bảng xếp hạng            ===" << endl;
        cout << "===      0. Thoát.                       ===" << endl;
        cout << "============================================" << endl;
        cout << "=========Chọn: ";
        cin >> choose;
        cin.ignore();
        if(cin.fail()){
            cout << "Nhập sai! Vui lòng nhập lại" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }else {
            switch (choose) {
                case 1:
                    MenuResult::resultDetail(timeUser);
                    break;
                case 2:
                    MenuResult::rankQuiz(quizID);
                    break;
                case 0:
                    return;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
            }
        }
    }
}

// HÀM XEM KẾT QUẢ CHI TIẾT
void MenuResult::resultDetail(string timeUser) {
    try {
        int question_id, answerUser;
        string contentQuestion, contentAnswerUser;
        Database::connectToDatabase();
        Query get_result_detail = Database::con.query("SELECT q.id , q.content , a.content, CAST(a.is_true AS INT) AS correct FROM results r INNER JOIN result_detail rd ON r.id  = rd.results_id "
                                                      "INNER JOIN questions q ON q.id = rd.questions_id INNER JOIN answers a ON a.id = rd.answers_id"
                                                      " WHERE r.time = '"+ timeUser +"'");
        StoreQueryResult sqr_get_result_detail =  get_result_detail.store();
        if(!sqr_get_result_detail.empty()){
            for(int i = 0; i < sqr_get_result_detail.size(); i++){
                Row row_get_result_detail = sqr_get_result_detail[i];
                question_id = (int)row_get_result_detail[0];
                answerUser = (int)row_get_result_detail[3];
                contentAnswerUser = (string)row_get_result_detail[2];
                cout << "Câu " << (i + 1) << ": " << row_get_result_detail[1] << endl;
                Query get_answer_question = Database::con.query("SELECT a.id, a.questions_id , a.content , a.is_true FROM answers a WHERE a.questions_id = "+
                                                                to_string(question_id) +";");
                StoreQueryResult sqr_get_answer_question = get_answer_question.store();
                for (int j = 0; j < sqr_get_answer_question.num_rows(); j++) {
                    Row row_get_answer_question = sqr_get_answer_question[j];
                    switch (j) {
                        case 0:
                            cout << "A. " << row_get_answer_question[2] << endl;
                            break;
                        case 1:
                            cout << "B. " << row_get_answer_question[2] << endl;
                            break;
                        case 2:
                            cout << "C. " << row_get_answer_question[2] << endl;
                            break;
                        case 3:
                            cout << "D. " << row_get_answer_question[2] << endl;
                            break;
                    }
                }
                cout << "- Đáp án bạn chọn: " << contentAnswerUser << endl;
                if(answerUser == 1){
                    cout << "- Đây là đáp án đúng!" << endl;
                }else {
                    cout << "- Đây là đáp án sai!" << endl;
                    Query query_answer_true = Database::con.query(
                            "SELECT a.content FROM answers a WHERE questions_id = " + to_string(question_id) +
                            " AND a.is_true = 1");
                    StoreQueryResult sqr_query_answer_true = query_answer_true.store();
                    Row row_query_answer_true = sqr_query_answer_true[0];
                    cout << "- Đáp án đúng là: " << row_query_answer_true[0] << endl;
                }
            }
        }else {
            cout << "Không thể xem kết quả chi tiết!" << endl;
        }
    }catch (Exception &e){
        cout << "Lỗi: " << e.what() << endl;
    }
}

// HÀM KẾT QUẢ NGƯỜI DÙNG SAU KHI THI
void MenuResult::resultUser(string timeUser){
    try {
        Database::connectToDatabase();
//  Truy vấn lấy tổng điểm số đáp án đúng và số câu đúng
        Query query_sum_point_istrue = Database::con.query(" SELECT SUM(q.`point`), COUNT(q.id)  FROM questions q inner join answers a on q.id = a.questions_id "
                                                           "INNER join quizs q2 on q2.id = q.quizs_id inner JOIN result_detail rd on rd.answers_id  = a.id  "
                                                           "INNER join results r on rd.results_id = r.id  WHERE a.is_true = 1 and r.time = '"+ timeUser +"' ");
        StoreQueryResult sqr = query_sum_point_istrue.store();
        Row row = sqr[0];
//  Truy vấn lấy tổng điểm câu hỏi và số câu hỏi
        Query query_sum_point_question = Database::con.query("SELECT SUM(q.point), count(q.id) from questions q inner join"
                                                             " result_detail rd on rd.questions_id = q.id INNER JOIN results r on r.id = rd.results_id"
                                                             " WHERE r.time = '"+ timeUser +"'");
        StoreQueryResult sqr2 = query_sum_point_question.store();
        Row row2 = sqr2[0];
        cout << "\n*****************************************" << endl;
        cout << "***   Số câu đúng là: " << row[1] << "/" << row2[1] << " câu    ***" << endl;
        cout << "***   Điểm: " << row[0] << "/" << row2[0] << " điểm.            ***" << endl;
        cout << "*****************************************" << endl;

    }catch(Exception &e) {
        cout << "Lỗi: " << e.what() << endl;
    }
}

int MenuResult::sumPointQuiz(string time_User){
    string timeUser = time_User;
    int pointQuiz;
    Database::connectToDatabase();
    Query query_sum_point_question = Database::con.query("SELECT SUM(q.point), count(q.id) from questions q inner join"
                                                         " result_detail rd on rd.questions_id = q.id INNER JOIN results r on r.id = rd.results_id"
                                                         " WHERE r.time = '"+ (timeUser) +"'");
    StoreQueryResult sqr2 = query_sum_point_question.store();
    Row row2 = sqr2[0];
    pointQuiz = (int) row2[0];
    return pointQuiz;
}

// HÀM HIỂN THỊ BẢNG XẾP HẠNG
void MenuResult::rankQuiz(int quiz_id) {
    string username, timeUser;
    int point;
    try{
        Database::connectToDatabase();
        Query query_username_time_point = Database::con.query("SELECT u.username, r.`time`,COUNT(a.id)*10 as \"Điểm\"  FROM results r inner join users u on u.id = r.users_id inner join result_detail rd on r.id = rd.results_id INNER join questions q on rd.questions_id = q.id INNER join answers a on a.id = rd.answers_id WHERE r.quizs_id = "+
                                                              to_string(quiz_id) +" and a.is_true = 1  GROUP BY r.`time` ORDER by COUNT(a.is_true) DESC ");
        StoreQueryResult sqr_query_username_time_point = query_username_time_point.store();
        if(!sqr_query_username_time_point.empty()){
            cout << "-----------------------BẢNG XẾP HẠNG TRONG ĐỀ-------------------" << endl;
            const int columnWidth = 30;

            // In khung đầu tiên
            std::cout << std::setfill('-') << std::setw(columnWidth * 3 + 4) << "" << std::setfill(' ') << std::endl;

            // In tên cột
            std::cout << '|' << std::setw(5) << std::left << "STT" << '|'
                      << std::setw(columnWidth) << std::left << "Username" << '|'
                      << std::setw(columnWidth) << std::left << "Thời gian" << '|'
                      << std::setw(columnWidth) << std::left << "Điểm" << '|' << std::endl;

            // In khung thứ hai
            std::cout << std::setfill('-') << std::setw(columnWidth * 3 + 4) << "" << std::setfill(' ') << std::endl;
            std::cout << std::setfill('-') << std::setw(columnWidth * 3 + 4) << "" << std::setfill(' ') << std::endl;

            for (int i = 0; i < sqr_query_username_time_point.num_rows(); ++i) {
//                Row row_query_username_time_point = sqr_query_username_time_point[i];
//                username = (string)row_query_username_time_point[0];
//                timeUser = (string)row_query_username_time_point[1];
//                point = row_query_username_time_point[2];
//                std::cout << '|' << std::setw(5) << std::left << i + 1 << '|'
//                          << std::setw(columnWidth) << std::left << username << '|'
//                          << std::setw(columnWidth) << std::left << timeUser << '|'
//                          << std::setw(3) << std::left << point << "/" << std::setw(5) << sumPointQuiz(
//                        timeUser)<< '|' << std::endl;
                Row row_query_username_time_point = sqr_query_username_time_point[i];
                std::cout << '|' << std::setw(5) << std::left << i + 1 << '|'
                          << std::setw(columnWidth) << std::left << row_query_username_time_point[0] << '|'
                          << std::setw(columnWidth) << std::left << row_query_username_time_point[1] << '|'
                          << std::setw(3) << std::left << row_query_username_time_point[2]
//                          << '/' << sumPointQuiz(row_query_username_time_point[2])
                          << '|' << std::endl;
                // In khung xen kẽ
                std::cout << std::setfill('-') << std::setw(columnWidth * 3 + 4) << "" << std::setfill(' ') << std::endl;
            }
        }
    }catch(Exception &e) {
        cout << "Lỗi: " << e.what() << endl;
    }
}




