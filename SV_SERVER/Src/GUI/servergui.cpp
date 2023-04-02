#include "servergui.h"
#include "../../Shared/c_textdrawinghelper.h"
#include "c_controller.h"

QString gRolesStr[11] = {
    "",
    "Harry Potter",
    "Hermione Granger",
    "Ron Weasley",
    "Sirius Black",
    "Neville Londubat",
    "Albus Dumbledor",
    "Drago Malfoy",
    "Beatrix Lestrange",
    "Lucius Malfoy",
    "Lord Voldemort"
};

QString gPositionsStr[5] = {
    "",
    "Former",
    "Former",
    "Minister",
    "Director"
};

C_Controller::E_ST gMachineState = C_Controller::E_ST::st_iddle;

GUI_Player::GUI_Player(QWidget *parent, C_Player *player)
    : QWidget(parent)
    , mPlayer(player)
    , mHover(false)
{
    C_Controller *controller = dynamic_cast<C_Controller*>(parent);
    if(controller)
    {
        connect(controller, &C_Controller::sig_machineStepChanged, this, &GUI_Player::onUpdateGUI);
    }
    if(mPlayer)
    {
        connect(mPlayer, &C_Player::sig_attributeChanged, this, &GUI_Player::onUpdateGUI);
    }
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    setHeight(SIZE_SMALL);
    show();
}

