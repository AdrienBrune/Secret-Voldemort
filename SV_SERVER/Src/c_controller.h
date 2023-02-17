#ifndef C_CONTROLLER_H
#define C_CONTROLLER_H

#include <QMainWindow>
#include <QList>

#include "../../Shared/Remote/c_message.h"
#include "../../Shared/Components/c_electiontracker.h"
#include "../../Shared/Components/c_gametracker.h"
#include "../../Shared/Components/c_lawboard.h"
#include "../../Shared/Components/c_player.h"
#include "../../Shared/Components/c_stack.h"
#include "Remote/c_servertcp.h"
#include "c_tools.h"

class C_NextStep;

QT_BEGIN_NAMESPACE
namespace Ui { class C_Controller; }
QT_END_NAMESPACE

enum E_VOTE
{
    notDefined,
    lumos,
    nox
};

struct S_VOTE_RESULTS{
    quint8 lumos = 0;
    quint8 nox = 0;
    quint8 voteNumber = 0;
};

class C_Controller : public QMainWindow
{
    Q_OBJECT

public:
    enum E_ST
    {
        st_iddle,
        st_startGame,
        st_startTurn,
        st_directorElectionBegin,
        st_voteResult,
        st_voteVoldemortElected,
        st_voteResultNox,
        st_voteResultLumos,
        st_ministerDraw,
        st_ministerDiscard,

        st_directorDiscard,
        st_directorAskedVeto,
        st_ministerRefusedVeto,
        st_ministerAcceptedVeto,
        st_electionTrackerIncremented,
        st_ministryEnterChaos,
        st_lawOnTopOfThePileVoted,
        st_everyoneCouldBeNextDirector,

        st_lawVoted,

        st_triggerPowerAssassination,
        st_playerAssassinated,

        st_triggerPowerSpying,
        st_spyingInProgress,
        st_playerSpied,

        st_triggerPowerClairvoyance,
        st_ministerClairvoyanceCarriedOut,

        st_triggerPowerSubstituteMinister,
        st_nextMinisterSelected,

        st_endOfTurn,
        st_stackShuffled,

        st_gameFinished
    };

public:
    C_Controller(QWidget *parent = nullptr);
    ~C_Controller();

private slots:
    void onStartGame();
    void onAddPlayer(C_TcpSocketAck *client);
    void onRemovePlayer(C_TcpSocketAck *client);
    void onMessageReceived(C_Message *message, C_Player *player);

private:
    void onEvent(C_Message_Event::E_EVENT event, C_Player *player, const QByteArray &data);
private slots:
    void EVENT_DirectorSelected(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_PlayerVoted(C_Player *player = nullptr, const QByteArray &data = nullptr); // C_Player* ?
    void EVENT_MinisterDrew(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_MinisterDiscarded(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_DirectorDiscarded(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_DirectorAskedVeto(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_MinisterAcceptedVeto(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_MinisterRefusedVeto(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_PowerSubstituteMinisterSelected(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_PowerClairvoyanceUsed(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_PowerSpyingInProgress(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_PowerSpyingCarriedOut(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_PowerPlayerToAssassinSelected(C_Player *player = nullptr, const QByteArray &data = nullptr);
    void EVENT_SetPlayerName(C_Player *player = nullptr, const QByteArray &data = nullptr);

    void on_buttonRestartTurn_clicked();

private:
    void onMachineState();
    void addNextState(C_NextStep *nextStep);
    void clearStepQueue();

private:
    void updateGameState(QString notification, C_Message_Event::E_EVENT event = C_Message_Event::E_EVENT::ApplicationStarted, C_Player *player = nullptr);
    void sendUpdatedGameToPlayers();
    void sendNotification(QString notification, C_Player *player = nullptr);
    void sendEvent(C_Message_Event::E_EVENT event, C_Player *player = nullptr);
    void initGame();
    int checkForPower();
    void MinisterEnterChaos();
    void prepareCurrentTurn();
    void prepareNextTurn();
    E_VOTE voteResult();

private:
    /* MUST folow the C_Message_Event::E_EVENT enumeration - nullptr means not implemented */
    void (C_Controller::*eventFunctions[C_Message_Event::E_EVENT::event_number])(C_Player *, const QByteArray & data) = {
        nullptr,
        nullptr,
        &C_Controller::EVENT_DirectorSelected,
        nullptr,
        &C_Controller::EVENT_PlayerVoted,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &C_Controller::EVENT_MinisterDrew,
        nullptr,
        &C_Controller::EVENT_MinisterDiscarded,
        nullptr,
        &C_Controller::EVENT_DirectorDiscarded,
        &C_Controller::EVENT_DirectorAskedVeto,
        nullptr,
        &C_Controller::EVENT_MinisterAcceptedVeto,
        &C_Controller::EVENT_MinisterRefusedVeto,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &C_Controller::EVENT_PowerSubstituteMinisterSelected,
        nullptr,
        &C_Controller::EVENT_PowerClairvoyanceUsed,
        nullptr,
        &C_Controller::EVENT_PowerSpyingInProgress,
        &C_Controller::EVENT_PowerSpyingCarriedOut,
        nullptr,
        &C_Controller::EVENT_PowerPlayerToAssassinSelected,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &C_Controller::EVENT_SetPlayerName
    };

private:
    E_ST mMachineStep;
    QList<C_NextStep*> mStepQueue;

    C_Stack *mStack;
    C_LawBoard *mPhenixOrderBoard;
    C_LawBoard *mDeathEaterBoard;
    C_GameTracker *mGameTracker;
    C_ElectionTracker *mElectionTracker;
    QList<C_Player*> mPlayers;

    C_ServerTcp *mServer;
    QList<C_Player*> mPlayersSaved; // Disconnection-Reconnection backup
    QList<E_VOTE> mVoteStack;
    S_VOTE_RESULTS mVoteResults;
    C_Player *nextSubstituteMinister;

private:
    Ui::C_Controller *ui;
};

class C_NextStep : public QTimer
{
    Q_OBJECT
public:
    C_NextStep(C_Controller::E_ST step, quint32 delay = 0)
        : mNextStep(step)
    {
        setSingleShot(true);
        setInterval(delay);
    };
    ~C_NextStep(){};
public:
    const C_Controller::E_ST &getNextStep()const{ return mNextStep; };
public:
    C_Controller::E_ST mNextStep;
};


#endif // C_CONTROLLER_H
