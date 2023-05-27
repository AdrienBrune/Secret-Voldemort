#include "w_lawcard.h"

W_LawCard::W_LawCard(QWidget *parent, C_LawCard::E_FACTION faction, bool clickable)
    : QPushButton(parent)
    , mFaction(faction)
    , mSelected(E_SELECTION::none)
    , mClickable(clickable)
    , mHover(false)
{
    setAttribute(Qt::WA_Hover, true);
    if(!mClickable)
        setEnabled(false);
    show();
}

W_LawCard::~W_LawCard()
{

}

void W_LawCard::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect area;
    if(mHover)
    {
        area.setRect(0, 0, width(), height());
    }
    else
    {
        area.setRect(width()/10, height()/10, width()*8/10, height()*8/10);
    }

    if(mSelected == E_SELECTION::notSelected)
    {
        painter.setOpacity(0.3);
        area.setRect(area.x() + area.width()*2/10, area.y() + area.height()*2/10, area.width()*6/10, area.height()*6/10);
    }

    painter.drawPixmap(area, QPixmap(QString(":/images/Law%1.png").arg(mFaction == C_LawCard::E_FACTION::phenixOrder ? "_PhenixOrder" : mFaction == C_LawCard::E_FACTION::deathEater ? "_DeathEater" : "")));
}
