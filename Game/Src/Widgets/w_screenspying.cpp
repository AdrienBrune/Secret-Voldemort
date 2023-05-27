#include "w_screenspying.h"
#include "../../Shared/c_textdrawinghelper.h"
#include <QTimer>

#define FACTION_CARD_MARGIN_LEFT    ((width()-FACTION_CARD_WIDTH)/2)
#define FACTION_CARD_RATIO          (1.42)
#define FACTION_CARD_WIDTH          (width()*0.187)
#define FACTION_CARD_HEIGHT         (FACTION_CARD_WIDTH*FACTION_CARD_RATIO)
#define FACTION_CARD_LEVEL          (height()/4+20)

W_ScreenSpying::W_ScreenSpying(QWidget *parent, C_Player *playerToSpy)
    : QWidget(parent)
    , mPlayerToSpy(playerToSpy)
    , wFactionCard(nullptr)
{
    setGeometry(0, 0, parent->width(), parent->height());
    if(playerToSpy)
    {
        wFactionCard = new W_FactionCard(this, IS_DEATHEATER(playerToSpy) ? C_LawCard::E_FACTION::deathEater : C_LawCard::E_FACTION::phenixOrder);
    }
    show();
    QTimer::singleShot(5000, this, [&](){ emit sig_timeout(); });
}

W_ScreenSpying::~W_ScreenSpying()
{

}

void W_ScreenSpying::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setOpacity(0.8);
    painter.setPen(QPen(QColor(0,0,0,0), 0));
    painter.setBrush(QColor(20, 20, 20));
    painter.drawRect(rect());

    if(!wFactionCard)
        return;

    QRect boundingArea(20, 0, width()-40, height()/4-20);
    QString string = QString("Faction partagée par %1").arg(mPlayerToSpy ? mPlayerToSpy->getName() : "le joueur");
    painter.setFont(C_AutoScaleFont("Germania", string, boundingArea));
    painter.setPen(QPen(QColor(250, 250, 250), 1));
    painter.drawLine(QLine(0, height()/4, width(), height()/4));
    painter.drawText(boundingArea, Qt::AlignCenter|Qt::AlignBottom, string);
}

void W_ScreenSpying::resizeEvent(QResizeEvent *)
{
    if(wFactionCard)
    {
        wFactionCard->setGeometry(FACTION_CARD_MARGIN_LEFT, FACTION_CARD_LEVEL, FACTION_CARD_WIDTH, FACTION_CARD_HEIGHT);
    }
}
