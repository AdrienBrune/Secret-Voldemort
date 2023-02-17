#ifndef C_SERVERTCP_H
#define C_SERVERTCP_H

#include "../../Shared/Remote/c_message.h"
#include "../../Shared/Remote/C_TcpSocketAck.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class C_ServerTcp : public QObject
{
    Q_OBJECT

public:
    C_ServerTcp(QObject *parent = nullptr);
    ~C_ServerTcp();

signals:
    void sig_addPlayer(C_TcpSocketAck *socket);
    void sig_removePlayer(C_TcpSocketAck *socket);
    void sig_MessageReceived(C_Message *message);

private slots:
    void onNewConnection();
    void onServerSocketError(QAbstractSocket::SocketError socketError);
    void onClientDisconnected();
    void onClientSocketEvent(QAbstractSocket::SocketState socketState);
    void onClientSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer *mServer;
};

#endif // C_SERVERTCP_H
