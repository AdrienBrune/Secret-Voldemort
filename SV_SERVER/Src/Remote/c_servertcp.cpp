#include "c_servertcp.h"
#include "../../Shared/debug.h"
#include "../../Shared/Remote/serialization.h"
#include "../../Shared/Remote/c_tcpsocketack.h"

C_ServerTcp::C_ServerTcp(QObject *parent)
    : QObject(parent)
    , mServer(nullptr)
{
    mServer = new QTcpServer(this);
    connect(mServer, &QTcpServer::acceptError, this, &C_ServerTcp::onServerSocketError);
    connect(mServer, &QTcpServer::newConnection, this, &C_ServerTcp::onNewConnection);

    mServer->listen(QHostAddress::Any, static_cast<quint16>(30000));
}

C_ServerTcp::~C_ServerTcp()
{

}

void C_ServerTcp::onNewConnection()
{
    QTcpSocket* socket = mServer->nextPendingConnection();
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    C_TcpSocketAck *clientSocket = new C_TcpSocketAck(this, socket);
    connect(clientSocket, &C_TcpSocketAck::sig_clientSocketEvent, this, &C_ServerTcp::onClientSocketEvent);
    connect(clientSocket, &C_TcpSocketAck::sig_clientSocketError, this, &C_ServerTcp::onClientSocketError);
    connect(clientSocket, &C_TcpSocketAck::sig_clientDisconnected, this, &C_ServerTcp::onClientDisconnected);

    emit sig_addPlayer(clientSocket);
}

void C_ServerTcp::onServerSocketError(QAbstractSocket::SocketError socketError)
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

void C_ServerTcp::onClientDisconnected()
{
    qDebug() << "Client déconnecté";
    emit sig_removePlayer(dynamic_cast<C_TcpSocketAck*>(sender()));
}

void C_ServerTcp::onClientSocketEvent(QAbstractSocket::SocketState socketState)
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

void C_ServerTcp::onClientSocketError(QAbstractSocket::SocketError socketError)
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

