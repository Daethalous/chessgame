#ifndef PIECE_H
#define PIECE_H

#include <QPushButton>
#include <QDebug>

class Piece : public QPushButton
{
    Q_OBJECT

public:
    Piece* neighbours[6];
    int possession;    //1~6 for each player 0 for none
    QPointF position;   //for centre point    
    const static int R = 10;


    Piece(QWidget* parent, int possession, QPointF position) : QPushButton(parent), possession(possession), position(position){
        this -> setGeometry(position.x() - R, position.y() - R, 2*R, 2*R);
        switch(possession){
            case 0:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(157,157,157);");
            break;
        case 1:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(255,0,0);");
            break;
        case 2:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(0,255,255);");
            break;
        }

    };

    void ChangePossession(int possession){
        this -> possession = possession;
        switch(possession){
        case 0:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(157,157,157);");
            break;
        case 1:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(255,0,0);");
            break;
        case 2:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(0,255,255);");
            break;
        }
    }

    /**
     * @brief NeighbourPosition
     * @param piece
     * @return 0~5 for each direction of adjacent piece
     *          -1 when not adjacent
     */
    int NeighbourPosition(Piece* piece){
        for(int i = 0; i < 6; i++){
            if (this -> neighbours[i] == piece)
                return i;
        }
        return -10;
    }

    /**
     * @brief isSubneighbour
     * @param piece
     * @return 所选棋子与当前棋子是否直线相隔一棋子
     */
    bool isSubneighbour(Piece* piece){
        for(int i = 0; i < 6; i++){
            //qDebug() << i << " " << this -> neighbours[i] ->NeighbourPosition(piece);
            if (this -> neighbours[i] != nullptr && this -> neighbours[i] ->NeighbourPosition(piece) == i && this -> neighbours[i] -> possession != 0){
                qDebug() << "ready";
                return true;
            }
        }
        return false;
    }

    void select(){
        switch(this -> possession){
        case 0:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(157,157,157);");
            break;
        case 1:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(100,0,0);");
            break;
        case 2:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(0,200,255);");
            break;
        }
    }

    void unselect(){
        switch(possession){
        case 0:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(157,157,157);");
            break;
        case 1:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(255,0,0);");
            break;
        case 2:
            this -> setStyleSheet("border:2px groove gray;border-radius:10px;background:rgb(0,255,255);");
            break;
        }
    }

private:

};

#endif // PIECE_H
