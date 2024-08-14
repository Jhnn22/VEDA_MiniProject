#include "gamemanage.h"
#include <QDebug>
#include <QString>  // [i]연산자를 이용해 배열처럼 문자단위 접근가능!!!!, 유니코드도 지원해줘서 substr 할 필요도 없네??!!!
#include <QMessageBox>
using namespace std;


GameManage::gameResult GameManage::run(QString &question, QString &answer, QString &resultString, int &currentAttempts, int &maxAttempts){
    int strike = 0;
    int ball = 0;

    // 시도 횟수 초과시 종료
    if(isGameOver(currentAttempts, maxAttempts)){
        resultString.append("남은 시도 횟수가 없습니다.");
        return gameResult::gameOver;
    }

    // 문자열 길이 비교
    if(question.length() != answer.length()){
        QMessageBox::warning(nullptr, "경고", "입력한 단어의 길이가 올바르지 않습니다. ");
        return gameResult::inValidInput;
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

    // 결과
    if(strike == question.length()){
        resultString.append("결과>> 정답입니다!");
        return gameResult::correct;
    }
    else if(strike == 0 && ball == 0){
        resultString.append("결과>> 아웃입니다...");
    }
    else{
        QString str = QString("결과>> 스트라이크 : %1, 볼 : %2").arg(strike).arg(ball);
        resultString.append(str);
    }

    // 시도 횟수 증가
    currentAttempts++;


    return gameResult::inCorrect;
}

// 게임 종료 확인
bool GameManage::isGameOver(int &currentAttempts, int &maxAttempts){
    return currentAttempts >= maxAttempts;
}

// 시도 횟수 초기화
void GameManage::resetAttempts(int &currentAttempts, int &maxAttempts){
    currentAttempts = 0;
    maxAttempts = 10;
}
