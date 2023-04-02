#ifndef C_CONTROLLER_H
#define C_CONTROLLER_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QFile>

#include "Handlers/c_remoteinterfacehandler.h"
#include "Handlers/c_boardhandler.h"
#include "Handlers/c_playershandler.h"
#include "Handlers/c_gamehandler.h"
#include "Handlers/c_soundhandler.h"
#include "c_guiinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class C_Controller; }
QT_END_NAMESPACE

class C_Controller : public QMainWindow
{
    Q_OBJECT

public:
    C_Controller(QWidget *parent = nullptr);
    ~C_Controller();

public:
    /* Events triggered from server remote messages */
    void Event_DirectorSelection(QByteArray data = nullptr);
    void Event_DirectorElection(QByteArray data = nullptr);
    void Event_MinisterDraw(QByteArray data = nullptr);
    void Event_MinisterDiscard(QByteArray data = nullptr);
    void Event_DirectorDiscard(QByteArray data = nullptr);
    void Event_DirectorAskedVeto(QByteArray data = nullptr);
    void Event_PowerSubstituteMinister(QByteArray data = nullptr);
    void Event_PowerClairvoyance(QByteArray data = nullptr);
    void Event_PowerSpying(QByteArray data = nullptr);
    void Event_PowerAssassination(QByteArray data = nullptr);
    void EVENT_PowerPlayerAssassinated(QByteArray data = nullptr);
    void EVENT_EndGame(QByteArray data = nullptr);

public slots:
    void onEvent(C_Message_Event::E_EVENT, QByteArray data = nullptr);
    void onNotify(QString string);
    /* Events triggered from board interactions */
    void onPlayerClicked(C_Player* player);
    void onDrawLawCards();
    void onDiscardLawCard(C_LawCard::E_FACTION law);
    void onAskVeto();
    void onClairvoyancePowerUsed();
    void onSpyingPowerUsed();
    void onVote(W_VoteCard::E_VOTE vote);
    void onConnected();
    void onDisconnected();
    void onQuitGame(){ close(); };

private:
    /* MUST folow the C_Message_Event::E_EVENT enumeration - nullptr is not implemented */
    void (C_Controller::*eventFunctions[C_Message_Event::E_EVENT::event_number])(QByteArray data) = {
            nullptr,
            &C_Controller::Event_DirectorSelection,
            nullptr,
            &C_Controller::Event_DirectorElection,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            &C_Controller::Event_MinisterDraw,
            nullptr,
            &C_Controller::Event_MinisterDiscard,
            nullptr,
            &C_Controller::Event_DirectorDiscard,
            nullptr,
            nullptr,
            &C_Controller::Event_DirectorAskedVeto,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            &C_Controller::Event_PowerSubstituteMinister,
            nullptr,
            &C_Controller::Event_PowerClairvoyance,
            nullptr,
            &C_Controller::Event_PowerSpying,
            nullptr,
            nullptr,
            &C_Controller::Event_PowerAssassination,
            nullptr,
            &C_Controller::EVENT_PowerPlayerAssassinated,
            nullptr,
            nullptr,
            &C_Controller::EVENT_EndGame,
            nullptr
    };

private slots:
    void onLoopConnection();

protected:
    void keyPressEvent(QKeyEvent *event)override;

private:
    C_RemoteInterfaceHandler *mRemoteInterface;
    C_BoardHandler *mBoard;
    C_PlayersHandler *mPlayers;
    C_GameHandler *mGame;
    C_SoundHandler *mSound;
    C_GuiInterface *mView;

private:
    Ui::C_Controller *ui;
};

#endif // C_CONTROLLER_H
