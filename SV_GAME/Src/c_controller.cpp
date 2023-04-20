#include "c_controller.h"
#include "ui_c_controller.h"
#include "../../Shared/Remote/serialization.h"
#include "Widgets/w_messagebox.h"

#include <QFontDatabase>

//#define LOOP_CONNECTION

C_Controller::C_Controller(QWidget *parent)
    : QMainWindow(parent)
    , mRemoteInterface(nullptr)
    , mBoard(nullptr)
    , mPlayers(nullptr)
    , mGame(nullptr)
    , mSound(nullptr)
    , mView(nullptr)
    , ui(new Ui::C_Controller)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    QFontDatabase::addApplicationFont(":/fonts/font_germania.ttf");
    QFontDatabase::addApplicationFont(":/fonts/font_harrypotter.ttf");

    QCursor cursor(QPixmap(":/images/custom_cursor.png"), 20, 20);
    setCursor(cursor);

    /* Config file creation if not found */
#ifdef __APPLE__
    QFile file("../config.json");
#else
    QFile file("config.json");
#endif
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_DBG("Config file not found, file initialization ...");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("{\n"
                        "\t\"Enable music\": true,\n"
                        "\t\"Enable sounds\": true,\n"
                        "\t\"IP\": \"127.0.0.1\",\n"
                        "\t\"Name\": \"MyName\",\n"
                        "\t\"Port\": 30000,\n"
                        "\t\"Volume\": 100\n"
                    "}");
        file.close();
    }


    mRemoteInterface = C_RemoteInterfaceHandler::getInstance(); // Instance creation
    mBoard = C_BoardHandler::getInstance();                     // Instance creation
    mPlayers = C_PlayersHandler::getInstance();                 // Instance creation
    mGame = C_GameHandler::getInstance();                       // Instance creation
    mSound = C_SoundHandler::getInstance();                     // Instance creation

    mView = new C_GuiInterface(this);                           // Must be the last Instance created

    /* Attach widgets to handlers */
    mBoard->setStack(mView->getStack());
    mBoard->setDeathEaterBoard(mView->getDeathEaterBoard());
    mBoard->setPhenixOrderBoard(mView->getPhenixOrderBoard());
    mGame->setElectionTracker(mView->getElectionTracker());
    mGame->setGameTracker(mView->getGameTracker());

    ui->layout->addWidget(mView);

    connect(mRemoteInterface, &C_RemoteInterfaceHandler::sig_event, this, &C_Controller::onEvent);
    connect(mRemoteInterface, &C_RemoteInterfaceHandler::sig_notif, this, &C_Controller::onNotify);
    connect(mRemoteInterface, &C_RemoteInterfaceHandler::sig_updateView, mView, &C_GuiInterface::onUpdateView);
    connect(mRemoteInterface, &C_RemoteInterfaceHandler::sig_connected, this, &C_Controller::onConnected);
    connect(mRemoteInterface, &C_RemoteInterfaceHandler::sig_disconnected, this, &C_Controller::onDisconnected);
    connect(mView, &C_GuiInterface::sig_playerClicked, this, &C_Controller::onPlayerClicked);
    connect(mView, &C_GuiInterface::sig_quitGame, this, &C_Controller::onQuitGame);
    connect(mView, &C_GuiInterface::sig_draw, this, &C_Controller::onDrawLawCards);
    connect(mView, &C_GuiInterface::sig_discard, this, &C_Controller::onDiscardLawCard);
    connect(mView, &C_GuiInterface::sig_askVeto, this, &C_Controller::onAskVeto);
    connect(mView, &C_GuiInterface::sig_clairvoyancePowerUsed, this, &C_Controller::onClairvoyancePowerUsed);
    connect(mView, &C_GuiInterface::sig_spyingPowerUsed, this, &C_Controller::onSpyingPowerUsed);
    connect(mView, &C_GuiInterface::sig_vote, this, &C_Controller::onVote);

#ifdef LOOP_CONNECTION
    QTimer *loopConnection = new QTimer(this);
    connect(loopConnection, &QTimer::timeout, this, &C_Controller::onLoopConnection);
    loopConnection->start(1000);
#endif

}

C_Controller::~C_Controller()
{
    if(mBoard)
        delete mBoard;
    if(mPlayers)
        delete mPlayers;
    if(mGame)
        delete mGame;
    if(mView)
        delete mView;
    if(mRemoteInterface)
        delete mRemoteInterface;
    if(mSound)
        delete mSound;
    delete ui;
}

/* Events from remote server commands */

void C_Controller::Event_DirectorSelection(QByteArray data)
{
    /* Hide potential opened screens */
    mView->quitAllScreen();

    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    me->getPosition() == C_Player::E_POSITION::Minister ? mView->enablePlayersInteraction(true) : mView->enablePlayersInteraction(false) ;
}

