#ifndef W_SCREENLAWS_H
#define W_SCREENLAWS_H

#include <QPushButton>
#include <QPainter>

#include "../../Shared/Components/c_stack.h"
#include "Handlers/c_soundhandler.h"

class W_LawCard : public QPushButton
{
    Q_OBJECT

public:
    explicit W_LawCard(QWidget *parent = nullptr, C_LawCard::E_FACTION faction = C_LawCard::E_FACTION::notDefined, bool readOnly = false)
        : QPushButton(parent)
        , mFaction(faction)
        , mHover(false)
        , mReadOnly(readOnly)
    {
        setAttribute(Qt::WA_Hover, true);
        show();
    }
    ~W_LawCard(){};

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

        painter.drawPixmap(area, QPixmap(QString(":/images/Law_%1.png").arg(mFaction == C_LawCard::E_FACTION::phenixOrder ? "PhenixOrder" : mFaction == C_LawCard::E_FACTION::deathEater ? "DeathEater" : "")));
    };

private:
   C_LawCard::E_FACTION mFaction;
   bool mHover;
   bool mReadOnly;
};

class W_ScreenLaws : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenLaws(QWidget *parent = nullptr, QList<C_LawCard::E_FACTION> laws = QList<C_LawCard::E_FACTION>(), bool readOnly = false);
    ~W_ScreenLaws();

signals:
    void sig_discard(C_LawCard::E_FACTION);
    void sig_askVeto();
    void sig_timeout(C_LawCard::E_FACTION faction = C_LawCard::E_FACTION::notDefined);

protected:
    void paintEvent(QPaintEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;

private slots:
    void onDiscard();

private:
    QRect getLawCardPosition(quint8 index);

private:
    QList<W_LawCard*> wLaws;
    bool mReadOnly;
    QPushButton *mButtonVeto;
};

#endif // W_SCREENLAWS_H
