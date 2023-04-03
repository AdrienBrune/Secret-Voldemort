#include "c_controller.h"
#include "ui_c_controller.h"

#include <QRandomGenerator>

#define DEFAULT_TIME_BEETWEEN_STEP  4000

C_Controller::C_Controller(QWidget *parent)
    : QMainWindow(parent)
    , mMachineStep(E_ST::st_iddle)
    , mStack(nullptr)
    , mPhenixOrderBoard(nullptr)
    , mDeathEaterBoard(nullptr)
    , mGameTracker(nullptr)
    , mElectionTracker(nullptr)
    , mServer(nullptr)
    , nextSubstituteMinister(nullptr)
    , ui(new Ui::C_Controller)
{
    ui->setupUi(this);

    mServer = new C_ServerTcp(this);
    connect(mServer, &C_ServerTcp::sig_addPlayer, this, &C_Controller::onAddPlayer);
    connect(mServer, &C_ServerTcp::sig_removePlayer, this, &C_Controller::onRemovePlayer);
    connect(ui->buttonStartGame, &QPushButton::clicked, this, &C_Controller::onStartGame);

    mStack = new C_Stack(this);
    mPhenixOrderBoard = new C_PhenixOrderBoard(this);
    mDeathEaterBoard = new C_DeathEaterBoard(this);
    mGameTracker = new C_GameTracker(this);
    mElectionTracker = new C_ElectionTracker(this);

    QTimer *tMachineState = new QTimer(this);
    connect(tMachineState, &QTimer::timeout, this, &C_Controller::onMachineState);
    tMachineState->start(50);

    // Server GUI linking

    ui->stack->setStack(mStack);
    ui->Board_DeathEater->setLawBoard(mDeathEaterBoard);
    connect(mDeathEaterBoard, &C_LawBoard::sig_cardPut, ui->Board_DeathEater, &GUI_LawBoard::onUpdateGUI);
    ui->Board_PhenixOrder->setLawBoard(mPhenixOrderBoard);
    connect(mPhenixOrderBoard, &C_LawBoard::sig_cardPut, ui->Board_PhenixOrder, &GUI_LawBoard::onUpdateGUI);
    connect(this, &C_Controller::sig_printLog, this, &C_Controller::onPrintLog);
    connect(mServer, &C_ServerTcp::sig_printLog, this, &C_Controller::sig_printLog);
    connect(mPhenixOrderBoard, &C_LawBoard::sig_printLog, this, &C_Controller::sig_printLog);
    connect(mDeathEaterBoard, &C_LawBoard::sig_printLog, this, &C_Controller::sig_printLog);
    connect(mGameTracker, &C_GameTracker::sig_printLog, this, &C_Controller::sig_printLog);
    connect(mStack, &C_Stack::sig_printLog, this, &C_Controller::sig_printLog);
}

C_Controller::~C_Controller()
{
    while(!mPlayers.isEmpty())
        delete mPlayers.takeLast();
    while(!mPlayersSaved.isEmpty())
        delete mPlayersSaved.takeLast();
    while(!mStepQueue.isEmpty())
        delete mStepQueue.takeLast();
    delete ui;
}

void C_Controller::onEvent(C_Message_Event::E_EVENT event, C_Player *player, const QByteArray &data)
{
    if(C_Message_Event::E_EVENT::event_number < event)
    {
        LOG_DBG(QString("Error event message received out of bounds (eventIdx=%1").arg(event));
        return;
    }

    if(eventFunctions[event])
    {
        (this->*eventFunctions[event])(player, data);
    }
}

void C_Controller::EVENT_DirectorSelected(C_Player *, const QByteArray &data)
{
    if(!data.size())
    {
        LOG_DBG("Error data received empty");
        return;
    }

    quint8 indexDirector = static_cast<quint8>(data[0]);
    if(indexDirector >= mPlayers.size())
    {
        LOG_DBG("Error index Director out of bounds");
        return;
    }

    mPlayers[indexDirector]->setFlagFocus(true);

    mVoteResults.lumos = 0;
    mVoteResults.nox = 0;
    mVoteStack.clear();

    addNextState(new C_NextStep(E_ST::st_directorElectionBegin));
}

