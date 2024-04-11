#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QtNetwork>

#include "piece.h"

namespace Ui {
class Chess;
}

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    Piece* selected;
    explicit ChessBoard(QWidget *parent = nullptr);
    bool Select(Piece* piece, int player);
    void Count();
    bool Win(int player);
    //void mousePressEvent(QMouseEvent*);
    ~ChessBoard();
    Ui::Chess *ui;
    int W, H;
    Piece* pieces[17][17];
    void onPieceClicked(int i,int j);
    QTcpSocket* serverSocket;
    void recvMessage();
    void setPieces();
private slots:
    void on_createConnectionBtn_clicked();
    void on_connectServerBtn_clicked();
    void on_surrenderBtn_clicked();
    void on_startBtn_clicked();
};


#endif // CHESSBOARD_H
