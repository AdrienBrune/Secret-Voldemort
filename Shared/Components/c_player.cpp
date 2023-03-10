#include "../../Shared/Components/c_player.h"
#include "../../Shared/Remote/c_tcpsocketack.h"

C_Player::C_Player(QWidget *parent, C_TcpSocketAck *socket)
    : QWidget(parent)
    , mName("Joueur")
    , mRole(E_ROLE::notAttributed)
    , mStatus(E_STATUS::unconnected)
    , mPosition(E_POSITION::none)
    , mFlagVote(false)
    , mFlagFocus(false)
    , mSocket(socket)
{
    if(socket)
    {
        connect(socket, &C_TcpSocketAck::sig_messageReceived, this, &C_Player::onMessageReceived);
    }
    hide();
}

C_Player::~C_Player()
{
    if(mSocket)
        delete mSocket;
}

void C_Player::onMessageReceived(C_Message *message)
{
    emit sig_messageReceived(message, this);
}

void C_Player::sendMessage(C_Message *message)
{
    mSocket->loadMessage(message);
}
