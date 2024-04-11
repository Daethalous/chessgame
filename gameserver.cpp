#include "gameserver.h"
#include <QMessageBox>

GameServer::GameServer(QObject *parent)
    : QObject{parent}
{
}

void GameServer::initServer()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,14514);

    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    QString servIP;
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            servIP = address.toString();
    }
    QString servInfo = "Connect via->"+ servIP+":";
    servInfo+=QString::number(this->listenSocket->serverPort(),10);
    QMessageBox::information(nullptr, "ip confirm", servInfo);

    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void GameServer::acceptConnection()
{
    QMessageBox::information(nullptr, "connected","connected");
    auto socket = this->listenSocket->nextPendingConnection();
    this->playerSockets.push_back(socket);
    connect(socket, &QTcpSocket::readyRead, this, &GameServer::recvMessage);
}

void GameServer::recvMessage()
{
    auto socket = (QTcpSocket*)sender();
    QByteArray data = socket->readAll();
    //QMessageBox::information(nullptr, "recv", data);
    for(auto& s : playerSockets)
        s->write(data);
}
