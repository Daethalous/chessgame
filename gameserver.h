#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QtNetwork>
#include <QVector>

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);

    void initServer();

public slots:
    void acceptConnection();
    void recvMessage();

private:
    QVector<QTcpSocket*> playerSockets;
    QTcpServer  *listenSocket;
};

#endif // GAMESERVER_H
