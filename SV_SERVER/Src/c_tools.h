#ifndef C_TOOLS_H
#define C_TOOLS_H

#include "../../Shared/Components/c_player.h"

struct S_MINISTER_TO_RESTORE
{
    quint8 index = 0;
    bool active = false;

};

extern S_MINISTER_TO_RESTORE ministerToRestaure;

class C_Tools
{
public:
    C_Tools(){};

public:
    static void setRoles(QList<C_Player*> *playerList);
    static void setNextPositions(QList<C_Player*> *playerList, C_Player *nextMinister = nullptr);
    static void removePositionsExceptMinister(QList<C_Player*> *playerList);
    static void removeVote(QList<C_Player*> *playerList);
    static void removeFlagVote(QList<C_Player*> *playerList);
    static void removeFocusOnPlayers(QList<C_Player*>* playerList);
    static void removeActionRequested(QList<C_Player*>* playerList);
    static C_Player *getPlayer(QList<C_Player*> *playerList, C_Player::E_POSITION position);
    static QList<C_Player*> getPlayerPlaying(QList<C_Player*> *playerList);
    static QList<C_Player*> getPlayerAlive(QList<C_Player*> *playerList);
    static C_Player *getFocusedPlayer(QList<C_Player*> *playerList);
};

#endif // C_TOOLS_H
