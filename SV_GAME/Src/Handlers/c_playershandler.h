#ifndef C_PLAYERSHANDLER_H
#define C_PLAYERSHANDLER_H

#include <QWidget>

#include "Widgets/Components/w_player.h"

/* Singleton implementation */
class C_PlayersHandler : public QWidget
{
    Q_OBJECT

protected:
    C_PlayersHandler(QWidget *parent = nullptr);
public:
    C_PlayersHandler(C_PlayersHandler &other) = delete;
    void operator=(const C_PlayersHandler &) = delete;
    ~C_PlayersHandler();

signals:
    void sig_playerListUpdated();

public:
    static C_PlayersHandler *getInstance();

public:
    QList<C_Player*> *getPlayers(){ return &mPlayers; };
    void setPlayers(const QList<C_Player*>& players)
    {
        while(!mPlayers.isEmpty())
            delete mPlayers.takeLast();
        mPlayers.append(players);
    };

    const quint8 &getMyIdentifier()const{ return mMyIdentifier; };
    void setMyIdentifier(const quint8 &identifier){ mMyIdentifier = identifier; };

public:
    C_Player *addPlayerInList(C_Player *player = nullptr);
    void removePlayerInList(C_Player *player = nullptr);
    void clearPlayerList();

public:
    C_Player *getMyPlayerInstance()const;
    C_Player *getMinister()const;
    C_Player *getDirector()const;
    C_Player *getExMinister()const;
    C_Player *getExDirector()const;
    C_Player *getPlayerFocus()const;
    QList<C_Player*> getDeathEaterTeam()const;
    quint8 getPlayerIdentifier(C_Player *player)const;

protected:
    static C_PlayersHandler *mPlayersHandler;

private:
    QList<C_Player*> mPlayers;
    quint8 mMyIdentifier;
};

#endif // C_PLAYERSHANDLER_H