void C_Controller::Event_DirectorElection(QByteArray data)
{
    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    if(me->isPlaying() && me->getStatus() != C_Player::E_STATUS::dead)
        mView->displayScreenVote();
}

void C_Controller::Event_MinisterDraw(QByteArray data)
{
    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    if(me->getPosition() == C_Player::E_POSITION::Minister)
    {
        mView->enableDrawing(true);
    }
}

void C_Controller::Event_MinisterDiscard(QByteArray data)
{
    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    if(me->getPosition() == C_Player::E_POSITION::Minister)
    {
        mView->displayScreenLaws(3);
    }
}

void C_Controller::Event_DirectorDiscard(QByteArray data)
{
    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    if(me->getPosition() == C_Player::E_POSITION::Director)
    {
        mView->displayScreenLaws(2);
    }
}

void C_Controller::Event_DirectorAskedVeto(QByteArray data)
{
    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    if(me->getPosition() == C_Player::E_POSITION::Minister)
    {
        W_MessageBoxVeto popup(this);
        if(popup.exec() == QDialog::Accepted)
        {
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Minister_accept_veto));
        }
        else
        {
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Minister_refuse_veto));
        }
    }
}

void C_Controller::Event_PowerSubstituteMinister(QByteArray data)
{
    mView->displayScreenPowerUnlocked(C_LawBoard::E_POWER::substituteMinister);

    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    me->getPosition() == C_Player::E_POSITION::Minister ? mView->enablePlayersInteraction(true) : mView->enablePlayersInteraction(false) ;
}

void C_Controller::Event_PowerClairvoyance(QByteArray data)
{
    mView->displayScreenPowerUnlocked(C_LawBoard::E_POWER::clairvoyance);

    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    if(me->getPosition() == C_Player::E_POSITION::Minister)
    {
        QTimer::singleShot(TIMEOUT_SCREEN_POWER_TRIGGER + 500, [&](){
            mView->displayScreenLaws(3, true);
        });
    }
}

void C_Controller::Event_PowerSpying(QByteArray data)
{
    mView->displayScreenPowerUnlocked(C_LawBoard::E_POWER::spying);

    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    me->getPosition() == C_Player::E_POSITION::Minister ? mView->enablePlayersInteraction(true) : mView->enablePlayersInteraction(false) ;
}

void C_Controller::Event_PowerAssassination(QByteArray data)
{
    mView->displayScreenPowerUnlocked(C_LawBoard::E_POWER::assassination);

    C_Player *me = mPlayers->getMyPlayerInstance();
    if(!me)
    {
        LOG_DBG("My player instance not found");
        return;
    }

    me->getPosition() == C_Player::E_POSITION::Minister ? mView->enablePlayersInteraction(true) : mView->enablePlayersInteraction(false) ;
}

void C_Controller::EVENT_PowerPlayerAssassinated(QByteArray)
{
    mSound->playSound(E_SOUNDS::kill);
}

void C_Controller::EVENT_EndGame(QByteArray)
{
    C_Player *Voldemort = nullptr;
    for(C_Player *player : *mPlayers->getPlayers())
    {
        if(player->getRole() == C_Player::E_ROLE::Voldemort)
        {
            Voldemort = player;
            break;
        }
    }

    if(mBoard->getPhenixOrderBoard()->getCardsOnBoard() == 5
          || Voldemort->getStatus() == C_Player::E_STATUS::dead)
    {
        mView->displayScreenEndGame(C_LawCard::E_FACTION::phenixOrder);
    }
    else if((mBoard->getDeathEaterBoard()->getCardsOnBoard() == 6)
      || ((Voldemort->getFlagFocus() || Voldemort->getPosition() == C_Player::E_POSITION::Director) && mBoard->getDeathEaterBoard()->getCardsOnBoard() >= 3))
    {
        mView->displayScreenEndGame(C_LawCard::E_FACTION::deathEater);
    }
}

void C_Controller::onEvent(C_Message_Event::E_EVENT event, QByteArray data)
{
    if(C_Message_Event::E_EVENT::event_number < event)
    {
        LOG_DBG(QString("Error event message received out of bounds (eventIdx=%1").arg(event));
        return;
    }

    /* Stop/Start music depending in current event */
    if(event != C_Message_Event::E_EVENT::ApplicationStarted
    && event != C_Message_Event::E_EVENT::CS_set_name)
        mSound->stopSound(E_SOUNDS::musicEntrance);
    event == C_Message_Event::E_EVENT::SC_game_finished ? mSound->playSound(E_SOUNDS::musicGameFinished) : mSound->stopSound(E_SOUNDS::musicGameFinished);

    if(eventFunctions[event])
    {
        (this->*eventFunctions[event])(data);
    }
}

