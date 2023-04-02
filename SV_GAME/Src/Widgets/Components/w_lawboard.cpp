#include "Widgets/Components/w_lawboard.h"
#include "Handlers/c_playershandler.h"
#include "Handlers/c_gamehandler.h"

#define STR_NO_POWER        ""
#define STR_CLAIRVOYANCE    "Pouvoir de Clairvoyance\nLe Ministre de la magie regarde les trois cartes de loi du dessus de la pile"
#define STR_ASSASSINATION   "Pouvoir d'Assassinat\nLe Ministre de la magie obtient la lourde tâche d'assassiner un joueur"
#define STR_SPYING          "Pouvoir d'Espionnage\nLe Ministre de la magie peut regarder la faction d'un joueur"
#define STR_COUP_DETAT      "Coup d'état\nLe Ministre de la magie choisit le Ministre pour le tour suivant"

#define STR_IMG_CLAIRVOYANCE    "Divination"
#define STR_IMG_ASSASSINATION   "Avada_Kedavra"
#define STR_IMG_SPYING          "Endoloris"
#define STR_IMG_COUP_DETAT      "Impero"

QString gStrPowerBoard[11][6] = {
    {STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER},
    {STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER},
    {STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER},
    {STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER},
    {STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER, STR_NO_POWER},
    {STR_NO_POWER, STR_NO_POWER, STR_CLAIRVOYANCE, STR_ASSASSINATION, STR_ASSASSINATION, STR_NO_POWER},
    {STR_NO_POWER, STR_NO_POWER, STR_CLAIRVOYANCE, STR_ASSASSINATION, STR_ASSASSINATION, STR_NO_POWER},
    {STR_NO_POWER, STR_SPYING, STR_COUP_DETAT, STR_ASSASSINATION, STR_ASSASSINATION, STR_NO_POWER},
    {STR_NO_POWER, STR_SPYING, STR_COUP_DETAT, STR_ASSASSINATION, STR_ASSASSINATION, STR_NO_POWER},
    {STR_SPYING, STR_SPYING, STR_COUP_DETAT, STR_ASSASSINATION, STR_ASSASSINATION, STR_NO_POWER},
    {STR_SPYING, STR_SPYING, STR_COUP_DETAT, STR_ASSASSINATION, STR_ASSASSINATION, STR_NO_POWER}
};

QString gStrPowerImages[11][6] = {
    {"", "", "", "", "", ""}, // To remove
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", STR_IMG_CLAIRVOYANCE, STR_IMG_ASSASSINATION, STR_IMG_ASSASSINATION, ""},
    {"", "", STR_IMG_CLAIRVOYANCE, STR_IMG_ASSASSINATION, STR_IMG_ASSASSINATION, ""},
    {"", STR_IMG_SPYING, STR_IMG_COUP_DETAT, STR_IMG_ASSASSINATION, STR_IMG_ASSASSINATION, ""},
    {"", STR_IMG_SPYING, STR_IMG_COUP_DETAT, STR_IMG_ASSASSINATION, STR_IMG_ASSASSINATION, ""},
    {STR_IMG_SPYING, STR_IMG_SPYING, STR_IMG_COUP_DETAT, STR_IMG_ASSASSINATION, STR_IMG_ASSASSINATION, ""},
    {STR_IMG_SPYING, STR_IMG_SPYING, STR_IMG_COUP_DETAT, STR_IMG_ASSASSINATION, STR_IMG_ASSASSINATION, ""}
};

