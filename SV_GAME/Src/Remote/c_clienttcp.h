#ifndef C_CLIENTTCP_H
#define C_CLIENTTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QList>

#include "../../Shared/Remote/c_message.h"
#include "../../Shared/Components/c_player.h"

#define DEFAULT_PORT    30000
#define DEFAULT_IP      "127.0.0.1"

class C_ClientTcp : public QObject
{
    Q_OBJECT

public:
    C_ClientTcp(QObject *parent = nullptr);
    ~C_ClientTcp();

signals :
    void sig_MessageReceived(C_Message *message);
    void sig_connected();
    void sig_disconnected();
    void sig_updateView();

public:
    const QString &getClientIP()const{ return mIP; };
    void setClientIP(const QString &ip){ mIP = ip; };

    const quint16 &getClientPort()const{ return mPort; };
    void setClientPort(const quint16 &port){ mPort = port; };

    const QString &getName()const{ return mName; };
    void setName(const QString &name){ mName = name; };

    const bool &isConnected()const{ return mConnected; };

public slots:
    void onConnect();
    void onDisconnect();
    void onSetIP(const QString &ip){ mIP = ip; };
    void onSetPort(const QString port){ mPort = static_cast<quint16>(port.toInt()); };
    void onSetName(const QString name){ mName = name; };

private slots :
    void onConnected();
    void onDisconnected();
    void onSocketEvent(QAbstractSocket::SocketState socketState);
    void onSocketError(QAbstractSocket::SocketError socketError);

public:
    void sendMessage(C_Message *message);

private :
    QString mIP;
    quint16 mPort;
    QString mName;
    bool mConnected;
    C_TcpSocketAck *mClientSocket;
};

#endif // C_CLIENTTCP_H
