#include "w_stack.h"
#include "Handlers/c_gamehandler.h"
#include "Handlers/c_soundhandler.h"

#define COLOR_ORANGE QColor(215, 153, 44)

W_Stack::W_Stack(QWidget *parent)
    : C_Stack(parent)
    , mHover(false)
    , mClickable(false)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    show();
}

W_Stack::~W_Stack()
{

}

void W_Stack::leaveEvent(QEvent *)
{
    mHover = false;
    update();
}

void W_Stack::enterEvent(QEvent *)
{
    if(mClickable)
        C_SoundHandler::getInstance()->playSound(E_SOUNDS::hover);
    mHover = true;
    update();
}

void W_Stack::paintEvent(QPaintEvent *)
{
#define LAW_CARD_RATIO  1.49
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_GameHandler::getInstance()->getGameTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    QRect area;
    quint32 widthLaw = width()*6/8, heightLaw = height()*4/5;
    if(heightLaw > widthLaw*LAW_CARD_RATIO) // Width smaller - Size Law card from width
    {
        area.setRect((width() - widthLaw)/2, height() - widthLaw*(LAW_CARD_RATIO) - height()/10, widthLaw, widthLaw*(LAW_CARD_RATIO));
    }
    else // Height smaller - Size Law card from height
    {

        area.setRect((width() - (heightLaw*(1/LAW_CARD_RATIO)))/2, height() - heightLaw - height()/10, heightLaw*(1/LAW_CARD_RATIO), heightLaw);
    }

    painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter.setPen(QPen(COLOR_ORANGE, 4));

    painter.drawPixmap(area, QPixmap(":/images/Law.png"));
    if(mClickable)
        painter.drawRoundedRect(QRect(area.x()+4, area.y()+4, area.width()-8, area.height()-8), 5, 5);

    if(mHover && mClickable)
    {
        painter.rotate(2);
        painter.drawPixmap(area, QPixmap(":/images/Law.png"));
        painter.drawRoundedRect(QRect(area.x()+4, area.y()+4, area.width()-8, area.height()-8), 5, 5);
        painter.rotate(2);
        painter.drawPixmap(area, QPixmap(":/images/Law.png"));
        painter.drawRoundedRect(QRect(area.x()+4, area.y()+4, area.width()-8, area.height()-8), 5, 5);
    }
    if(mHover && !mClickable)
    {
        painter.setPen(QPen(QColor(250, 250, 250)));
        painter.setFont(QFont("Germania", area.width()/14));
        painter.drawText(area.x(), area.y() + area.height()/2, area.width(), area.height()/2, Qt::AlignCenter, QString("%1 cartes\nrestantes").arg(mLawCards.size()));
    }
}

void W_Stack::mousePressEvent(QMouseEvent *)
{
    if(mClickable)
    {
        C_SoundHandler::getInstance()->playSound(E_SOUNDS::draw);
        emit sig_draw();
    }
    update();
}