W_LawBoard::W_LawBoard(QWidget *parent, quint8 lawSpotNumber)
    : C_LawBoard(parent, lawSpotNumber)
{
    // Setup Resizing of cards displayed on board
    connect(this, &C_LawBoard::sig_updateGUI, this, &W_LawBoard::onUpdateGUI);
    mTimerResize = new QTimer(this);
    mTimerResize->setSingleShot(true);
    connect(mTimerResize, &QTimer::timeout, this, [&](){
        for(int i = 0; i < mCards.size(); i++)
        {
            mCards[i]->setGeometry(getLawSlot(i));
        }
        update();
    });

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

    if(!C_GameHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance()->getPlayers())
        return;

    if(!C_GameHandler::getInstance()->getGameTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    /* Draw board */
    painter.drawPixmap(mArea, QPixmap(QString(":/images/Board_DeathEater.png")));

    /* Draw powers */
    for(int i = mCardsOnBoard; i < mLawSpotNumber; i++)
    {
        QRect slotArea(getLawSlot(i)), boundingArea;
        if(mHover != i)
        {
            boundingArea.setRect(slotArea.x() + slotArea.width()*(1.0 - 8.0/10.0)/2.0, slotArea.y() + slotArea.height()*(1.0 - 8.0/10.0)/2.0, slotArea.width()*8.0/10.0, slotArea.height()*8.0/10.0);
        }
        else
        {
            boundingArea.setRect(slotArea.x(), slotArea.y(), slotArea.width(), slotArea.height());
        }
        painter.drawPixmap(boundingArea, QPixmap(QString(":/images/%1.png").arg(gStrPowerImages[C_PlayersHandler::getInstance()->getPlayers()->size() > 10 ? 10 : C_PlayersHandler::getInstance()->getPlayers()->size()][i])));
    }
}

void W_DeathEaterBoard::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    setToolTip("");
    for(int i = mCardsOnBoard; i < mLawSpotNumber; i++)
    {
        if(getLawSlot(i).contains(mouseEvent->pos()))
        {
            if(!C_PlayersHandler::getInstance())
                return;

            if(!C_PlayersHandler::getInstance()->getPlayers())
                return;

            mHover = i;
            setToolTip(gStrPowerBoard[C_PlayersHandler::getInstance()->getPlayers()->size() > 10 ? 10 :C_PlayersHandler::getInstance()->getPlayers()->size()][i]);
            update();
            return;
        }
    }
    mHover = NO_HOVER;
    update();
}

QRect W_DeathEaterBoard::getLawSlot(quint8 slotIndex)
{
    if(height() > width()/DEATHEATER_BOARD_RATIO) // Width smaller - Size Board from width
    {
        mArea.setRect(0, 0, width(), width()/DEATHEATER_BOARD_RATIO);
    }
    else // Height smaller - Size Board from height
    {
        mArea.setRect(0, 0, height()*DEATHEATER_BOARD_RATIO, height());
    }

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

    if(!C_GameHandler::getInstance())
        return;

    if(!C_GameHandler::getInstance()->getGameTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    getLawSlot(0); // Necessary to update mArea
    painter.drawPixmap(mArea, QPixmap(":/images/Board_PhenixOrder.png"));
}

QRect W_PhenixOrderBoard::getLawSlot(quint8 slotIndex)
{
    if(height() > width()/PHENIX_ORDER_BOARD_RATIO) // Width smaller - Size Board from width
    {
        mArea.setRect(0, height() - width()/PHENIX_ORDER_BOARD_RATIO, width(), width()/PHENIX_ORDER_BOARD_RATIO);
    }
    else // Height smaller - Size Board from height
    {
        mArea.setRect(0, 0, height()*PHENIX_ORDER_BOARD_RATIO, height());
    }

    return QRect(PHENIX_ORDER_BOARD_MARGIN_LEFT + slotIndex*(PHENIX_ORDER_BOARD_LAW_WIDTH+PHENIX_ORDER_BOARD_SPACING), PHENIX_ORDER_BOARD_LAX_LEVEL, PHENIX_ORDER_BOARD_LAW_WIDTH, PHENIX_ORDER_BOARD_LAW_HEIGHT);
}










void W_BoardDeathEaterCard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(!C_GameHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance()->getPlayers())
        return;

    if(!C_GameHandler::getInstance()->getGameTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    QRect boundingArea(width()*(1.0 - 8.0/10.0)/2, height()*(1.0 - 8.0/10.0)/2, width()*8.0/10.0, height()*8.0/10.0);

    if(mHover)
    {
        boundingArea.setRect(0, 0, width(), height());
    }
    painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Law_DeathEater.png")));
}

void W_BoardPhenixOrderCard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(!C_GameHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance()->getPlayers())
        return;

    if(!C_GameHandler::getInstance()->getGameTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    QRect boundingArea(width()*(1.0 - 8.0/10.0)/2, height()*(1.0 - 8.0/10.0)/2, width()*8.0/10.0, height()*8.0/10.0);

    if(mHover)
    {
        boundingArea.setRect(0, 0, width(), height());
    }
    painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Law_PhenixOrder.png")));
}
