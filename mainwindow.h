#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int W, H;
    QPointF R0;
    QPointF a[4][5], b[4][5], c[4][5], d[4][5], e[4][5], f[4][5];
    int r0, A[4][5], B[4][5], C[4][5], D[4][5], E[4][5], F[4][5];

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
};
#endif // MAINWINDOW_H
