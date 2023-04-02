#include "c_guiinterface.h"
#include "ui_c_guiinterface.h"
#include "Handlers/c_gamehandler.h"
#include "Handlers/c_boardhandler.h"
#include "Handlers/c_playershandler.h"
#include "../../Shared/debug.h"
#include "c_controller.h"
#include "Widgets/w_messagebox.h"

#include <QPainterPath>

C_GuiInterface::C_GuiInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::C_GuiInterface)
    , wSecretRole(nullptr)
    , wScreenVote(nullptr)
    , wScreenLaws(nullptr)
    , wScreenSpying(nullptr)
    , wScreenPower(nullptr)
    , wScreenMenu(nullptr)
    , wScreenEndGame(nullptr)
    , mEnableMenu(true)
{
    ui->setupUi(this);

    wStack = ui->Stack;
    wDeathEaterBoard = ui->DeathEaterBoard;
    wPhenixOrderBoard = ui->PhenixOrderBoard;
    wEasel = ui->Easel;
    wMessageLogger = ui->MessageDisplayer;
    wElectionTracker = ui->ElectionTracker;
    wGameTracker = ui->GameTracker;
    wPlayersManagement = ui->PlayersManagement;
    connect(wPlayersManagement, &W_PlayersManagement::sig_playerClicked, this, &C_GuiInterface::sig_playerClicked);
    connect(wStack, &W_Stack::sig_draw, this, &C_GuiInterface::sig_draw);

    wSecretRole = new W_SecretRole(this);
    wSecretRole->installEventFilter(this);
}

C_GuiInterface::~C_GuiInterface()
{
    delete ui;
}

void C_GuiInterface::onUpdateView()
{
    wStack->update();
    wDeathEaterBoard->update();
    wPhenixOrderBoard->update();
    wEasel->update();
    wMessageLogger->update();
    wElectionTracker->update();
    wGameTracker->update();
    wSecretRole->update();
    wPlayersManagement->onUpdateList();
}

void C_GuiInterface::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawPixmap(rect(), QPixmap(":/images/background.png"));

    /* Black top border */
    painter.setOpacity(0.5);
    painter.setPen(QPen(QColor(0, 0, 0), 4));
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRect(QRect(0, 0, width(), wMessageLogger->height()));

    /* Black bottom border */
    QPainterPath whiteLine;
    whiteLine.moveTo(QPointF(0, ui->Easel->y()));
    whiteLine.lineTo(QPointF(width()*2/5 - width()/10, ui->Easel->y()));
    whiteLine.cubicTo(QPointF(width()*2/5 - width()/10, height()-height()/10), QPointF(width()*2/5, ui->Easel->y()), QPointF(width()*2/5, height()-height()/10));
    whiteLine.lineTo(QPointF(width()+20, height()-height()/10));
    QPainterPath bottomBlackBorder(whiteLine);
    bottomBlackBorder.lineTo(width(), height());
    bottomBlackBorder.lineTo(0, height());
    bottomBlackBorder.lineTo(0, ui->Easel->y());

    painter.setOpacity(0.5);
    painter.setPen(QPen(QColor(0, 0, 0), 0));
    painter.setBrush(QColor(0, 0, 0));
    painter.drawPath(bottomBlackBorder);
    painter.setOpacity(1);
    painter.setPen(QPen(QColor(250, 250, 250), 2));
    painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter.drawPath(whiteLine);
}

void C_GuiInterface::resizeEvent(QResizeEvent *)
{
    wMessageLogger->adjustMessageSize();

    if(wSecretRole)
    {
        wSecretRole->setGeometry(wSecretRole->getStartLocation());
    }
    if(wScreenMenu)
    {
        wScreenMenu->setGeometry(0, 0, width(), height());
    }
    if(wScreenVote)
    {
        wScreenVote->setGeometry(0, 0, width(), height());
    }
    if(wScreenLaws)
    {
        wScreenLaws->setGeometry(0, 0, width(), height());
    }
    if(wScreenSpying)
    {
        wScreenSpying->setGeometry(0, 0, width(), height());
    }
    if(wScreenPower)
    {
        wScreenPower->setGeometry(0, 0, width(), height());
    }
}

void C_GuiInterface::enablePlayersInteraction(bool toggle)
{
    if(!C_PlayersHandler::getInstance())
    {
        LOG_DBG("Error player handler not created");
        return;
    }

    for(C_Player *player : *C_PlayersHandler::getInstance()->getPlayers())
    {
        W_Player *wPlayer = dynamic_cast<W_Player*>(player);
        if(wPlayer)
        {
            wPlayer->setClickable(toggle);
        }
        else
        {
            LOG_DBG("Error player dynamic cast failed");
        }
    }
    onUpdateView();
}

void C_GuiInterface::enableDrawing(bool toggle)
{
    wStack->setClickable(toggle);
}

