#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    W = this -> width();
    H = this -> height();
    QPointF temp(W/2 - 10, H/10 * 5 - 10);
    R0 = temp;
    r0 = 0;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            QPointF temp(W/2 - 10 + H/20/sqrt(3) * (1 + i - j), H/10 * 5 - 10 + H/20 * (i + j + 1));
            a[i][j] = temp;
            A[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            QPointF temp(W/2 - 10 + H/20/sqrt(3) * (2 + 2 * i + j), H/10 * 5 - 10 + H/20 * j);
            b[i][j] = temp;
            B[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            QPointF temp(W/2 - 10 + H/20/sqrt(3) * (1 + i + 2 * j), H/10 * 5 - 10 + H/20 * (- i - 1));
            c[i][j] = temp;
            C[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            QPointF temp(W/2 - 10 + H/20/sqrt(3) * (- 1 - i + j), H/10 * 5 - 10 + H/20 * (- i - j - 1));
            d[i][j] = temp;
            D[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            QPointF temp(W/2 - 10 + H/20/sqrt(3) * (- 2 - 2 * i - j), H/10 * 5 - 10 + H/20 * (- j));
            e[i][j] = temp;
            E[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 5; j++){
            QPointF temp(W/2 - 10 + H/20/sqrt(3) * (- 1 - i - 2 * j), H/10 * 5 - 10 + H/20 * (i + 1));
            f[i][j] = temp;
            F[i][j] = 0;
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 4; j >= 4 - i; j --){
            A[i][j] = 1;
            D[i][j] = -1;
        }
    }
}

bool operator<=(QPointF a,QPointF b){
    if (a.x() - b.x() >= -10
            && a.x() - b.x() <= 10
            && a.y() - b.y() >= -10
            && a.y() - b.y() <= 10)
        return true;
    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* ev){
    if(ev->button()==Qt::LeftButton){
            QPointF pos = ev->position();
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 5; j++){
                    if (pos <= R0){
                        r0 = 1;
                        break;
                    }
                    if (pos <= a[i][j]){
                        A[i][j] = 1;
                        break;
                    }
                    if (pos <= b[i][j]){
                        B[i][j] = 1;
                        break;
                    }
                    if (pos <= c[i][j]){
                        C[i][j] = 1;
                        break;
                    }
                    if (pos <= d[i][j]){
                        D[i][j] = 1;
                        break;
                    }
                    if (pos <= e[i][j]){
                        E[i][j] = 1;
                        break;
                    }
                    if (pos <= f[i][j]){
                        F[i][j] = 1;
                        break;
                    }
                }
            }
            update();
    }
}

void MainWindow::paintEvent(QPaintEvent* ev)
{
QPainter p(this);
p.setRenderHint(QPainter::Antialiasing);
p.setPen(Qt::black);
p.setBrush(Qt::gray);
p.drawEllipse(R0, 10, 10);
for(int i = 0; i < 4; i ++){
    for(int j = 0; j < 5; j ++){
        p.drawEllipse(a[i][j], 10, 10);
        p.drawEllipse(b[i][j], 10, 10);
        p.drawEllipse(c[i][j], 10, 10);
        p.drawEllipse(d[i][j], 10, 10);
        p.drawEllipse(e[i][j], 10, 10);
        p.drawEllipse(f[i][j], 10, 10);
    }
}
p.setBrush(QColor(59,197,187));
for(int i = 0; i < 4; i ++){
    for(int j = 0; j < 5; j ++){
        if (r0 == 1){
            p.drawEllipse(R0, 10, 10);
        }
        if (A[i][j] == 1){
            p.drawEllipse(a[i][j], 10, 10);
        }
        if (B[i][j] == 1){
            p.drawEllipse(b[i][j], 10, 10);
        }
        if (C[i][j] == 1){
            p.drawEllipse(c[i][j], 10, 10);
        }
        if (D[i][j] == 1){
            p.drawEllipse(d[i][j], 10, 10);
        }
        if (E[i][j] == 1){
            p.drawEllipse(e[i][j], 10, 10);
        }
        if (F[i][j] == 1){
            p.drawEllipse(f[i][j], 10, 10);
        }
    }
}
p.setBrush(Qt::yellow);
for(int i = 0; i < 4; i ++){
    for(int j = 0; j < 5; j ++){
        if (r0 == -1){
            p.drawEllipse(R0, 10, 10);
        }
        if (A[i][j] == -1){
            p.drawEllipse(a[i][j], 10, 10);
        }
        if (B[i][j] == -1){
            p.drawEllipse(b[i][j], 10, 10);
        }
        if (C[i][j] == -1){
            p.drawEllipse(c[i][j], 10, 10);
        }
        if (D[i][j] == -1){
            p.drawEllipse(d[i][j], 10, 10);
        }
        if (E[i][j] == -1){
            p.drawEllipse(e[i][j], 10, 10);
        }
        if (F[i][j] == -1){
            p.drawEllipse(f[i][j], 10, 10);
        }
    }
}
}
