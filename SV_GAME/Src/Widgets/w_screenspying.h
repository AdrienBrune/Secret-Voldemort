#ifndef W_SCREENSPYING_H
#define W_SCREENSPYING_H

#include <QPushButton>
#include <QPainter>

#include "Widgets/Components/w_lawcard.h"
#include "../../Shared/Components/c_player.h"
#include "Handlers/c_soundhandler.h"

class W_FactionCard : public QPushButton
{
    Q_OBJECT

public:
    explicit W_FactionCard(QWidget *parent = nullptr, C_LawCard::E_FACTION faction = C_LawCard::E_FACTION::notDefined)
        : QPushButton(parent)
        , mFaction(faction)
        , mHover(false)
    {
        setAttribute(Qt::WA_Hover, true);
        show();
    }
    ~W_FactionCard(){};

public:
    const C_LawCard::E_FACTION &getFaction()const{ return mFaction; };

protected:
    void leaveEvent(QEvent*)override{ mHover = false; };
    void enterEvent(QEvent*)override{ C_SoundHandler::getInstance()->playSound(E_SOUNDS::hover); mHover = true; };
    void paintEvent(QPaintEvent *)override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect area;

        if(mHover)
            area.setRect(0, 0, width(), height());
        else
            area.setRect(width()/10, height()/10, width()*8/10, height()*8/10);

        painter.drawPixmap(area, QPixmap(QString(":/images/Faction_%1.png").arg(mFaction == C_LawCard::E_FACTION::phenixOrder ? "PhenixOrder" : "DeathEater")));
    };

private:
   C_LawCard::E_FACTION mFaction;
   bool mHover;
};




class W_ScreenSpying : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenSpying(QWidget *parent = nullptr, C_Player *playerToSpy = nullptr);
    ~W_ScreenSpying();

signals:
    void sig_timeout();

protected:
    void paintEvent(QPaintEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;

private:
    C_Player *mPlayerToSpy;
    W_FactionCard *wFactionCard;
};

#endif // W_SCREENSPYING_H
