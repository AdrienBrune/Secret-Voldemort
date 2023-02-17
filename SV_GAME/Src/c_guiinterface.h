#ifndef C_GUIINTERFACE_H
#define C_GUIINTERFACE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QResizeEvent>

#include "Widgets/Components/w_gametracker.h"
#include "Widgets/Components/w_electiontracker.h"
#include "Widgets/Components/w_player.h"
#include "Widgets/Components/w_lawboard.h"
#include "Widgets/Components/w_stack.h"
#include "Widgets/Components/w_easel.h"
#include "Widgets/Components/w_messagedisplayer.h"
#include "Widgets/w_playersmanagement.h"
#include "Widgets/w_screenmenu.h"
#include "Widgets/w_secretrole.h"
#include "Widgets/w_screenvote.h"
#include "Widgets/w_screenlaws.h"
#include "Widgets/w_screenspying.h"
#include "Widgets/w_screenpowerunlocked.h"

namespace Ui{
    class C_GuiInterface;
}

/* Singleton implementation */
class C_GuiInterface : public QWidget
{
    Q_OBJECT

public:
    explicit C_GuiInterface(QWidget *parent = nullptr);
    ~C_GuiInterface();

signals:
    void sig_playerClicked(C_Player*);
    void sig_draw();
    void sig_vote(W_VoteCard::E_VOTE);
    void sig_discard(C_LawCard::E_FACTION);
    void sig_askVeto();
    void sig_PowerReady(C_LawBoard::E_POWER);
    void sig_clairvoyancePowerUsed();
    void sig_spyingPowerUsed();
    void sig_quitGame();

public slots:
    void onUpdateView();

public slots:
    void onQuitScreenMenu();
    void onQuitScreenVote(W_VoteCard::E_VOTE vote);
    void onQuitScreenLaws(C_LawCard::E_FACTION faction);
    void onAskVeto();
    void onQuitScreenSpying();
    void onQuitScreenPower(C_LawBoard::E_POWER power);
    void onOpenSettings();

protected:
    void paintEvent(QPaintEvent *)override;
    void resizeEvent(QResizeEvent *)override;

public:
    C_Stack *getStack()const{ return wStack; };
    C_LawBoard *getDeathEaterBoard()const{ return wDeathEaterBoard; };
    C_LawBoard *getPhenixOrderBoard()const{ return wPhenixOrderBoard; };
    W_Easel *getEasel()const{ return wEasel; };
    C_ElectionTracker *getElectionTracker()const{ return wElectionTracker; };
    W_GameTracker *getGameTracker()const{ return wGameTracker; };
    W_MessageDisplayer *getMessageLogger()const{ return wMessageLogger; };

    W_ScreenMenu *getMenu()const{ return wScreenMenu; };

public:
    void enablePlayersInteraction(bool toggle);
    void enableDrawing(bool toggle);
    void displayScreenMenu();
    void displayScreenVote();
    void displayScreenLaws(quint8 cardNumberToShow, bool readOnly = false);
    void displayScreenSpying(C_Player *player);
    void displayScreenPowerUnlocked(C_LawBoard::E_POWER power);
    void quitAllScreen();

protected:
    bool eventFilter(QObject *object, QEvent *event)override;

private:
    Ui::C_GuiInterface *ui;

private:
    /* Widgets composing main screen */
    W_Stack *wStack;
    W_LawBoard *wDeathEaterBoard;
    W_LawBoard *wPhenixOrderBoard;
    W_Easel *wEasel;
    W_MessageDisplayer *wMessageLogger;
    C_ElectionTracker *wElectionTracker;
    W_GameTracker *wGameTracker;
    W_PlayersManagement *wPlayersManagement;
    W_SecretRole *wSecretRole;

    /* Foreground reduced opacity screens */
    W_ScreenVote *wScreenVote;
    W_ScreenLaws *wScreenLaws;
    W_ScreenSpying *wScreenSpying;
    W_ScreenPowerUnlocked *wScreenPower;
    W_ScreenMenu *wScreenMenu;

    bool mEnableMenu;
};

#endif // C_GUIINTERFACE_H
