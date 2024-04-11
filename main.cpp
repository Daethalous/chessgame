#include "chessboard.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller::getInstance()->init();
    return a.exec();
}
