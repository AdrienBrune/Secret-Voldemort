#include "c_tools.h"

#include <QRandomGenerator>

S_MINISTER_TO_RESTORE ministerToRestaure;

void C_Tools::setRoles(QList<C_Player*> *playerList)
{
#define GET_ROLE_PHENIXORDER teamPhenixOrder.takeAt(QRandomGenerator().global()->bounded(teamPhenixOrder.size()))
#define GET_ROLE_DEATHEATER  teamDeathEater.takeAt(QRandomGenerator().global()->bounded(teamDeathEater.size()))
    QList<C_Player::E_ROLE> teamPhenixOrder, teamDeathEater;
    teamPhenixOrder << C_Player::E_ROLE::Harry << C_Player::E_ROLE::Dumbledor << C_Player::E_ROLE::Hermione
                    << C_Player::E_ROLE::Neville << C_Player::E_ROLE::Ron << C_Player::E_ROLE::Sirius;
    teamDeathEater  << C_Player::E_ROLE::Beatrix << C_Player::E_ROLE::Drago << C_Player::E_ROLE::Lucius;

    QList<C_Player*> players;
    for(C_Player *player : qAsConst(*playerList))
        players.append(player);

    switch(playerList->size()-1)
    {
    case 9:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_PHENIXORDER);
    case 8:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_DEATHEATER);
    case 7:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_PHENIXORDER);
    case 6:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_DEATHEATER);
    case 5:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_PHENIXORDER);
    case 4:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_PHENIXORDER);
    case 3:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_PHENIXORDER);
    case 2:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_DEATHEATER);
    case 1:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(GET_ROLE_PHENIXORDER);
    case 0:
        players.takeAt(QRandomGenerator().global()->bounded(players.size()))->setRole(C_Player::E_ROLE::Voldemort);
        break;
    }
}

void C_Tools::setNextPositions(QList<C_Player*> *playerList, C_Player *nextMinister)
{
    quint8 indexMinister = 0;

    for(int i = 0; i < playerList->size(); i++)
    {
        switch((*playerList)[i]->getPosition())
        {
        case C_Player::E_POSITION::Minister:
            if(nextMinister) // Player selected as new Minister
            {
                // Save Minister to come back to
                ministerToRestaure.index = i;
                ministerToRestaure.active = true;
            }
            else // Next Minister selection
            {
                if(ministerToRestaure.active)
                {
                    ministerToRestaure.active = false;
                    // Go back to previous minister before substitute Minister power
                    indexMinister = ministerToRestaure.index;
                    do{
                        if(++indexMinister >= playerList->size())
                            indexMinister = 0;
                    }while((*playerList)[indexMinister]->getStatus() != C_Player::E_STATUS::playing);
                }
                else
                {
                    // Regular Minister selection
                    indexMinister = i;
                    do{
                        if(++indexMinister >= playerList->size())
                            indexMinister = 0;
                    }while((*playerList)[indexMinister]->getStatus() != C_Player::E_STATUS::playing);
                }
            }
            (*playerList)[i]->setPosition(C_Player::E_POSITION::none);
            break;

        case C_Player::E_POSITION::Director:
            (*playerList)[i]->setPosition(C_Player::E_POSITION::none);
            break;

        default:
            break;
        }
    }

    if(nextMinister)
        nextMinister->setPosition(C_Player::E_POSITION::Minister);
    else
        (*playerList)[indexMinister]->setPosition(C_Player::E_POSITION::Minister);
}

void C_Tools::removePositionsExceptMinister(QList<C_Player*> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        if((*playerList)[i]->getPosition() != C_Player::E_POSITION::Minister)
            (*playerList)[i]->setPosition(C_Player::E_POSITION::none);
    }
}

void C_Tools::removeVote(QList<C_Player*> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        (*playerList)[i]->setVote(C_Player::E_VOTE::noVote);
    }
}

void C_Tools::removeFlagVote(QList<C_Player*> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        (*playerList)[i]->setFlagVote(false);
    }
}

void C_Tools::removeFocusOnPlayers(QList<C_Player*> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        (*playerList)[i]->setFlagFocus(false);
    }
}

void C_Tools::removeActionRequested(QList<C_Player *> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        (*playerList)[i]->setActionRequested(false);
    }
}

void C_Tools::resetPlayersEligibility(QList<C_Player *> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        (*playerList)[i]->setEligibility(true);
    }
}

void C_Tools::updatePlayersEligibility(QList<C_Player *> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        if((*playerList)[i]->getPosition() == C_Player::E_POSITION::Director)
        {
            (*playerList)[i]->setEligibility(false);
        }
        else if((*playerList)[i]->getPosition() == C_Player::E_POSITION::Minister && C_Tools::getPlayerPlaying(playerList).size() > 5)
        {
            (*playerList)[i]->setEligibility(false);
        }
        else
        {
            (*playerList)[i]->setEligibility(true);
        }
    }
}

C_Player *C_Tools::getPlayer(QList<C_Player*> *playerList, C_Player::E_POSITION position)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        if((*playerList)[i]->getPosition() == position)
            return (*playerList)[i];
    }
    return nullptr;
}

QList<C_Player*> C_Tools::getPlayerPlaying(QList<C_Player*> *playerList)
{
    QList<C_Player*> players;
    for(int i = 0; i < playerList->size(); i++)
    {
        if((*playerList)[i]->isPlaying())
        {
            players.append((*playerList)[i]);
        }
    }
    return players;
}

QList<C_Player*> C_Tools::getPlayerAlive(QList<C_Player*> *playerList)
{
    QList<C_Player*> players;
    for(int i = 0; i < playerList->size(); i++)
    {
        if((*playerList)[i]->getStatus() == C_Player::E_STATUS::playing)
        {
            players.append((*playerList)[i]);
        }
    }
    return players;
}

C_Player *C_Tools::getFocusedPlayer(QList<C_Player*> *playerList)
{
    for(int i = 0; i < playerList->size(); i++)
    {
        if((*playerList)[i]->getFlagFocus())
            return (*playerList)[i];
    }
    return nullptr;
}
