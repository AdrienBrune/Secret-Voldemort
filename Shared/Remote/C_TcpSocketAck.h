#ifndef C_TCPSOCKETACK_H
#define C_TCPSOCKETACK_H

#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>
#include "../../Shared/Remote/c_message.h"
#include "../../Shared/debug.h"

#define TIMEOUT_ACKNOWLEDGE 5000

class C_TcpSocketAck : public QObject
{
    Q_OBJECT

private:
    enum E_STEP{
        iddle,
        send,
        waitAck
    };

public:
    C_TcpSocketAck(QObject *parent, QTcpSocket *socket)
        : QObject(parent)
        , mSocket(socket)
        , mTimeoutAck(nullptr)
        , mMessage(nullptr)
        , mStep(iddle)
    {
        mTimeoutAck = new QTimer(this);
        mTimeoutAck->setSingleShot(true);
        connect(mTimeoutAck, &QTimer::timeout, this, &C_TcpSocketAck::onAckTimeout);
        connect(mSocket, &QTcpSocket::connected, this, &C_TcpSocketAck::sig_clientConnected);
        connect(mSocket, &QTcpSocket::readyRead, this, &C_TcpSocketAck::onMessageReceived);
        connect(mSocket, &QTcpSocket::stateChanged, this, &C_TcpSocketAck::sig_clientSocketEvent);
        connect(mSocket, &QTcpSocket::errorOccurred, this, &C_TcpSocketAck::sig_clientSocketError);
        connect(mSocket, &QTcpSocket::disconnected, this, &C_TcpSocketAck::sig_clientDisconnected);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &C_TcpSocketAck::onMachineState, Qt::ConnectionType::QueuedConnection);
        timer->start(10);
    }
    ~C_TcpSocketAck()
    {
        if(mMessage)
            delete mMessage;
        while(!mMessageQueue.isEmpty())
            delete mMessageQueue.takeLast();
    }

signals:
    void sig_clientConnected();
    void sig_clientDisconnected();
    void sig_clientSocketEvent(QAbstractSocket::SocketState);
    void sig_clientSocketError(QAbstractSocket::SocketError);
    void sig_messageReceived(C_Message *);
    void sig_updateView();

private slots:
    void onMachineState()
    {
        switch(mStep)
        {
        case iddle:
            if(!mMessageQueue.isEmpty()) // Message in queue ready to be sent
            {
                mStep = send;
            }
            break;

        case send:
            sendMessage(mMessageQueue.takeFirst()); // remove message from queue + Send it
            mStep = waitAck;
            break;

        case waitAck:
            if(!mTimeoutAck->isActive()) // Message sent + Ack received - Ready to send next message
            {
                mStep = iddle;
            }
            break;

        default:
            mStep = iddle;
            break;
        }
    }

    void onAckTimeout()
    {
        LOG_DBG("Acknowledge not received");
        /* Resend message loaded */
        if(mMessage)
        {
            LOG_DBG("Message re-sent");
            mTimeoutAck->start(TIMEOUT_ACKNOWLEDGE);

            QDataStream stream(mSocket);
            stream << *mMessage;
        }
    }

    void onMessageReceived()
    {
        while(mSocket->bytesAvailable())
        {
            QDataStream stream(mSocket);
            C_Message *msgHeader = new C_Message();
            C_Message *msgContent;

            stream >> *msgHeader;

            switch(msgHeader->getIdentifier())
            {
            case C_Message::E_ID::acknowledge:
                mTimeoutAck->stop();
                delete msgHeader;
                continue;

            case C_Message::E_ID::update_players:
                msgContent = new C_Message_UpdatePlayers(msgHeader);
                stream >> *msgContent;
                break;

            case C_Message::E_ID::update_board:
                msgContent = new C_Message_UpdateBoard(msgHeader);
                stream >> *msgContent;
                break;

            case C_Message::E_ID::update_game:
                msgContent = new C_Message_UpdateGame(msgHeader);
                stream >> *msgContent;
                break;

            case C_Message::E_ID::notification:
                msgContent = new C_Message_Notification(msgHeader);
                stream >> *msgContent;
                break;

            case C_Message::E_ID::event:
                msgContent = new C_Message_Event(msgHeader);
                stream >> *msgContent;
                break;

            default:
                LOG_DBG("Error desynchronization while receiving messages !");
                mSocket->readAll();
                delete msgHeader;
                continue;
            }
            /* Send acknowledge */
            C_Message_Acknowledge * ack = new C_Message_Acknowledge();
            stream << *ack;
            delete ack;

            /* Process message received */
            emit sig_messageReceived(msgContent);
            emit sig_updateView();
        }
    }

public:
    void loadMessage(C_Message *message)
    {
        mMessageQueue.append(message);
    }

    void connectToHost(QString address, quint16 port)
    {
        mSocket->connectToHost(QHostAddress(address), port);
    }

    void close()
    {
        mSocket->close();
    }
    QAbstractSocket::SocketState state()
    {
        return mSocket->state();
    }

private:
    void sendMessage(C_Message *message)
    {
        if(mMessage)
        {
            delete mMessage;
            mMessage = nullptr;
        }
        mMessage = message;
        mTimeoutAck->start(TIMEOUT_ACKNOWLEDGE);

        QDataStream stream(mSocket);
        stream << *message;
    }

private:
    QTcpSocket *mSocket;
    QTimer *mTimeoutAck;
    C_Message *mMessage;
    QList<C_Message*> mMessageQueue;
    quint8 mStep;
};

#endif // C_TCPSOCKETACK_H
