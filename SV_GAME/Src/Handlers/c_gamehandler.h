#ifndef C_GAMEHANDLER_H
#define C_GAMEHANDLER_H

#include <QWidget>

#include "Widgets/Components/w_gametracker.h"
#include "Widgets/Components/w_electiontracker.h"

/* Singleton implementation */
class C_GameHandler : public QWidget
{
    Q_OBJECT

protected:
    C_GameHandler(QWidget *parent = nullptr);
public:
    C_GameHandler(C_GameHandler &other) = delete;
    void operator=(const C_GameHandler &) = delete;
    ~C_GameHandler();

public:
    static C_GameHandler *getInstance();

protected:
    static C_GameHandler *mGameHandler;

public:
    C_ElectionTracker *getElectionTracker()const{ return mElectionTracker; };
    void setElectionTracker(C_ElectionTracker *electionTracker){ mElectionTracker = electionTracker; };
    C_GameTracker *getGameTracker()const{ return mGameTracker; };
    void setGameTracker(C_GameTracker *gameTracker){ mGameTracker = gameTracker; };

private:
    C_ElectionTracker *mElectionTracker;
    C_GameTracker *mGameTracker;
};

#endif // C_GAMEHANDLER_H
