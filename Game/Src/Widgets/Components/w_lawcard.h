#ifndef W_LAWCARD_H
#define W_LAWCARD_H

#include <QPushButton>
#include <QPainter>

#include "../../Shared/Components/c_lawcard.h"
#include "Handlers/c_soundhandler.h"


class W_LawCard : public QPushButton
{
    Q_OBJECT

public:
    enum E_SELECTION{
        none,
        selected,
        notSelected
    };

public:
    explicit W_LawCard(QWidget *parent = nullptr, C_LawCard::E_FACTION faction = C_LawCard::E_FACTION::notDefined, bool clickable = true);
    ~W_LawCard();

public:
    const C_LawCard::E_FACTION &getFaction()const{ return mFaction; };

    const E_SELECTION &getSelection()const{ return mSelected; };
    void setSelection(const E_SELECTION &selection){ mSelected = selection; update(); };

protected:
    void leaveEvent(QEvent*)override{ mHover = false; update(); };
    void enterEvent(QEvent*)override{ mHover = true; update(); C_SoundHandler::getInstance()->playSound(E_SOUNDS::hover);};
    void paintEvent(QPaintEvent*)override;

private:
   C_LawCard::E_FACTION mFaction;
   E_SELECTION mSelected;
   bool mClickable;
   bool mHover;
};

#endif // W_LAWCARD_H
