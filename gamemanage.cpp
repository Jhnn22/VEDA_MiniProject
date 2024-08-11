#include "gamemanage.h"
#include <QDebug>
#include <QString>  // [i]연산자를 이용해 배열처럼 문자단위 접근가능!!!!, 유니코드도 지원해줘서 substr 할 필요도 없네??!!!
#include <QMessageBox>
using namespace std;

// void GameManage::gameStart(){

// }

bool GameManage::run(QString &question, QString &answer, QString &resultString){
    // 시도 횟수 초과시 문자열 비교 및 결과 부분은 필요 없으므로 시작 부분에 선언
    if(isGameOver()){
        resultString.append("남은 시도 횟수가 없습니다.");
        return false;
    }

    int strike = 0;
    int ball = 0;

    // 문자열 길이 비교
    if(question.length() < answer.length()){
        // qDebug() << "답변이 너무 깁니다. 정답의 길이(" << question.length() << ")에 맞춰 다시 시도해주세요.";
        QMessageBox::warning(nullptr, "경고", "입력한 단어의 길이가 정답보다 깁니다. 정답의 길이에 맞춰주세요.");
        return false;
    }
    else if(question.length() > answer.length()){
        answer = answer.leftJustified(question.length(), ' ');  // Qt에서는 resize 대신 leftJustified 사용
    }

    // 각 문자 비교
    for(int i=0; i<question.length(); i++){
        if(question[i] == answer[i]){
            strike++;
        }
        else if(answer.contains(question[i])){
            ball++;
        }
    }

    // 시도 횟수 증가
    currentAttempts++;

    // 결과
    if(strike == question.length()){
        // qDebug() << "정답입니다";
        resultString.append("결과>> 정답입니다!");
        return true;
    }
    else if(strike == 0 && ball == 0){
        // qDebug() << "OUT";
        resultString.append("결과>> 아웃입니다...");
    }
    else{
        // qDebug() << "스트라이크 : " <<  strike;
        // qDebug() << "볼 : " <<  ball;
        // resultString.append("스트라이크 : %1\n").arg(strike); //append사용시 .arg는 미리 선언
        // resultString.append("볼 : %1\n").arg(ball);
        QString str = QString("결과>> 스트라이크 : %1, 볼 : %2").arg(strike).arg(ball);
        resultString.append(str);
    }
    return false;
}

// 게임 종료 확인
bool GameManage::isGameOver(){
    if(currentAttempts>maxAttempts) return true;
    else return false;
}

// 시도 횟수 초기화
void GameManage::resetAttempts(){
    currentAttempts = 0;
}