void C_Controller::EVENT_PlayerVoted(C_Player *player, const QByteArray &data)
{
    quint8 vote;
    QDataStream stream(data);

    player->setFlagVote(true);
    sendUpdatedGameToPlayers();

    stream >> vote;
    mVoteStack.append(static_cast<E_VOTE>(vote));
    switch(voteResult())
    {
    case E_VOTE::lumos:
        addNextState(new C_NextStep(E_ST::st_voteResult));
        C_Tools::getFocusedPlayer(&mPlayers)->setPosition(C_Player::E_POSITION::Director);
        C_Tools::getFocusedPlayer(&mPlayers)->setFlagFocus(false);
        C_Tools::removeFlagVote(&mPlayers);
        if(mDeathEaterBoard->getCardsOnBoard() > 2 && IS_VOLDEMORT(C_Tools::getPlayer(&mPlayers, C_Player::E_POSITION::Director)))
        {
            addNextState(new C_NextStep(E_ST::st_voteVoldemortElected, DEFAULT_TIME_BEETWEEN_STEP));
            addNextState(new C_NextStep(st_gameFinished, DEFAULT_TIME_BEETWEEN_STEP));
        }
        else
            addNextState(new C_NextStep(E_ST::st_voteResultLumos, DEFAULT_TIME_BEETWEEN_STEP));
        break;

    case E_VOTE::nox:
        addNextState(new C_NextStep(E_ST::st_voteResult));
        C_Tools::removeFlagVote(&mPlayers);

        addNextState(new C_NextStep(E_ST::st_voteResultNox, DEFAULT_TIME_BEETWEEN_STEP));

        mElectionTracker->setCounter(mElectionTracker->getCounter() + 1);
        addNextState(new C_NextStep(st_electionTrackerIncremented, DEFAULT_TIME_BEETWEEN_STEP));

        if(mElectionTracker->getFlag())
        {
            MinisterEnterChaos();
        }
        else
        {
            addNextState(new C_NextStep(st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
        }
        break;

    default:
        return;
    }
}

void C_Controller::EVENT_MinisterDrew(C_Player *, const QByteArray &)
{
    addNextState(new C_NextStep(E_ST::st_ministerDiscard));
}

void C_Controller::EVENT_MinisterDiscarded(C_Player *, const QByteArray &data)
{
    quint8 law;
    QDataStream stream(data);

    if(mStack->getStack().size() < 3 || mStack->getStack().size() < 3)
    {
        LOG_DBG("Error not enought laws in pile to process vote");
        return;
    }

    stream >> law;
    for(int i = mStack->getStack().size() - 3; i < mStack->getStack().size(); i++)
    {
        if(law == mStack->getStack()[i].getFaction())
        {
            mStack->getStack().takeAt(i);
            break;
        }
    }

    addNextState(new C_NextStep(E_ST::st_directorDiscard));
}

void C_Controller::EVENT_DirectorDiscarded(C_Player *, const QByteArray &data)
{
    quint8 law;
    QDataStream stream(data);
    C_LawBoard *lawBoard = nullptr;

    if(mStack->getStack().size() < 2 || mStack->getStack().size() < 2)
    {
        LOG_DBG("Error not enought laws in pile to process vote");
        return;
    }

    /* Law vote */
    stream >> law;
    for(int i = mStack->getStack().size() - 2; i < mStack->getStack().size(); i++)
    {
        if(law == mStack->getStack()[i].getFaction())
        {
            mStack->getStack().takeAt(i);
            break;
        }
    }
    mStack->getStack().takeLast().getFaction() == C_LawCard::E_FACTION::deathEater ? lawBoard = mDeathEaterBoard : lawBoard = mPhenixOrderBoard;
    lawBoard->addCardOnBoard();
    mElectionTracker->setCounter(0);
    addNextState(new C_NextStep(E_ST::st_lawVoted));

    if(lawBoard->getCardsOnBoard() >= lawBoard->getLawSpotNumber())
    {
        addNextState(new C_NextStep(st_gameFinished, DEFAULT_TIME_BEETWEEN_STEP));
    }
    else
    {
        if(!checkForPower())
        {
            addNextState(new C_NextStep(st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
        }
    }
}

void C_Controller::EVENT_DirectorAskedVeto(C_Player *, const QByteArray &)
{
    addNextState(new C_NextStep(st_directorAskedVeto));
}

void C_Controller::EVENT_MinisterAcceptedVeto(C_Player *, const QByteArray &)
{
    addNextState(new C_NextStep(st_ministerAcceptedVeto));

    mElectionTracker->setCounter(mElectionTracker->getCounter() + 1);
    addNextState(new C_NextStep(st_electionTrackerIncremented, DEFAULT_TIME_BEETWEEN_STEP));

    if(mElectionTracker->getFlag())
    {
        MinisterEnterChaos();
    }

    addNextState(new C_NextStep(st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
}

void C_Controller::EVENT_MinisterRefusedVeto(C_Player *, const QByteArray &)
{
    addNextState(new C_NextStep(st_ministerRefusedVeto));
}

void C_Controller::EVENT_PowerSubstituteMinisterSelected(C_Player *, const QByteArray &data)
{
    quint8 playerIndex;
    QDataStream stream(data);

    C_Tools::removeFocusOnPlayers(&mPlayers);
    stream >> playerIndex;

    if(playerIndex >= mPlayers.size())
    {
        LOG_DBG("Player selected out of bounds");
        return;
    }

    mPlayers[playerIndex]->setFlagFocus(true);
    nextSubstituteMinister = mPlayers[playerIndex];
    addNextState(new C_NextStep(st_nextMinisterSelected));
    addNextState(new C_NextStep(E_ST::st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
}

void C_Controller::EVENT_PowerClairvoyanceUsed(C_Player *, const QByteArray &)
{
    addNextState(new C_NextStep(st_ministerClairvoyanceCarriedOut));
    addNextState(new C_NextStep(st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
}

void C_Controller::EVENT_PowerSpyingInProgress(C_Player *, const QByteArray &data)
{
    quint8 playerIndex;
    QDataStream stream(data);

    C_Tools::removeFocusOnPlayers(&mPlayers);
    stream >> playerIndex;

    if(playerIndex >= mPlayers.size())
    {
        LOG_DBG("Player selected out of bounds");
        return;
    }

    mPlayers[playerIndex]->setFlagFocus(true);
    addNextState(new C_NextStep(E_ST::st_spyingInProgress));
}

void C_Controller::EVENT_PowerSpyingCarriedOut(C_Player *, const QByteArray &)
{
    addNextState(new C_NextStep(E_ST::st_playerSpied));
    addNextState(new C_NextStep(E_ST::st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
}

void C_Controller::EVENT_PowerPlayerToAssassinSelected(C_Player *, const QByteArray &data)
{
    quint8 playerIndex;
    QDataStream stream(data);

    C_Tools::removeFocusOnPlayers(&mPlayers);
    stream >> playerIndex;

    if(playerIndex >= mPlayers.size())
    {
        LOG_DBG("Player selected out of bounds");
        return;
    }

    mPlayers[playerIndex]->setFlagFocus(true);
    mPlayers[playerIndex]->setStatus(C_Player::E_STATUS::dead);
    addNextState(new C_NextStep(E_ST::st_playerAssassinated));
    if(IS_VOLDEMORT(mPlayers[playerIndex]))
    {
        addNextState(new C_NextStep(st_gameFinished, DEFAULT_TIME_BEETWEEN_STEP));
    }
    else
    {
        addNextState(new C_NextStep(E_ST::st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP));
    }
}

void C_Controller::EVENT_SetPlayerName(C_Player *player, const QByteArray &data)
{
    player->setName(data);
}

void C_Controller::onMachineState()
{
    /* Process next state */

    if(!mStepQueue.isEmpty())
    {
        static quint8 sStepQueueSt = 0;
        switch(sStepQueueSt)
        {
        case 0: // IDLE
            mStepQueue.first()->start(); // Start next step delay
            sStepQueueSt++;
            break;

        case 1: // DELAY
            if(!mStepQueue.first()->isActive()) // Next step is ready to be loaded
            {
                if(mMachineStep != E_ST::st_iddle)
                {
                    LOG_DBG("Error next state stamp another state");
                }
                mMachineStep = mStepQueue.first()->getNextStep();
                emit sig_machineStepChanged();
                delete mStepQueue.takeFirst();
                sStepQueueSt = 0;
            }
            break;

        default:
            sStepQueueSt = 0;
            break;
        }
    }

    /* Machine state */

    C_Player *minister = C_Tools::getPlayer(&mPlayers, C_Player::E_POSITION::Minister);
    C_Player *director = C_Tools::getPlayer(&mPlayers, C_Player::E_POSITION::Director);
    C_Player *playerFocus = C_Tools::getFocusedPlayer(&mPlayers);
    C_LawBoard *board = nullptr;

    switch(mMachineStep)
    {
    case E_ST::st_iddle:
        break;

    case E_ST::st_startGame:
        updateGameState(QString("La partie vient de commencer"));
        addNextState(new C_NextStep(st_startTurn, DEFAULT_TIME_BEETWEEN_STEP));
        break;

    case E_ST::st_startTurn:
        prepareCurrentTurn();
        updateGameState(QString("Début du tour, %1 est le Ministre de la magie").arg(minister ? minister->getName() : "Le Ministre"));
        updateGameState(QString("%1 choisit le prochain Directeur de Poudlard").arg(minister ? minister->getName() : "Le Ministre"), C_Message_Event::E_EVENT::SC_Director_selection_begin);
        break;

    case E_ST::st_directorElectionBegin:
        updateGameState(QString("%1 propose d'élire %2 en tant que Directeur de Poudlard").arg(minister ? minister->getName() : "Le Ministre").arg(playerFocus ? playerFocus->getName() : "le joueur sélectionné"), C_Message_Event::E_EVENT::SC_Director_election_begin);
        break;

    case E_ST::st_voteResult:
        C_Tools::removeFocusOnPlayers(&mPlayers);
        updateGameState(QString("Résultats du vote : %1 Lumos, %2 Nox").arg(mVoteResults.lumos).arg(mVoteResults.nox));
        break;

    case E_ST::st_voteVoldemortElected:
        updateGameState(QString("%1 a été élu Directeur de Poudlard").arg(director ? director->getName() : "Le joueur sélectionné"), C_Message_Event::SC_Director_elected);
        break;

    case E_ST::st_voteResultNox:
        updateGameState(QString("%1 n'a pas été élu Directeur de Poudlard").arg(director ? director->getName() : "Le joueur sélectionné"), C_Message_Event::SC_Director_not_elected);
        break;

    case E_ST::st_voteResultLumos:
        updateGameState(QString("%1 est élu Directeur de Poudlard").arg(director ? director->getName() : "Le joueur sélectionné"), C_Message_Event::SC_Director_elected);
        addNextState(new C_NextStep(st_ministerDraw, DEFAULT_TIME_BEETWEEN_STEP));
        break;

    case E_ST::st_ministerDraw:
        updateGameState(QString("Le Ministre de la magie et le Directeur de Poudlard s'apprêtent à voter une loi"));
        updateGameState(QString("Veuillez piocher 3 cartes de loi"), C_Message_Event::SC_Minister_draw, minister);
        break;

    case E_ST::st_ministerDiscard:
        updateGameState(QString("Le Ministre de la magie est en train de choisir la loi à défausser"));
        updateGameState(QString(""), C_Message_Event::SC_Minister_discard, minister);
        break;

    case E_ST::st_directorDiscard:
        updateGameState(QString("Le Directeur de Poudlard est en train de choisir la loi à défausser"));
        updateGameState(QString(""), C_Message_Event::SC_Director_discard, director);
        break;

    case E_ST::st_directorAskedVeto:
        updateGameState(QString("%1 a demandé à ne pas voter de loi ce tour-ci").arg(director ? director->getName() : "Le Directeur de Poudlard"), C_Message_Event::SC_Director_asked_veto);
        break;

    case E_ST::st_ministerRefusedVeto:
        updateGameState(QString("%1 a refusé la demande de veto").arg(minister ? minister->getName() : "Le Ministre de la magie"), C_Message_Event::SC_Director_discard);
        break;

    case E_ST::st_ministerAcceptedVeto:
        updateGameState(QString("Le Ministre a accepté la demande de veto"));
        break;

    case E_ST::st_electionTrackerIncremented:
        updateGameState(QString("Un jeton d'élection a été ajouté, au bout de 3, le ministère entrera dans un état de chaos"));
        break;

    case E_ST::st_ministryEnterChaos:
        updateGameState(QString("Le Ministère de la magie entre dans un état de chaos !!"));
        break;

    case E_ST::st_lawOnTopOfThePileVoted:
        mStack->getStack().takeLast().getFaction() == C_LawCard::E_FACTION::deathEater ? board = mDeathEaterBoard : board = mPhenixOrderBoard ;
        board->addCardOnBoard();
        checkForPower(); // Use potential power and ignore it
        board->getCardsOnBoard() >= board->getLawSpotNumber() ? addNextState(new C_NextStep(st_gameFinished, DEFAULT_TIME_BEETWEEN_STEP)) : addNextState(new C_NextStep(st_endOfTurn, DEFAULT_TIME_BEETWEEN_STEP)) ;
        updateGameState(QString("CHAOS - Loi du dessus de la pile votée et pouvoirs désactivés"));
        break;

    case E_ST::st_everyoneCouldBeNextDirector:
        updateGameState(QString("CHAOS - Tout le monde devient éligible pour être le prochain Directeur"));
        break;

    case E_ST::st_lawVoted:
        updateGameState(QString("Une nouvelle loi a été votée"));
        break;

    case E_ST::st_triggerPowerAssassination:
        updateGameState(QString("%1 choisit quel joueur assassiner").arg(minister ? minister->getName() : "Le Ministre de la magie"), C_Message_Event::SC_power_assassination);
        break;

    case E_ST::st_triggerPowerSpying:
        updateGameState(QString("%1 choisit quel joueur espionner").arg(minister ? minister->getName() : "Le Ministre de la magie"), C_Message_Event::SC_power_spying);
        break;

    case E_ST::st_triggerPowerClairvoyance:
        updateGameState(QString("%1 regarde les trois lois du dessus de la pile").arg(minister ? minister->getName() : "Le Ministre de la magie"), C_Message_Event::SC_power_clairvoyance);
        break;

    case E_ST::st_triggerPowerSubstituteMinister:
        updateGameState(QString("%1 choisit le Ministre de la magie pour le prochain tour").arg(minister ? minister->getName() : "Le Ministre de la magie"), C_Message_Event::SC_power_substitute_Minister);
        break;

    case E_ST::st_playerAssassinated:
        updateGameState(QString("%1 a été assassiné par %2").arg(playerFocus ? playerFocus->getName() : "Un joueur").arg(minister ? minister->getName() : "Le Ministre de la magie"), C_Message_Event::SC_player_murdered);
        break;

    case E_ST::st_spyingInProgress:
        updateGameState(QString("%1 est en train d'espionner %2").arg(minister ? minister->getName() : "Le Ministre de la magie").arg(playerFocus ? playerFocus->getName() : "un joueur"));
        break;

    case E_ST::st_playerSpied:
        updateGameState(QString("%1 a terminé d'espionner %2").arg(minister ? minister->getName() : "Le Ministre de la magie").arg(playerFocus ? playerFocus->getName() : "le joueur"));
        break;

    case E_ST::st_ministerClairvoyanceCarriedOut:
        updateGameState(QString("%1 a terminé de regarder les cartes de loi du dessus de la pile").arg(minister ? minister->getName() : "Le Ministre de la magie"));
        break;

    case E_ST::st_nextMinisterSelected:
        updateGameState(QString("%1 a choisi %2 pour être le Ministre de la magie le tour prochain").arg(minister ? minister->getName() : "Le Ministre de la magie").arg(playerFocus ? playerFocus->getName() : "le joueur"));
        break;

    case E_ST::st_endOfTurn:
        prepareNextTurn();
        if(mStack->getStack().size() < 3)
            addNextState(new C_NextStep(st_stackShuffled, DEFAULT_TIME_BEETWEEN_STEP));
        updateGameState(QString("Fin du tour, le prochain tour est sur le point de commencer"), C_Message_Event::SC_end_of_turn);
        addNextState(new C_NextStep(st_startTurn, DEFAULT_TIME_BEETWEEN_STEP));
        break;

    case E_ST::st_stackShuffled:
        mStack->reshuffleStack();
        updateGameState(QString("La pile a été à nouveau remplie"));
        break;

    case E_ST::st_gameFinished:
        updateGameState(QString("La partie est terminée"), C_Message_Event::SC_game_finished);
        break;

    default:
        mMachineStep = E_ST::st_iddle;
        LOG_DBG(QString("Error step not handled (id=%1)").arg(mMachineStep));
        break;
    }
    mMachineStep = E_ST::st_iddle;
}

void C_Controller::addNextState(C_NextStep *nextStep)
{
    mStepQueue.append(nextStep);
}

void C_Controller::clearStepQueue()
{
    while(!mStepQueue.isEmpty())
        delete mStepQueue.takeLast();
}

void C_Controller::updateGameState(QString notification, C_Message_Event::E_EVENT event, C_Player *player)
{
    if(event != C_Message_Event::E_EVENT::ApplicationStarted)
        mGameTracker->setEvent(event);

    sendUpdatedGameToPlayers();
    if(notification.length())
        sendNotification(notification, player);
    if(event != C_Message_Event::E_EVENT::ApplicationStarted)
        sendEvent(event, player);
}

void C_Controller::sendUpdatedGameToPlayers()
{
    if(!mElectionTracker || !mStack || !mPhenixOrderBoard || !mDeathEaterBoard)
    {
        LOG_DBG("Error can't send updated game status to player - One instance not defined");
        return;
    }

    for(int i = 0; i < mPlayers.size(); i++)
    {
        C_Message_UpdateGame *updateGame = new C_Message_UpdateGame(mElectionTracker, mGameTracker);
        C_Message_UpdateBoard *updateBoard = new C_Message_UpdateBoard(mStack, mPhenixOrderBoard, mDeathEaterBoard);
        C_Message_UpdatePlayers *updatePlayers = new C_Message_UpdatePlayers(i, &mPlayers);
        mPlayers[i]->sendMessage(updatePlayers);
        mPlayers[i]->sendMessage(updateGame);
        mPlayers[i]->sendMessage(updateBoard);
    }
}

void C_Controller::sendNotification(QString notification, C_Player *player)
{
    if(player)
        player->sendMessage(new C_Message_Notification(notification));
    else
    {
        for(C_Player *player : qAsConst(mPlayers))
        {
            player->sendMessage(new C_Message_Notification(notification));
        }
    }
}

void C_Controller::sendEvent(C_Message_Event::E_EVENT event, C_Player *player)
{
    if(player)
        player->sendMessage(new C_Message_Event(event));
    else
    {
        for(C_Player *player : qAsConst(mPlayers))
        {
            player->sendMessage(new C_Message_Event(event));
        }
    }
}

void C_Controller::initGame()
{
    //ui->buttonStartGame->setEnabled(false);

    // Remove players saved
    while(!mPlayersSaved.isEmpty())
        delete mPlayersSaved.takeLast();
    // Remove votes on stack
    mVoteStack.clear();
    // Shuffle stack
    mStack->reshuffleStack();
    // Remove law voted from boards
    mPhenixOrderBoard->setCardsOnBoard(0);
    mDeathEaterBoard->setCardsOnBoard(0);
    // Set game tracker
    mGameTracker->setEvent(C_Message_Event::E_EVENT::ApplicationStarted);
    // Reset election tracker
    mElectionTracker->setCounter(0);
    // Remove all flags from players
    for(C_Player *player : qAsConst(mPlayers))
    {
        player->setFlagFocus(false);
        player->setFlagVote(false);
        player->setRole(C_Player::E_ROLE::notAttributed);
        player->setPosition(C_Player::E_POSITION::none);
        player->setStatus(C_Player::E_STATUS::playing);
    }
    nextSubstituteMinister = nullptr;
    ministerToRestaure.active = false;
    ministerToRestaure.index = 0;
    // Link roles to players
    C_Tools::setRoles(&mPlayers);
    // Minister selection
    mPlayers[QRandomGenerator().global()->bounded(mPlayers.size())]->setPosition(C_Player::E_POSITION::Minister);

    clearStepQueue();
}

int C_Controller::checkForPower()
{
    /* If Minister into chaos, ignore power after using it */
    if(mElectionTracker->getFlag())
    {
        mDeathEaterBoard->checkForPower(C_Tools::getPlayerPlaying(&mPlayers).size());
        return 0;
    }

    switch(mDeathEaterBoard->checkForPower(C_Tools::getPlayerPlaying(&mPlayers).size()))
    {
    case C_LawBoard::E_POWER::assassination:
        addNextState(new C_NextStep(E_ST::st_triggerPowerAssassination, 3000));
        return 1;

    case C_LawBoard::E_POWER::clairvoyance:
        addNextState(new C_NextStep(E_ST::st_triggerPowerClairvoyance, 3000));
        return 1;

    case C_LawBoard::E_POWER::spying:
        addNextState(new C_NextStep(E_ST::st_triggerPowerSpying, 3000));
        return 1;

    case C_LawBoard::E_POWER::substituteMinister:
        addNextState(new C_NextStep(E_ST::st_triggerPowerSubstituteMinister, 3000));
        return 1;

    default:
        break;
    }

    return 0;
}

void C_Controller::MinisterEnterChaos()
{
    addNextState(new C_NextStep(st_ministryEnterChaos, DEFAULT_TIME_BEETWEEN_STEP));
    addNextState(new C_NextStep(st_everyoneCouldBeNextDirector, DEFAULT_TIME_BEETWEEN_STEP));
    addNextState(new C_NextStep(st_lawOnTopOfThePileVoted, DEFAULT_TIME_BEETWEEN_STEP));
}

void C_Controller::prepareCurrentTurn()
{
    C_Tools::removeFocusOnPlayers(&mPlayers);
    C_Tools::removeFlagVote(&mPlayers);
    nextSubstituteMinister = nullptr;
    mVoteResults.lumos = 0;
    mVoteResults.nox = 0;
    mVoteResults.voteNumber = 0;
}

void C_Controller::prepareNextTurn()
{
    C_Tools::removeFocusOnPlayers(&mPlayers);
    if(mElectionTracker->getFlag())
    {
        C_Tools::removePositionsExceptMinister(&mPlayers);
        mElectionTracker->setCounter(0);
    }
    C_Tools::setNextPositions(&mPlayers, nextSubstituteMinister);
    nextSubstituteMinister = nullptr;
}

E_VOTE C_Controller::voteResult()
{
    mVoteResults.voteNumber = 0;
    for(C_Player *player : qAsConst(mPlayers))
        if(player->getStatus() == C_Player::E_STATUS::playing)
            mVoteResults.voteNumber++;
    if(mVoteStack.size() < mVoteResults.voteNumber)
        return E_VOTE::notDefined;
    else
    {
        /* Everyone has voted */
        while(!mVoteStack.isEmpty())
        {
            mVoteStack.takeLast() == E_VOTE::lumos ? mVoteResults.lumos++ : mVoteResults.nox++;
        }
        return (mVoteResults.lumos > mVoteResults.nox) ? E_VOTE::lumos : E_VOTE::nox;
    }
}

void C_Controller::onStartGame()
{
    if(mPlayers.size() < 5)
        return;

    initGame();

    addNextState(new C_NextStep(E_ST::st_startGame));
}

void C_Controller::onAddPlayer(C_TcpSocketAck *client)
{
    C_Player *player = new C_Player(this, client);
    connect(player, &C_Player::sig_messageReceived, this, &C_Controller::onMessageReceived);

    if(!mPlayersSaved.isEmpty())
    {
        /* Player backup */
        C_Player *playerBackup = mPlayersSaved.takeFirst();
        player->setRole(playerBackup->getRole());
        player->setPosition(playerBackup->getPosition());
        player->setStatus(playerBackup->getStatus());
        player->setRole(playerBackup->getRole());
        player->setFlagFocus(playerBackup->getFlagFocus());
    }
    else
    {
        /* New player joined the game */
        player->setStatus(C_Player::E_STATUS::connected);
    }
    mPlayers.append(player);

    updateGUIPlayerList();
    sendUpdatedGameToPlayers();
}

void C_Controller::onRemovePlayer(C_TcpSocketAck *client)
{
    for(int i = 0; i < mPlayers.size(); i++)
    {
        if(mPlayers[i]->getSocket() == client)
        {
            if(mGameTracker->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted
            || mGameTracker->getEvent() == C_Message_Event::E_EVENT::SC_game_finished)
            {
                delete mPlayers.takeAt(i);
            }
            else
            {
                mPlayersSaved << mPlayers.takeAt(i);
            }
        }
    }
    updateGUIPlayerList();
    sendUpdatedGameToPlayers();
}

void C_Controller::updateGUIPlayerList()
{
    while(!mGUIPlayers.isEmpty())
        delete mGUIPlayers.takeLast();

    for(C_Player *player : qAsConst(mPlayers))
    {
        GUI_Player *guiPlayer = new GUI_Player(this, player);
        ui->layoutPlayers->addWidget(guiPlayer);
        mGUIPlayers.append(guiPlayer);
    }
}

void C_Controller::onMessageReceived(C_Message *message, C_Player *player)
{
    C_Message_Event *messageEvent = dynamic_cast<C_Message_Event*>(message);
    if(messageEvent)
    {
        onEvent(messageEvent->getEvent(), player, messageEvent->getData());
        sendUpdatedGameToPlayers();
        return;
    }
    C_Message_Notification *messageNotif = dynamic_cast<C_Message_Notification*>(message);
    if(messageNotif)
    {
        // TODO
        return;
    }

    LOG_DBG("Erreur message receive from client not an event, not a notif either");
    return;
}

void C_Controller::onPrintLog(QString message)
{
    ui->output_log->append(message);
}

void C_Controller::on_buttonRestartTurn_clicked()
{
    if(mGameTracker->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted
    || mGameTracker->getEvent() == C_Message_Event::E_EVENT::SC_game_finished)
    {
        return;
    }

    addNextState(new C_NextStep(st_startTurn));
}
