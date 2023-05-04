#include "w_lawcard.h"

W_LawCard::W_LawCard(QWidget *parent, C_LawCard::E_FACTION faction, bool clickable)
    : QPushButton(parent)
    , mFaction(faction)
    , mClickable(clickable)
    , mHover(false)
{
    setAttribute(Qt::WA_Hover, true);
    if(mClickable){
        connect(this, &QPushButton::clicked, this, [&](){
            C_SoundHandler::getInstance()->playSound(E_SOUNDS::cardSelection);
            emit clicked(this);
        });
    }
    else
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
    connect(this, &W_LawCard::clicked, this, [&](){
        emit clicked(this);
    });

    QRect area;
    if(mHover)
        area.setRect(0, 0, width(), height());
    else
        area.setRect(width()/10, height()/10, width()*8/10, height()*8/10);

    painter.drawPixmap(area, QPixmap(QString(":/images/Law%1.png").arg(mFaction == C_LawCard::E_FACTION::phenixOrder ? "_PhenixOrder" : mFaction == C_LawCard::E_FACTION::deathEater ? "_DeathEater" : "")));
}
