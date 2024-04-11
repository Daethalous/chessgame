#include "controller.h"
#include <QApplication>
#include <QTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

static Controller* instance;

Controller* Controller::getInstance(){
    if(instance == nullptr)
        instance = new Controller();
    return instance;
}

void Controller::init(){
    chess = new ChessBoard(nullptr);
    chess -> show();
}

void Controller::startGame(){
    chess -> setPieces();
    round = 1;
    currentPlayer = 1;
    life = 3;
    selected = nullptr;
    newTimerPerTurn();
    chess ->  ui -> label_2 ->setText("round " + QString::number(round));
    chess -> ui ->label_4 ->setText(QString::number(life) + " turns to waste");
    if(thisPlayer == currentPlayer)
        chess ->  ui -> turnShower -> setText("your turn");
    else chess ->  ui -> turnShower -> setText("opponent's turn");
}

void Controller::nextTurn(){
    checkVictory();
    checkLose();
    if(life == 0){
        sendLoseSignal();
    }
    selected = nullptr;
    status = Selecting;
    newTimerPerTurn();
    if (currentPlayer ==2){
        round++;
        currentPlayer = 1;
    }
    else
        currentPlayer++;
    chess ->  ui -> label_2 ->setText("round " + QString::number(round));
    chess -> ui ->label_4 ->setText(QString::number(life) + " turns to waste");
    if(thisPlayer == currentPlayer)
        chess ->  ui -> turnShower ->setText("your turn");
    else chess ->  ui -> turnShower ->setText("opponent's turn");
}

void Controller::onPieceClicked(int i,int j){
    if (thisPlayer == currentPlayer){
        QVariantMap obj;
        obj["msg_type"] = "operate";
        obj["i"] = i;
        obj["j"] = j;
        obj["player"] = thisPlayer;
        auto str = QJsonDocument::fromVariant(obj).toJson();
        serverSocket->write(str);
    }
}

void Controller::sendLoseSignal(){
    QVariantMap obj;
    obj["msg_type"] = "surrender";
    obj["player"] = this -> thisPlayer;
    auto str = QJsonDocument::fromVariant(obj).toJson();
    serverSocket->write(str);
}

void Controller::onSurrenderClicked(){
    if(this -> round >= 20){
        QVariantMap obj;
        obj["msg_type"] = "surrender";
        obj["player"] = this -> thisPlayer;
        auto str = QJsonDocument::fromVariant(obj).toJson();
        serverSocket->write(str);
    }
    else
        QMessageBox::information(nullptr, "error", "hold on");
}

void Controller::onStartClicked(){
    QVariantMap obj;
    obj["msg_type"] = "start";
    thisPlayer = rand()%2 + 1;
    obj["player"] = 3 - thisPlayer;
    auto str = QJsonDocument::fromVariant(obj).toJson();
    serverSocket->write(str);
}

void Controller::recvMessage(){
    QByteArray str = serverSocket->readAll();;
    qDebug()<<QString(str);
    QVariantMap obj = QJsonDocument::fromJson(str).object().toVariantMap();
    if(obj["msg_type"].toString() == "operate"){
        this -> operateChess(obj["i"].toInt(), obj["j"].toInt());
        // handle chess
    }
    if(obj["msg_type"].toString() == "surrender"){
        this -> surrender(obj["player"].toInt());
    }
    if(obj["msg_type"].toString() == "start"){
        QString info;
        if(this -> thisPlayer == 0)
            thisPlayer = obj["player"].toInt();
        startGame();
        if(thisPlayer == 1){
            info = "you are the first";
            chess -> ui -> label -> setStyleSheet("background:rgb(255,0,0);");
        }
        else {
            info = "you are the seceond";
            chess -> ui -> label -> setStyleSheet("background:rgb(0,255,255);");
        }
        QMessageBox::information(nullptr, "confirm", info);
    }

};

void Controller::operateChess(int i, int j){
    auto piece = chess -> pieces[i][j];
    qDebug() << i << " " << j ;
    if(selected == nullptr && piece -> possession == currentPlayer){
        selected = piece;//choose
        selected -> select();
        qDebug() << "chosen";
    }
    else if(selected != nullptr && piece -> possession == 0 && selected -> NeighbourPosition(piece) != -10 && status != Jumping){
        //move
        selected -> ChangePossession(0);
        piece -> ChangePossession(currentPlayer);
        selected -> unselect();
        selected = piece;
        selected -> select();
        status = Moving;
        qDebug() << "move done";
    }
    else if(selected != nullptr && piece -> possession == 0 && selected -> isSubneighbour(piece)){
        //jump
        selected -> ChangePossession(0);
        piece -> ChangePossession(currentPlayer);
        selected -> unselect();
        selected = piece;
        selected -> select();
        status = Jumping;
    }
    else if(selected == piece && (status == Jumping || status == Moving)){
        selected -> unselect();
        selected = nullptr;
        status = Selecting;
        nextTurn();
    }
    else if(selected == piece){
        selected -> unselect();
        selected = nullptr;
        status = Selecting;
    }
    else if(thisPlayer == currentPlayer){
        QMessageBox::information(nullptr, "error","bad place");
    }

//set a messagebox informing illegal action
}

void Controller::checkVictory(){
    int Target = 0;
    if(currentPlayer == 1){
        for (int i = 12; i >= 0; i--){
            for (int j = 3; j >= 0; j--){
                if ((i + j >= 12)){
                    if(this -> chess -> pieces[i][j] -> possession == 1)
                        Target ++;
                }
            }
        }
    }
    else if(currentPlayer == 2){
        for (int i = 4; i < 17; i++){
            for (int j = 13; j < 17; j++){
                if ((i + j <= 20)){
                    if(this -> chess -> pieces[i][j] -> possession == 2)
                        Target ++;
                }
            }
        }
    }
    qDebug() << Target;
    if(Target == 10){
        timerPerTurn -> stop();
        if(thisPlayer == currentPlayer)
        QMessageBox::information(nullptr, "victory", "you win");
        else
            QMessageBox::information(nullptr, "defeat", "you lose");
        QApplication::quit();
    }
}

void Controller::checkLose(){
    int Home = 0;
    if(currentPlayer == 2){
            for (int i = 12; i >= 0; i--){
                for (int j = 3; j >= 0; j--){
                    if ((i + j >= 12)){
                        if(this -> chess -> pieces[i][j] -> possession == 2)
                            Home ++;
                    }
                }
            }
        }
     else if(currentPlayer == 1){
            for (int i = 4; i < 17; i++){
                for (int j = 13; j < 17; j++){
                    if ((i + j <= 20)){
                        if(this -> chess -> pieces[i][j] -> possession == 1)
                            Home ++;
                    }
                }
            }
        }
      if((round ==20 && Home > 5)
              ||(round == 25 && Home > 2)
         ||(round == 30 && Home > 0)){
            sendLoseSignal();
      }


    qDebug() << Home;
}

void Controller::surrender(int thisplayer){
        if(this -> thisPlayer == thisplayer){
            QMessageBox::information(nullptr, "defeat", "you lose");
        }
        else{
            QMessageBox::information(nullptr, "victory", "you win");
        }
        QApplication::quit();
}