void GUI_Player::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!mPlayer)
        return;

    QRect box_background(0, 0, mHover && mPlayer->getRole() != C_Player::E_ROLE::notAttributed ? width() : mPlayer ? mPlayer->getStatus() == C_Player::E_STATUS::dead ? width()/2 : width()*3/4 : width()*3/4, height()), boundingArea;

    /* Focus drawing */
    if(mPlayer)
    {
        if(mPlayer->getFlagFocus())
        {
            painter.setPen(QPen(QColor(243, 194, 22), 2));
        }
    }

    painter.setOpacity(0.95);
    if(!mHover || mPlayer->getRole() == C_Player::E_ROLE::notAttributed)
    {
        /* Background box drawing */
        painter.setBrush(mPlayer->getStatus() != C_Player::E_STATUS::dead ? QColor(30,30,30) : QColor(180,80,80));
        painter.drawRoundedRect(box_background, 5, 100, Qt::RelativeSize);
        painter.setOpacity(1);

        /* Name drawing */
        boundingArea.setRect(box_background.x()+50, box_background.y()+box_background.height()/5, box_background.width()*7/8-50, box_background.height()*3/5);
        painter.setPen(QPen(QColor(250, 250, 250)));
        painter.setFont(C_AutoScaleFont("Germania", mPlayer->getName(), boundingArea, boundingArea.height()));
        painter.drawText(boundingArea, mPlayer->getName());
        if(mPlayer->getStatus() == C_Player::E_STATUS::dead)
            return;
        /* Position drawing */
        if(mPlayer->getPosition() != C_Player::E_POSITION::none)
        {
            boundingArea.setRect(box_background.x()+box_background.width()+5, box_background.y()+10, width()-box_background.width()-10, box_background.height()-20);
            painter.drawPixmap(boundingArea, QPixmap(QString(":/images/easel_%1").arg(gPositionsStr[mPlayer->getPosition()])));
        }
        /* Vote flag drawing */
        if(mPlayer->getFlagVote())
        {
            boundingArea.setRect(box_background.x()+box_background.width()-(box_background.height()-30)*3/2-15, box_background.y()+15, (box_background.height()-30)*3/2, box_background.height()-30);
            painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Vote_flag.png")));
        }
        /* Power in progress drawing */
        if(mPlayer->getPosition() == C_Player::E_POSITION::Minister)
        {
            QString power("");
            switch(gMachineState)
            {
                case C_Controller::E_ST::st_triggerPowerAssassination:
                case C_Controller::E_ST::st_playerAssassinated:
                    power = "Assassinat";
                    break;

                case C_Controller::E_ST::st_triggerPowerClairvoyance:
                case C_Controller::E_ST::st_ministerClairvoyanceCarriedOut:
                    power = "Clairvoyance";
                    break;

                case C_Controller::E_ST::st_triggerPowerSpying:
                case C_Controller::E_ST::st_spyingInProgress:
                case C_Controller::E_ST::st_playerSpied:
                    power = "Espionnage";
                    break;

                case C_Controller::E_ST::st_triggerPowerSubstituteMinister:
                case C_Controller::E_ST::st_nextMinisterSelected:
                    power = "Coup d'état";
                    break;

                default:
                    break;
            }
            if(power.length())
            {
                boundingArea.setRect(box_background.x()+box_background.width()-(box_background.height()-30)*3/2-15, box_background.y()+15, box_background.height()-30, box_background.height()-30);
                painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Power_%1.png").arg(power)));
            }
        }
    }
    else
    {
        /* Background box drawing */
        painter.setBrush(mPlayer->getStatus() != C_Player::E_STATUS::dead ? QColor(30,30,30) : QColor(180,80,80));
        painter.drawRoundedRect(box_background, 5, 20, Qt::RelativeSize);
        painter.setOpacity(1);

        /* Role drawing */
        boundingArea.setRect(box_background.x()+20, box_background.y()+20, box_background.width()/5, box_background.height()-40);
        painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Role_%1").arg(gRolesStr[mPlayer->getRole()])));
        /* Position drawing */
        if(mPlayer->getPosition() != C_Player::E_POSITION::none)
        {
            boundingArea.setRect(box_background.x()+box_background.width()+5, box_background.y()+10, width()-box_background.width()-10, box_background.height()-20);
            if(mPlayer->getPosition() == C_Player::E_POSITION::exDirector || mPlayer->getPosition() == C_Player::E_POSITION::exDirector)
                painter.setOpacity(0.7);
            painter.drawPixmap(boundingArea, QPixmap(QString(":/images/easel_%1").arg(gPositionsStr[mPlayer->getPosition()])));
            painter.setOpacity(1);
        }
        /* Name drawing */
        boundingArea.setRect(box_background.x()+box_background.width()/5+40, box_background.y()+box_background.height()*4/5-20, width()-(box_background.x()+box_background.width()/5+40), box_background.height()/5);
        painter.setPen(QPen(QColor(250, 250, 250)));
        painter.setFont(C_AutoScaleFont("Germania", mPlayer->getName(), boundingArea, boundingArea.height()));
        painter.drawText(boundingArea, mPlayer->getName());
        /* Position drawing */
        if(mPlayer->getPosition() != C_Player::E_POSITION::none)
        {
            boundingArea.setRect(box_background.x()+box_background.width()/5+40, box_background.y()+box_background.height()*3/5-20, 200, box_background.height()/5);
            painter.drawPixmap(boundingArea, QPixmap(QString(":/images/easel_%1").arg(gPositionsStr[mPlayer->getPosition()])));
        }
        /* Vote flag drawing */
        if(mPlayer->getFlagVote())
        {
            boundingArea.setRect(box_background.x()+box_background.width()-60-20, box_background.y()+box_background.height()-40-20, 60, 40);
            painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Vote_flag.png")));
        }
        /* Power in progress drawing */
        if(mPlayer->getPosition() == C_Player::E_POSITION::Minister)
        {
            QString power("");
            switch(gMachineState)
            {
                case C_Controller::E_ST::st_triggerPowerAssassination:
                case C_Controller::E_ST::st_playerAssassinated:
                    power = "Assassinat";
                    break;

                case C_Controller::E_ST::st_triggerPowerClairvoyance:
                case C_Controller::E_ST::st_ministerClairvoyanceCarriedOut:
                    power = "Clairvoyance";
                    break;

                case C_Controller::E_ST::st_triggerPowerSpying:
                case C_Controller::E_ST::st_spyingInProgress:
                case C_Controller::E_ST::st_playerSpied:
                    power = "Espionnage";
                    break;

                case C_Controller::E_ST::st_triggerPowerSubstituteMinister:
                case C_Controller::E_ST::st_nextMinisterSelected:
                    power = "Coup d'état";
                    break;

                default:
                    break;
            }
            if(power.length())
            {
                boundingArea.setRect(box_background.x()+box_background.width()-120-20, box_background.y()+20, 120, 120);
                painter.drawPixmap(boundingArea, QPixmap(QString(":/images/Power_%1.png").arg(power)));

                QString powerStr(QString("Pouvoir %1 en cours ...").arg(power));
                boundingArea.setRect(box_background.x()+box_background.width()/5+40, box_background.y()+20, width()/2, box_background.height()/5);
                painter.setPen(QPen(QColor(250, 250, 250)));
                painter.setFont(C_AutoScaleFont("Germania", powerStr, boundingArea, boundingArea.height()));
                painter.drawText(boundingArea, powerStr);
            }
        }
    }
    /* Socket empty drawing */
    if(!mPlayer->getSocket())
    {
        boundingArea.setRect(0,height()-30,30,30);
        painter.drawPixmap(boundingArea, QPixmap(QString(":/images/warning.png")));
    }
}

void GUI_Player::onUpdateGUI()
{
    C_Controller *controller = dynamic_cast<C_Controller*>(parent());
    if(controller)
    {
        gMachineState = controller->getMachineState();
    }
    update();
};






