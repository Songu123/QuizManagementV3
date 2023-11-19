//
// Created by MacBook on 16/11/2023.
//

#include "MenuQuiz.h"

using namespace mysqlpp;

using namespace std;

void MenuQuiz::menuQuiz() {
    int choose;
    while (true) {
        cout << "\nMENU QUIZ" << endl;
        cout << "1. Bắt đầu thi" << endl;
        cout << "2. Xem lịch sử thi" << endl;
        cout << "0. Thoát" << endl;
        cout << "Chọn: ";
        cin >> choose;
        if (cin.fail()) {
            cout << "Nhập sai! Vui lòng nhập lại." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            switch (choose) {
                case 1:
                    MenuQuiz::startQuiz();
                    break;
                case 2:
                    MenuQuiz::getQuizHistory();
                    break;
                case 0:
                    return;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại." << endl;
            }
        }
    }
}

// Hàm lấy ID đáp án của người dùng
int getAnswerID(int answerA, int answerB, int answerC, int answerD) {
    int id;
    int choose;
    int display = true;
    while (display) {
        cout << "Chọn (1)A, (2)B, (3)C, (4)D: ";
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
            cout << "Nhập sai! Vui lòng nhập lại" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            switch (choose) {
                case 1:
                    display = false;
                    id = answerA;
                    break;
                case 2:
                    display = false;
                    id = answerB;
                    break;
                case 3:
                    display = false;
                    id = answerC;
                    break;
                case 4:
                    display = false;
                    id = answerD;
                    break;
                default:
                    cout << "Nhập sai! Vui lòng nhập lại" << endl;
            }
        }
    }
    return id;
}

// Hàm kiểm tra trắc nghiệm Quiz
void testQuiz(int result_id, int quiz_id) {
    int answerA, answerB, answerC, answerD;
    int answerID;
    Database::connectToDatabase();
//    Truy vấn các câu hỏi có id đề = quiz_id
    Query query = Database::con.query("SELECT * FROM questions WHERE quizs_id = " + to_string(quiz_id) + "");
    StoreQueryResult sqr = query.store();
    if (sqr.num_rows() > 0) {
        for (int i = 0; i < sqr.num_rows(); ++i) {
            Row row = sqr[i];
            cout << "Câu " << i + 1 << ": " << row[1] << endl;
            int question_id = row[0];
//            Truy vấn các đáp án có id câu hỏi = question_id
            Query query1 = Database::con.query(
                    "SELECT * FROM answers WHERE questions_id = " + to_string(question_id) + "");
            StoreQueryResult sqr1 = query1.store();
            if (sqr1.num_rows() > 0) {
                for (int j = 0; j < sqr1.num_rows(); ++j) {
                    Row row2 = sqr1[j];
                    switch (j) {
                        case 0:
                            cout << "A. " << row2[2] << endl;
                            answerA = row2[0];
                            break;
                        case 1:
                            cout << "B. " << row2[2] << endl;
                            answerB = row2[0];
                            break;
                        case 2:
                            cout << "C. " << row2[2] << endl;
                            answerC = row2[0];
                            break;
                        case 3:
                            cout << "D. " << row2[2] << endl;
                            answerD = row2[0];
                            break;
                    }
                }
                answerID = getAnswerID(answerA, answerB, answerC, answerD);
//                Lưu vào cơ sở dữ liệu bảng result_detail
                Query query_save_answer = Database::con.query("INSERT INTO result_detail (results_id, questions_id, answers_id)\n"
                                                              " VALUES ("+ to_string(result_id) +", "+ to_string(question_id) + ", "+
                                                                      to_string(answerID) +");");
                query_save_answer.execute();
            }
        }
    }else {
        cout << "Không có câu hỏi nào trong đề." << endl;
    }
}

//void resultUser(string timeUser){
//    Database::connectToDatabase();
////  Truy vấn lấy tổng điểm số đáp án đúng và số câu đúng
//    Query query_sum_point_istrue = Database::con.query(" SELECT SUM(q.`point`), COUNT(q.id)  FROM questions q inner join answers a on q.id = a.questions_id "
//                                                       "INNER join quizs q2 on q2.id = q.quizs_id inner JOIN result_detail rd on rd.answers_id  = a.id  "
//                                                       "INNER join results r on rd.results_id = r.id  WHERE a.is_true = 1 and r.time = '"+ timeUser +"' ");
//    StoreQueryResult sqr = query_sum_point_istrue.store();
//    Row row = sqr[0];
////  Truy vấn lấy tổng điểm câu hỏi và số câu hỏi
//    Query query_sum_point_question = Database::con.query("SELECT SUM(q.point), count(q.id) from questions q inner join"
//                                                         " result_detail rd on rd.questions_id = q.id INNER JOIN results r on r.id = rd.results_id"
//                                                         " WHERE r.time = '"+ timeUser +"'");
//    StoreQueryResult sqr2 = query_sum_point_question.store();
//    Row row2 = sqr2[0];
//    cout << "Số câu đúng là: " << row[1] << "/" << row2[1] << " câu." << endl;
//    cout << "Điểm: " << row[0] << "/" << row2[0] << " điểm." << endl;
//}

