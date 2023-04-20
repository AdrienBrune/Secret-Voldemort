#include "Handlers/c_playershandler.h"
#include "../../Shared/debug.h"

C_PlayersHandler *C_PlayersHandler::mPlayersHandler = nullptr;

C_PlayersHandler::C_PlayersHandler(QWidget *parent)
    : QWidget(parent)
    , mMyIdentifier(0xFF)
{
    mPlayersHandler = this;
}

C_PlayersHandler::~C_PlayersHandler()
{
    while(!mPlayers.isEmpty())
        delete mPlayers.takeLast();
}

C_PlayersHandler *C_PlayersHandler::getInstance()
{
    if(!mPlayersHandler)
        return mPlayersHandler = new C_PlayersHandler();
    else
        return mPlayersHandler;
}

C_Player *C_PlayersHandler::addPlayerInList(C_Player *player)
{
    if(player)
    {
        mPlayers.append(player);
    }
    else
    {
        mPlayers.append(new W_Player(this));
    }
    emit sig_playerListUpdated();
    return mPlayers.last();
}

void C_PlayersHandler::removePlayerInList(C_Player *player)
{
    if(mPlayers.isEmpty())
    {
        LOG_DBG("Error player list is empty");
        return;
    }

    if(player)
    {
        for(int i=0; i < mPlayers.size(); i++)
        {
            if(player == mPlayers[i])
            {
                delete mPlayers.takeAt(i);
                break;
            }
        }
    }
    else
    {
        delete mPlayers.takeLast();
    }
    emit sig_playerListUpdated();
}

void C_PlayersHandler::clearPlayerList()
{
    while(!mPlayers.isEmpty())
        removePlayerInList();
}

C_Player *C_PlayersHandler::getMyPlayerInstance()const
{
    if(mPlayers.size() > mMyIdentifier)
    {
        return mPlayers[mMyIdentifier];
    }
    else
        return nullptr;
}

C_Player *C_PlayersHandler::getMinister()const
{
    for(C_Player *player : mPlayers)
    {
        if(player->getPosition() == C_Player::E_POSITION::Minister)
            return player;
    }

    return nullptr;
}

C_Player *C_PlayersHandler::getDirector()const
{
    for(C_Player *player : mPlayers)
    {
        if(player->getPosition() == C_Player::E_POSITION::Director)
            return player;
    }

    return nullptr;
}

C_Player *C_PlayersHandler::getExMinister()const
{
    for(C_Player *player : mPlayers)
    {
        if(player->getPosition() == C_Player::E_POSITION::exMinister)
            return player;
    }

    return nullptr;
}

C_Player *C_PlayersHandler::getExDirector()const
{
    for(C_Player *player : mPlayers)
    {
        if(player->getPosition() == C_Player::E_POSITION::exDirector)
            return player;
    }

    return nullptr;
}

C_Player *C_PlayersHandler::getPlayerFocus() const
{
    for(C_Player *player : mPlayers)
    {
        if(player->getFlagFocus())
            return player;
    }

    return nullptr;
}

QList<C_Player *> C_PlayersHandler::getDeathEaterTeam()const
{
    QList<C_Player*> deathEaterTeam;

    for(C_Player *player : mPlayers)
    {
        if(IS_DEATHEATER(player))
            deathEaterTeam.append(player);
    }

    return deathEaterTeam;
}

quint8 C_PlayersHandler::getPlayerIdentifier(C_Player *player)const
{
    QList<C_Player*> deathEaterTeam;

    for(int i = 0; i < mPlayers.size(); i++)
    {
        if(mPlayers[i] == player)
            return i;
    }

    return 0xFF;
}

quint8 C_PlayersHandler::getNumberPlayerPlaying()const
{
    quint8 numberPlayers = 0;
    for(C_Player *player : mPlayers)
    {
        if(player->isPlaying())
            numberPlayers++;
    }
    return numberPlayers;
}