void C_GuiInterface::displayScreenMenu()
{
    if(gSemaphoreMessageBox)
        return; // Menu access denied

    if(wScreenMenu)
    {
        // Close Menu
        delete wScreenMenu;
        wScreenMenu = nullptr;
        return;
    }

    // Open Menu
    wScreenMenu = new W_ScreenMenu(this);
    connect(wScreenMenu, &W_ScreenMenu::sig_quit, this, &C_GuiInterface::onQuitScreenMenu);
    connect(wScreenMenu, &W_ScreenMenu::sig_openSettings, this, &C_GuiInterface::onOpenSettings);
    connect(wScreenMenu, &W_ScreenMenu::sig_quitGame, this, &C_GuiInterface::sig_quitGame);
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

void C_GuiInterface::onQuitScreenMenu()
{
    if(wScreenMenu)
    {
        delete wScreenMenu;
        wScreenMenu = nullptr;
    }
}


void C_GuiInterface::displayScreenVote()
{
    if(wScreenVote)
    {
        delete wScreenVote;
        wScreenVote = nullptr;
    }

    wScreenVote = new W_ScreenVote(this);
    connect(wScreenVote, &W_ScreenVote::sig_vote, this, &C_GuiInterface::onQuitScreenVote);
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

void C_GuiInterface::onQuitScreenVote(W_VoteCard::E_VOTE vote)
{
    if(wScreenVote)
    {
        delete wScreenVote;
        wScreenVote = nullptr;
    }
    if(vote)
        emit sig_vote(vote);
}

void C_GuiInterface::displayScreenLaws(quint8 cardNumberToShow, bool readOnly)
{
    if(wScreenLaws)
    {
        delete wScreenLaws;
        wScreenLaws = nullptr;
    }

    C_Stack *stack =  C_BoardHandler::getInstance()->getStack();
    if(stack->getStack().size() < cardNumberToShow)
    {
        LOG_DBG("Error law cards in stack out of bound to be shown");
        return;
    }

    QList<C_LawCard::E_FACTION> laws;
    for(int i = stack->getStack().size() - cardNumberToShow; i < stack->getStack().size(); i++)
    {
        laws.append(stack->getStack().at(i).getFaction());
    }

    wScreenLaws = new W_ScreenLaws(this, laws, readOnly);
    connect(wScreenLaws, &W_ScreenLaws::sig_discard, this, &C_GuiInterface::onQuitScreenLaws);
    connect(wScreenLaws, &W_ScreenLaws::sig_askVeto, this, &C_GuiInterface::onAskVeto);
    connect(wScreenLaws, &W_ScreenLaws::sig_timeout, this, &C_GuiInterface::onQuitScreenLaws);
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

void C_GuiInterface::onQuitScreenLaws(C_LawCard::E_FACTION faction)
{
    if(wScreenLaws)
    {
        delete wScreenLaws;
        wScreenLaws = nullptr;
    }

    if(faction) // Card discarded
    {
        emit sig_discard(faction);
    }
    else // Power clairvoyance
    {
        emit sig_clairvoyancePowerUsed();
    }
}

void C_GuiInterface::onAskVeto()
{
    if(wScreenLaws)
    {
        delete wScreenLaws;
        wScreenLaws = nullptr;
    }
    emit sig_askVeto();
}

void C_GuiInterface::displayScreenSpying(C_Player *player)
{
    if(wScreenSpying)
    {
        delete wScreenSpying;
        wScreenSpying = nullptr;
    }

    wScreenSpying = new W_ScreenSpying(this, player);
    connect(wScreenSpying, &W_ScreenSpying::sig_timeout, this, &C_GuiInterface::onQuitScreenSpying);
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

void C_GuiInterface::onQuitScreenSpying()
{
    if(wScreenSpying)
    {
        delete wScreenSpying;
        wScreenSpying = nullptr;
    }
    emit sig_spyingPowerUsed();
}

void C_GuiInterface::displayScreenPowerUnlocked(C_LawBoard::E_POWER power)
{
    if(wScreenPower)
    {
        delete wScreenPower;
        wScreenPower = nullptr;
    }

    wScreenPower = new W_ScreenPowerUnlocked(this, power);
    connect(wScreenPower, &W_ScreenPowerUnlocked::sig_timeout, this, &C_GuiInterface::onQuitScreenPower);
    wScreenPower->update();
}

void C_GuiInterface::onQuitScreenPower(C_LawBoard::E_POWER power)
{
    if(wScreenPower)
    {
        delete wScreenPower;
        wScreenPower = nullptr;
    }
    emit sig_PowerReady(power);
}

void C_GuiInterface::displayScreenEndGame(C_LawCard::E_FACTION faction)
{
    if(wScreenEndGame)
    {
        delete wScreenEndGame;
        wScreenEndGame = nullptr;
    }

    wScreenEndGame = new W_ScreenEndGame(this, faction);
    connect(wScreenEndGame, &W_ScreenEndGame::sig_timeout, this, &C_GuiInterface::onQuitScreenEndGame);
    wScreenEndGame->update();
}

void C_GuiInterface::onQuitScreenEndGame()
{
    if(wScreenEndGame)
    {
        delete wScreenEndGame;
        wScreenEndGame = nullptr;
    }
}

void C_GuiInterface::quitAllScreen()
{
    if(wScreenLaws)
    {
        delete wScreenLaws;
        wScreenLaws = nullptr;
    }
    if(wScreenMenu)
    {
        delete wScreenMenu;
        wScreenMenu = nullptr;
    }
    if(wScreenPower)
    {
        delete wScreenPower;
        wScreenPower = nullptr;
    }
    if(wScreenVote)
    {
        delete wScreenVote;
        wScreenVote = nullptr;
    }
}

void C_GuiInterface::onOpenSettings()
{
    emit sig_spyingPowerUsed();
}

bool C_GuiInterface::eventFilter(QObject *object, QEvent *event)
{
    if(object == wSecretRole && event->type() == QEvent::HoverEnter)
    {
        wSecretRole->display(true);
    }

    if(object == wSecretRole && event->type() == QEvent::HoverLeave)
    {
        wSecretRole->display(false);
    }

    return false;
}