// HÀM LẤY THỜI GIAN HIỆN TẠI
string timeCurrent(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto str = oss.str();
    return str;
}


// HÀM NGƯƠ DÙNG BẮT ĐẦU THI QUIZ
void MenuQuiz::startQuiz() {
    Database::connectToDatabase();
    int quiz_id;
    string currentTime = timeCurrent();

    Query quiz = Database::con.query("SELECT * FROM quizs");
    StoreQueryResult sqr = quiz.store();
    if (sqr.num_rows() > 0) {
        cout << "DANH SÁCH CÁC ĐỀ THI" << endl;
        for (int i = 0; i < sqr.num_rows(); ++i) {
            Row row = sqr[i];
            cout << "ID: " << row[0] << ", Tên đề: " << row[1] << ", Mô tả: " << row[2] << endl;
        }
        cout << "Chọn id đề: ";
        cin >> quiz_id;
        cin.ignore();
        if(cin.fail()){
            cout << "Nhập ID sai! Vui lòng nhập lại." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }else {
//            Lấy question_id
            Query get_values_quiz = Database::con.query("SELECT * FROM quizs WHERE id = " + to_string(quiz_id) + "");
            StoreQueryResult get_question_id = get_values_quiz.store();
            if (get_question_id.num_rows() > 0) {
                Row row2 = get_question_id[0];
                int question_id = row2[0];

//                Nhập thông tin vào bảng result
                Query insert_results = Database::con.query("INSERT INTO results(users_id, quizs_id , `time`) VALUES\n"
                                                   "( "+ to_string(CurrentUser::getUserID()) +", "+ to_string(question_id) +", '"+ currentTime +"');");
                insert_results.execute();

//                Lấy result_id từ bảng result trong Database
                Query get_values_result = Database::con.query(" SELECT * FROM results r WHERE time = '"+ currentTime +"'");
                StoreQueryResult get_results_id = get_values_result.store();
                Row row4 = get_results_id[0];
                int result_id = row4[0];
//                Thi Quiz
                testQuiz(result_id, quiz_id);
//                Kết quả người dùng
                menuResult.menuResultUser(quiz_id,currentTime);
//                resultUser(currentTime);
            }else {
                cout << "Không có đề nào với ID đã nhập!" << endl;
            }
        }
    }
}

// Kẻ đường gạch
void drawHorizontalLine(int width) {
    for (int i = 0; i < width; ++i) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

// HÀM IN LỊCH SỬ THI CỦA NGƯỜI DÙNG
void MenuQuiz::getQuizHistory() {
    Database::connectToDatabase();
    string timeQuiz, nameQuiz;
    int quizID, point;
    Query query_Quiz_User = Database::con.query("SELECT r.`time` , r.quizs_id ,q.name , COUNT(a.id)*10 FROM results r "
                                                "inner join users u on u.id = r.users_id inner join result_detail rd on r.id = rd.results_id  "
                                                "inner join quizs q on r.quizs_id = q.id INNER JOIN answers a on a.id = rd.answers_id "
                                                "WHERE u.id = "+ to_string(CurrentUser::getUserID()) +" and a.is_true = 1 GROUP by r.`time`"
                                                                                                      " ORDER by r.`time` DESC ");
    StoreQueryResult sqr_query_Quiz_User = query_Quiz_User.store();
    if(!sqr_query_Quiz_User.empty()){
        int tableWidth = 5 + 30 + 10 + 100 + 5;
        cout << "---------------------------------------------------[LỊCH SỬ THI]------------------------------------------------------------------------";
        drawHorizontalLine(tableWidth);
        cout << '|' << std::setw(5) << std::left << "STT" << '|'
                  << std::setw(30) << std::left << "NGÀY GIỜ THI" << '|'
                  << std::setw(10) << std::left << "ID ĐỀ" << '|'
                  << std::setw(100) << std::left << "TÊN ĐỀ" << '|'
                  << std::setw(5) << std::left << "ĐIỂM" << '|' << std::endl;
        drawHorizontalLine(tableWidth);
        for (int i = 0; i < sqr_query_Quiz_User.num_rows(); ++i) {
            Row row_query_Quiz_User = sqr_query_Quiz_User[i];
            timeQuiz = (string) row_query_Quiz_User[0];
            quizID = (int) row_query_Quiz_User[1];
            nameQuiz = (string) row_query_Quiz_User[2];
            point = (int) row_query_Quiz_User[3];
            cout << '|' << std::setw(5) << std::left << (i + 1) << '|'
                      << std::setw(30) << std::left << timeQuiz << '|'
                      << std::setw(10) << std::left << quizID << '|'
                      << std::setw(100) << std::left << nameQuiz << '|'
                      << std::setw(5) << std::left << point << '|' << std::endl;
            drawHorizontalLine(tableWidth);
        }
    }else {
        cout << "Lịch sử thi của bạn trống!" << endl;
    }
}

