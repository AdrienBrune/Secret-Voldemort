#include "Remote/c_clienttcp.h"
#include "c_guiinterface.h"
#include "Handlers/c_boardhandler.h"
#include "../../Shared/Remote/C_TcpSocketAck.h"
#include "c_json.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>

C_ClientTcp::C_ClientTcp(QObject *parent)
    : QObject(parent)
    , mIP(DEFAULT_IP)
    , mPort(DEFAULT_PORT)
    , mConnected(false)
    , mClientSocket(nullptr)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    mClientSocket = new C_TcpSocketAck(this, socket);
    connect(mClientSocket, &C_TcpSocketAck::sig_clientConnected, this, &C_ClientTcp::onConnected);
    connect(mClientSocket, &C_TcpSocketAck::sig_messageReceived, this, &C_ClientTcp::sig_MessageReceived);
    connect(mClientSocket, &C_TcpSocketAck::sig_clientSocketEvent, this, &C_ClientTcp::onSocketEvent);
    connect(mClientSocket, &C_TcpSocketAck::sig_clientSocketError, this, &C_ClientTcp::onSocketError);
    connect(mClientSocket, &C_TcpSocketAck::sig_clientDisconnected, this, &C_ClientTcp::onDisconnected);
    connect(mClientSocket, &C_TcpSocketAck::sig_updateView, this, &C_ClientTcp::sig_updateView);

    double port = DEFAULT_PORT;
    C_Json::loadParameter("Name", &mName);
    C_Json::loadParameter("IP", &mIP);
    C_Json::loadParameter("Port", &port);
    mPort = port;

    onConnect();
}

C_ClientTcp::~C_ClientTcp()
{
    C_Json::saveParameter("Name", mName);
    C_Json::saveParameter("IP", mIP);
    C_Json::saveParameter("Port", static_cast<double>(mPort));
}

void C_ClientTcp::onConnect()
{
    qDebug() << "Client connection attempt to " << mIP << ":" << mPort;
    LOG_DBG(QString("Client connection attempt to %1:%2").arg(mIP).arg(mPort));

    onDisconnect();

    mClientSocket->connectToHost(mIP, mPort);
}

void C_ClientTcp::onConnected()
{
    mConnected = true;
    emit sig_connected();
}

void C_ClientTcp::onDisconnect()
{
    if(mClientSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "Client disconnection attempt from " << mIP << ":" << mPort;
        LOG_DBG(QString("Client disconnection attempt from %1:%2").arg(mIP).arg(mPort));
        mClientSocket->close();
    }
}

void C_ClientTcp::onDisconnected()
{
    mConnected = false;
    emit sig_disconnected();
}

void C_ClientTcp::onSocketEvent(QAbstractSocket::SocketState socketState)
{
    QStringList strStates = {
        "UnconnectedState",
        "HostLookupState",
        "ConnectingState",
        "ConnectedState",
        "BoundState",
        "ListeningState",
        "ClosingState"
    };

    qDebug() << QString("QAbstractSocket::SocketError::%1").arg(strStates[socketState]);
    LOG_DBG(QString("QAbstractSocket::SocketState::%1").arg(strStates[socketState]));
}

void C_ClientTcp::onSocketError(QAbstractSocket::SocketError socketError)
{
    QStringList strErrors = {
        "ConnectionRefusedError",
        "RemoteHostClosedError",
        "HostNotFoundError",
        "SocketAccessError",
        "SocketResourceError",
        "SocketTimeoutError",
        "DatagramTooLargeError",
        "NetworkError",
        "AddressInUseError",
        "SocketAddressNotAvailableError",
        "UnsupportedSocketOperationError",
        "UnfinishedSocketOperationError",
        "ProxyAuthenticationRequiredError",
        "SslHandshakeFailedError",
        "ProxyConnectionRefusedError",
        "ProxyConnectionClosedError",
        "ProxyConnectionTimeoutError",
        "ProxyNotFoundError",
        "ProxyProtocolError",
        "OperationError",
        "SslInternalError",
        "SslInvalidUserDataError",
        "TemporaryError"
    };

    if(socketError == QAbstractSocket::SocketError::UnknownSocketError)
    {
        qDebug() << "QAbstractSocket::SocketError::UnknownSocketError";
        LOG_DBG(QString("QAbstractSocket::SocketError::UnknownSocketError"));
        return;
    }

    qDebug() << QString("QAbstractSocket::SocketError::%1").arg(strErrors[socketError]);
    LOG_DBG(QString("QAbstractSocket::SocketError::%1").arg(strErrors[socketError]));
}

void C_ClientTcp::sendMessage(C_Message *message)
{
    mClientSocket->loadMessage(message);
}
