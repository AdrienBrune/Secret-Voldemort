#include "Widgets/Components/w_lawboard.h"
#include "Handlers/c_playershandler.h"
#include "Handlers/c_gamehandler.h"

W_LawBoard::W_LawBoard(QWidget *parent, quint8 lawSpotNumber)
    : C_LawBoard(parent, lawSpotNumber)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    show();
}

W_LawBoard::~W_LawBoard()
{

}

#define DEATHEATER_BOARD_SPACING      (0.0230*mArea.width())
#define DEATHEATER_BOARD_LAW_WIDTH    (0.1266*mArea.width())
#define DEATHEATER_BOARD_LAW_HEIGHT   (0.6185*mArea.height())
#define DEATHEATER_BOARD_MARGIN_LEFT  (mArea.x() + 0.0640*mArea.width())
#define DEATHEATER_BOARD_LAX_LEVEL    (mArea.y() + 0.1896*mArea.height())
#define DEATHEATER_BOARD_RATIO        (3.38)

void W_DeathEaterBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_GameHandler::getInstance()->getGameTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    if(height() > width()/DEATHEATER_BOARD_RATIO) // Width smaller - Size Board from width
    {
        mArea.setRect(0, 0, width(), width()/DEATHEATER_BOARD_RATIO);
    }
    else // Height smaller - Size Board from height
    {
        mArea.setRect(0, 0, height()*DEATHEATER_BOARD_RATIO, height());
    }

    quint8 mNumberOfPlayers = C_PlayersHandler::getInstance()->getPlayers()->size();
    painter.drawPixmap(mArea, QPixmap(QString(":/images/board_deathEater_%1.png").arg(mNumberOfPlayers < 7 ? "56" : mNumberOfPlayers < 9 ? "78" : "910")));

    for(int i = 0; i < mCardsOnBoard; i++)
    {
        painter.drawPixmap(getLawSlot(i), QPixmap(":/images/Law_DeathEater.png"));
    }
}

QRect W_DeathEaterBoard::getLawSlot(quint8 slotIndex)
{
    return QRect(DEATHEATER_BOARD_MARGIN_LEFT + slotIndex*(DEATHEATER_BOARD_LAW_WIDTH+DEATHEATER_BOARD_SPACING), DEATHEATER_BOARD_LAX_LEVEL, DEATHEATER_BOARD_LAW_WIDTH, DEATHEATER_BOARD_LAW_HEIGHT);
}

#define PHENIX_ORDER_BOARD_SPACING      (0.0291*mArea.width())
#define PHENIX_ORDER_BOARD_LAW_WIDTH    (0.1452*mArea.width())
#define PHENIX_ORDER_BOARD_LAW_HEIGHT   (0.6049*mArea.height())
#define PHENIX_ORDER_BOARD_MARGIN_LEFT  (mArea.x() + 0.0828*mArea.width())
#define PHENIX_ORDER_BOARD_LAX_LEVEL    (mArea.y() + 0.2005*mArea.height())
#define PHENIX_ORDER_BOARD_RATIO        (2.9)

void W_PhenixOrderBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_PlayersHandler::getInstance())
        return;

    if(!C_GameHandler::getInstance())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    if(height() > width()/PHENIX_ORDER_BOARD_RATIO) // Width smaller - Size Board from width
    {
        mArea.setRect(0, height() - width()/PHENIX_ORDER_BOARD_RATIO, width(), width()/PHENIX_ORDER_BOARD_RATIO);
    }
    else // Height smaller - Size Board from height
    {
        mArea.setRect(0, 0, height()*PHENIX_ORDER_BOARD_RATIO, height());
    }

    painter.drawPixmap(mArea, QPixmap(":/images/board_phenixOrder.png"));
    for(int i = 0; i < mCardsOnBoard; i++)
    {
        painter.drawPixmap(getLawSlot(i), QPixmap(":/images/Law_PhenixOrder.png"));
    }
}

QRect W_PhenixOrderBoard::getLawSlot(quint8 slotIndex)
{
    return QRect(PHENIX_ORDER_BOARD_MARGIN_LEFT + slotIndex*(PHENIX_ORDER_BOARD_LAW_WIDTH+PHENIX_ORDER_BOARD_SPACING), PHENIX_ORDER_BOARD_LAX_LEVEL, PHENIX_ORDER_BOARD_LAW_WIDTH, PHENIX_ORDER_BOARD_LAW_HEIGHT);
}