void GUI_Stack::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!mStack)
        return;

    if(!mHover || mIndexCurrentLawDisplayed == UNASSIGNED)
    {
        painter.drawPixmap(rect(), QPixmap(":/images/Law.png"));
        QString str(QString("%1\nlois restantes").arg(mStack->getStack().size()));
        QRect boundingArea(width()/3, height()*3/4, width()/3, height()/4);
        painter.setPen(QPen(QColor(250, 250, 250)));
        painter.setFont(C_AutoScaleFont("Germania", str, boundingArea, boundingArea.height()));
        painter.drawText(boundingArea, Qt::AlignCenter, str);
    }
    else
    {
        painter.drawPixmap(rect(), QPixmap(QString(":/images/Law_%1.png").arg(mStack->getStack().at(mIndexCurrentLawDisplayed).getFaction() == C_LawCard::E_FACTION::deathEater ? "DeathEater" : "PhenixOrder")));
        QString str(QString("indexe %1\ndans la pile").arg(mIndexCurrentLawDisplayed));
        QRect boundingArea(width()/8, height()*3/4, width()*3/4, height()/4);
        painter.setOpacity(0.7);
        painter.setBrush(QColor(250,250,250));
        painter.drawRect(boundingArea);
        painter.setOpacity(1);
        painter.setPen(QPen(QColor(30, 30, 30)));
        painter.setFont(C_AutoScaleFont("Germania", str, boundingArea, boundingArea.height()));
        painter.drawText(boundingArea, Qt::AlignCenter, str);
    }
}





#define DEATHEATER_BOARD_SPACING      (0.0230*rect().width())
#define DEATHEATER_BOARD_LAW_WIDTH    (0.1266*rect().width())
#define DEATHEATER_BOARD_LAW_HEIGHT   (0.6185*rect().height())
#define DEATHEATER_BOARD_MARGIN_LEFT  (rect().x() + 0.0640*rect().width())
#define DEATHEATER_BOARD_LAX_LEVEL    (rect().y() + 0.1896*rect().height())
#define DEATHEATER_BOARD_RATIO        (3.38)

#define PHENIX_ORDER_BOARD_SPACING      (0.0291*rect().width())
#define PHENIX_ORDER_BOARD_LAW_WIDTH    (0.1452*rect().width())
#define PHENIX_ORDER_BOARD_LAW_HEIGHT   (0.6049*rect().height())
#define PHENIX_ORDER_BOARD_MARGIN_LEFT  (rect().x() + 0.0828*rect().width())
#define PHENIX_ORDER_BOARD_LAX_LEVEL    (rect().y() + 0.2005*rect().height())
#define PHENIX_ORDER_BOARD_RATIO        (2.9)

void GUI_LawBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!mLawBoard)
        return;

    C_DeathEaterBoard *DeathEaterBoard = dynamic_cast<C_DeathEaterBoard*>(mLawBoard);
    if(DeathEaterBoard)
    {
        painter.drawPixmap(rect(), QPixmap(QString(":/images/board_deathEater_56.png")));

        for(int i = 0; i < mLawBoard->getCardsOnBoard(); i++)
        {
            painter.drawPixmap(getDeathEaterLawSlot(i), QPixmap(":/images/Law_DeathEater.png"));
        }
    }
    else
    {
        painter.drawPixmap(rect(), QPixmap(QString(":/images/board_phenixOrder.png")));

        for(int i = 0; i < mLawBoard->getCardsOnBoard(); i++)
        {
            painter.drawPixmap(getPhenixOrderLawSlot(i), QPixmap(":/images/Law_PhenixOrder.png"));
        }
    }
}

QRect GUI_LawBoard::getDeathEaterLawSlot(quint8 slotIndex)
{
    return QRect(DEATHEATER_BOARD_MARGIN_LEFT + slotIndex*(DEATHEATER_BOARD_LAW_WIDTH+DEATHEATER_BOARD_SPACING), DEATHEATER_BOARD_LAX_LEVEL, DEATHEATER_BOARD_LAW_WIDTH, DEATHEATER_BOARD_LAW_HEIGHT);
}

QRect GUI_LawBoard::getPhenixOrderLawSlot(quint8 slotIndex)
{
    return QRect(PHENIX_ORDER_BOARD_MARGIN_LEFT + slotIndex*(PHENIX_ORDER_BOARD_LAW_WIDTH+PHENIX_ORDER_BOARD_SPACING), PHENIX_ORDER_BOARD_LAX_LEVEL, PHENIX_ORDER_BOARD_LAW_WIDTH, PHENIX_ORDER_BOARD_LAW_HEIGHT);
}
