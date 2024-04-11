#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QtNetwork>

#include "piece.h"
#include "chessboard.h"
#include "gameserver.h"
#include "ui_chessboard.h"

enum Status{
    Selecting = 0,
    Jumping = 1,
    Moving = 2,
};

class Controller : public QObject
{
    Q_OBJECT

public:
    ChessBoard* chess;
    Status status;
    int thisPlayer = 0;
    int life;
    static Controller* getInstance();
    void init();
    void nextTurn();
    void checkVictory();
    void checkLose();
    void onPieceClicked(int i,int j);
    void onSurrenderClicked();
    void onStartClicked();
    void sendLoseSignal();
    void updateTime(){
        time_out++;
        turnStartTime = turnStartTime.addSecs(1);
        chess -> ui -> timeRecord->setText(turnStartTime.toString("hh:mm:ss"));
        if(time_out == 20){
        if(thisPlayer == currentPlayer){
        QMessageBox::information(nullptr, "timeout", "you're too slow!");
        life --;
        }
        nextTurn();
        }
    }
    void newTimerPerTurn(){
        if(timerPerTurn != nullptr && timerPerTurn->isActive()){
            delete timerPerTurn;
            timerPerTurn=nullptr;
        }
        time_out =0;
        turnStartTime.setHMS(0, 0, 0);
        timerPerTurn = new QTimer();
        timerPerTurn -> setSingleShot(false);
        timerPerTurn -> start(1000);
        connect(timerPerTurn, &QTimer::timeout, this, &Controller::updateTime);
    }
    int time_out;
    QTimer* timerPerTurn = nullptr;
    QTime turnStartTime;
    GameServer* gameServer = nullptr;//nullptr in client's
    QTcpSocket* serverSocket = nullptr;
    void initClient(QString ip){
        this-> serverSocket = new QTcpSocket;
        this->serverSocket->connectToHost(QHostAddress(ip), 14514);
        this->serverSocket->connected();
        connect(this->serverSocket,&QTcpSocket::readyRead,this,&Controller::recvMessage);
    };
    void recvMessage();
    void operateChess(int i, int j);
    void surrender(int thisplayer);
    void startGame();

private:
    Controller(){};
    int round;
    int currentPlayer;
    int cntHome[6], cntTarget[6];
    Piece* selected;
};
#endif // CONTROLLER_H
