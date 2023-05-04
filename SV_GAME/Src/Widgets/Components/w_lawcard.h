#ifndef W_LAWCARD_H
#define W_LAWCARD_H

#include <QPushButton>
#include <QPainter>

#include "Handlers/c_soundhandler.h"

class C_LawCard
{
public:
    enum E_FACTION
    {
        notDefined,
        phenixOrder,
        deathEater
    };

public:
    C_LawCard(E_FACTION faction)
        : mFaction(faction)
    {};
    ~C_LawCard(){};

public:
    const E_FACTION &getFaction()const{ return mFaction; };
    void setFaction(const E_FACTION &type){ mFaction = type; };

protected:
    E_FACTION mFaction;
};


class W_LawCard : public QPushButton
{
    Q_OBJECT

public:
    explicit W_LawCard(QWidget *parent = nullptr, C_LawCard::E_FACTION faction = C_LawCard::E_FACTION::notDefined, bool clickable = true);
    ~W_LawCard();

signals:
    void clicked(W_LawCard*);

public:
    const C_LawCard::E_FACTION &getFaction()const{ return mFaction; };

protected:
    void leaveEvent(QEvent*)override{ mHover = false; update(); };
    void enterEvent(QEvent*)override{ mHover = true; update(); C_SoundHandler::getInstance()->playSound(E_SOUNDS::hover);};
    void paintEvent(QPaintEvent*)override;

private:
   C_LawCard::E_FACTION mFaction;
   bool mClickable;
   bool mHover;
};

#endif // W_LAWCARD_H