void C_Controller::onNotify(QString string)
{
    mView->getMessageLogger()->addMessageInQueue(string);
}

/* Events from board interaction */

void C_Controller::onPlayerClicked(C_Player *player)
{
    mView->enablePlayersInteraction(false);

    if(!mGame->getGameTracker())
    {
        LOG_DBG("Error game tracker instance does not exist");
        return;
    }

    switch(mGame->getGameTracker()->getEvent())
    {
        case C_Message_Event::E_EVENT::SC_Director_selection_begin:
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Director_selected, QByteArray().append(static_cast<quint8>(C_PlayersHandler::getInstance()->getPlayerIdentifier(player)))));
            break;

        case C_Message_Event::E_EVENT::SC_power_assassination:
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_assassin_player, QByteArray().append(static_cast<quint8>(C_PlayersHandler::getInstance()->getPlayerIdentifier(player)))));
            break;

        case C_Message_Event::E_EVENT::SC_power_spying:
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_spying_player, QByteArray().append(static_cast<quint8>(C_PlayersHandler::getInstance()->getPlayerIdentifier(player)))));
            mView->displayScreenSpying(player);
            break;

        case C_Message_Event::E_EVENT::SC_power_substitute_Minister:
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_substitute_Minister_selected, QByteArray().append(static_cast<quint8>(C_PlayersHandler::getInstance()->getPlayerIdentifier(player)))));
            break;

        default:
            LOG_DBG(QString("Error event not handled after player clicked interaction (evenIdx=%1)").arg(mGame->getGameTracker()->getEvent()));
            break;
    }
}

void C_Controller::onDrawLawCards()
{
    mView->enableDrawing(false);

    mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Minister_drew));
}

void C_Controller::onDiscardLawCard(C_LawCard::E_FACTION law)
{
    switch(mGame->getGameTracker()->getEvent())
    {
        case C_Message_Event::E_EVENT::SC_Director_discard:
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Director_discarded, QByteArray().append(static_cast<quint8>(law))));
            break;

        case C_Message_Event::E_EVENT::SC_Minister_discard:
            mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Minister_discarded, QByteArray().append(static_cast<quint8>(law))));
            break;

        default:
            LOG_DBG(QString("Error event not handled after player discarded (evenIdx=%1)").arg(mGame->getGameTracker()->getEvent()));
            break;
    }
}

void C_Controller::onAskVeto()
{
    mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_Director_asked_veto));
}

void C_Controller::onClairvoyancePowerUsed()
{
    mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_clairvoyance_used));
}

void C_Controller::onSpyingPowerUsed()
{
    mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_spying_carried_out));
}

void C_Controller::onVote(W_VoteCard::E_VOTE vote)
{
    mRemoteInterface->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_player_voted, QByteArray().append(static_cast<quint8>(vote))));
}

void C_Controller::onConnected()
{
    mView->quitAllScreen();

    onNotify("Vous êtes maintenant connecté");

    C_SoundHandler::getInstance()->playSound(E_SOUNDS::notify);

    if(C_RemoteInterfaceHandler::getInstance())
        C_RemoteInterfaceHandler::getInstance()->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_set_name, QByteArray(C_RemoteInterfaceHandler::getInstance()->getClient()->getName().toLatin1())));

#ifndef LOOP_CONNECTION
    W_MessageBoxContinue popup(this, "Vous êtes connecté");
    popup.exec();
#endif
}

void C_Controller::onDisconnected()
{
    mView->quitAllScreen();

    C_PlayersHandler::getInstance()->clearPlayerList();
    C_GameHandler::getInstance()->getGameTracker()->setEvent(C_Message_Event::E_EVENT::ApplicationStarted);

    onNotify("Déconnecté... Veuillez vous reconnecter");

    mView->onUpdateView();

    C_SoundHandler::getInstance()->playSound(E_SOUNDS::notify);

#ifndef LOOP_CONNECTION
    W_MessageBoxContinue popup(this, "Vous avez été déconnecté");
    popup.exec();
#endif
}

void C_Controller::onLoopConnection()
{
    if(!mRemoteInterface->getClient()->isConnected())
        mRemoteInterface->getClient()->onConnect();
}

void C_Controller::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_Escape)
    {
        mView->displayScreenMenu();
        if(!mView->getMenu())
            C_RemoteInterfaceHandler::getInstance()->getClient()->sendMessage(new C_Message_Event(C_Message_Event::E_EVENT::CS_set_name, QByteArray(C_RemoteInterfaceHandler::getInstance()->getClient()->getName().toLatin1())));
    }
}

