#ifndef C_PLAYER_H
#define C_PLAYER_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>

class C_TcpSocketAck;
class C_Message;

#define IS_DEATHEATER(player)   (player->getRole() > C_Player::E_ROLE::Dumbledor ? true : false)
#define IS_PHENIXORDER(player)  (player->getRole() <= C_Player::E_ROLE::Dumbledor ? true : false)
#define IS_VOLDEMORT(player)    (player->getRole() == C_Player::E_ROLE::Voldemort ? true : false)

class C_Player : public QWidget
{
    Q_OBJECT

public:
    enum E_STATUS
    {
        unconnected = 0,
        connected,
        playing,
        dead
    };

    enum E_ROLE
    {
        notAttributed = 0,
        Harry,
        Hermione,
        Ron,
        Sirius,
        Neville,
        Dumbledor,
        Drago,
        Beatrix,
        Lucius,
        Voldemort,
        numberOfRoles
    };

    enum E_POSITION{
        none = 0,
        exMinister,
        exDirector,
        Minister,
        Director
    };

public:
    C_Player(QWidget *parent = nullptr, C_TcpSocketAck *socket = nullptr);
    ~C_Player();

signals:
    void sig_messageReceived(C_Message *, C_Player *);

private slots:
    void onMessageReceived(C_Message *message);

public:
    const QString &getName()const{ return mName; };
    void setName(const QString &name){ mName = name; };

    const E_ROLE &getRole()const{ return mRole; };
    void setRole(const E_ROLE &role){ mRole = role; };

    const E_STATUS &getStatus()const{ return mStatus; };
    void setStatus(const E_STATUS &status){ mStatus = status; };

    const E_POSITION &getPosition()const{ return mPosition; };
    void setPosition(const E_POSITION &position){ mPosition = position; };

    const bool &getFlagVote()const{ return mFlagVote; };
    void setFlagVote(const bool &flag){ mFlagVote = flag; };

    const bool &getFlagFocus()const{ return mFlagFocus; };
    void setFlagFocus(const bool &flag){ mFlagFocus = flag; };

    C_TcpSocketAck *getSocket()const{ return mSocket; };
    void setSocket(C_TcpSocketAck *socket){ mSocket = socket; };

public:
    void sendMessage(C_Message *message);

protected:
    QString mName;
    E_ROLE mRole;
    E_STATUS mStatus;
    E_POSITION mPosition;
    bool mFlagVote;
    bool mFlagFocus;
    C_TcpSocketAck *mSocket;
};

#endif // C_PLAYER_H
