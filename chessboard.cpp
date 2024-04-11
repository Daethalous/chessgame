#include "chessboard.h"
#include "ui_chessboard.h"
#include "piece.h"
#include "controller.h"
#include "gameserver.h"
#include <QPushButton>
#include <QDebug>
#include <QInputDialog>

ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chess)
{
    ui->setupUi(this);
    selected = nullptr;
    W = this -> width();
    H = this -> height();
    QPointF pos[17][17];
    for (int i = 0; i < 17; i++){
        for (int j = 0; j < 17; j++){
            pos[i][j] = QPointF(W/2 + H/20/sqrt(3) * (2 * i + j - 24), H/10 * 5 + H/20 * (j - 8));
            auto onclick = [=](bool){
                Controller::getInstance() -> onPieceClicked(i,j);
            };

            if ((i >= 4 && j > 12 && i + j <= 20)){
                //opponent's pieces
                pieces[i][j] = new Piece(this, 0, pos[i][j]);
                connect(pieces[i][j], &QPushButton::clicked, this,onclick);
            }
            else if (i <= 12 && j < 4 && i + j >= 12){
                //player's pieces
                pieces[i][j] = new Piece(this, 0, pos[i][j]);
                connect(pieces[i][j], &QPushButton::clicked, this,onclick);
            }

            else if ((i >= 4 && j >= 4 && i + j <= 20)
                 || (i <= 12 && j <= 12 && i + j >= 12)){
                //set background
                pieces[i][j] = new Piece(this, 0, pos[i][j]);
                connect(pieces[i][j], &QPushButton::clicked, this,onclick);
            }
            else pieces[i][j] = nullptr;
        }
    }
    qDebug() << "test";
    for (int i = 1; i < 17; i++){
        for (int j = 0; j < 17; j++){
            if(pieces[i][j] != nullptr){
            if(i < 16)
                pieces[i][j] -> neighbours[0] = pieces[i + 1][j];
            else
                pieces[i][j] -> neighbours[0] = nullptr;
            if(i < 16 && j > 0)
                pieces[i][j] -> neighbours[1] = pieces[i + 1][j - 1];
            else
                pieces[i][j] -> neighbours[1] = nullptr;
            if(j > 0)
                pieces[i][j] -> neighbours[2] = pieces[i][j - 1];
            else
                pieces[i][j] -> neighbours[2] = nullptr;
            if(i > 0)
                pieces[i][j] -> neighbours[3] = pieces[i - 1][j];
            else
                pieces[i][j] -> neighbours[3] = nullptr;
            if(i > 0 && j < 16)
                pieces[i][j] -> neighbours[4] = pieces[i - 1][j + 1];
            else
                pieces[i][j] -> neighbours[4] = nullptr;
            if(j < 16)
                pieces[i][j] -> neighbours[5] = pieces[i][j + 1];
            else
                pieces[i][j] -> neighbours[5] = nullptr;
        }
        }
    }   //set neighbours
}

void ChessBoard::setPieces(){
    for (int i = 0; i < 17; i++){
        for (int j = 0; j < 17; j++){
            if ((i >= 4 && j > 12 && i + j <= 20)){
                pieces[i][j] -> ChangePossession(1);
            }
            else if (i <= 12 && j < 4 && i + j >= 12){
                pieces[i][j] -> ChangePossession(2);
            }
        }
    }
}

ChessBoard::~ChessBoard()
{
    delete ui;
}

void ChessBoard::on_createConnectionBtn_clicked()
{
    Controller::getInstance() -> gameServer = new GameServer(this);
    Controller::getInstance() -> gameServer ->initServer();
    Controller::getInstance() -> initClient("127.0.0.1");
}


void ChessBoard::on_connectServerBtn_clicked()
{
    auto serverIp = QInputDialog::getText(nullptr, "connect", "ip:");
    Controller::getInstance() -> initClient(serverIp);
}


void ChessBoard::on_surrenderBtn_clicked()
{
    Controller::getInstance() -> onSurrenderClicked();
}


void ChessBoard::on_startBtn_clicked()
{
    if(Controller::getInstance() -> gameServer == nullptr)
        QMessageBox::information(nullptr, "error", "wait for server to start");
    else
        Controller::getInstance() -> onStartClicked();
}

