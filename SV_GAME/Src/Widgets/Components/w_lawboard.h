#ifndef W_LAWBOARD_H
#define W_LAWBOARD_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

#include "../../Shared/Components/c_lawboard.h"

#define NO_HOVER    10

class W_BoardLawCard : public QWidget
{
    Q_OBJECT

public:
    W_BoardLawCard(QWidget *parent = nullptr)
        : QWidget(parent)
        , mHover(false)
    {
        hide();
    }
    ~W_BoardLawCard(){};

public:
    void setSize(QSize size){ setGeometry(0, 0, size.width(), size.height()); }

protected:
    void leaveEvent(QEvent*)override{ mHover = false; update(); };
    void enterEvent(QEvent*)override{ mHover = true; update(); };

protected:
    bool mHover;
};

class W_BoardDeathEaterCard : public W_BoardLawCard
{
    Q_OBJECT

public:
    explicit W_BoardDeathEaterCard(QWidget *parent = nullptr)
        : W_BoardLawCard(parent)
    {
        setToolTip("Les Mangemorts remportent la partie en votant 6 de ces lois\nVoldemort fait gagner sa faction en étant élu Directeur de Poudlard si 3 lois sont votées");
    }
    ~W_BoardDeathEaterCard(){}

protected:
    void paintEvent(QPaintEvent *)override;

};
class W_BoardPhenixOrderCard : public W_BoardLawCard
{
    Q_OBJECT

public:
    explicit W_BoardPhenixOrderCard(QWidget *parent = nullptr)
        : W_BoardLawCard(parent)
    {
        setToolTip("L'Order du Phénix remporte la partie en votant 5 de ces lois");
    }
    ~W_BoardPhenixOrderCard(){}

protected:
    void paintEvent(QPaintEvent *)override;

};






class W_LawBoard : public C_LawBoard
{
    Q_OBJECT

public:
    explicit W_LawBoard(QWidget *parent = nullptr, quint8 lawSpotNumber = 0);
    ~W_LawBoard();

protected:
    virtual QRect getLawSlot(quint8 slotIndex)=0;

protected slots:
    void onUpdateGUI()
    {
        int diff = mCardsOnBoard - mCards.size();
        if(diff != 0)
        {
            /* Add or remove cards */
            for(int i = (diff > 0 ? mCards.size() : mCardsOnBoard); i < (diff > 0 ? mCardsOnBoard : mCards.size()); i = i + (diff > 0 ? 1 : -1))
            {
                if(diff > 0)
                {
                    W_BoardLawCard *law = nullptr;
                    if(getLawSpotNumber() == SPOT_NUMBER_MANGEMORT_BOARD)
                        law = new W_BoardDeathEaterCard(this);
                    else
                        law = new W_BoardPhenixOrderCard(this);
                    mCards.append(law);
                    law->setGeometry(getLawSlot(i));
                    law->show();
                }
                else if(diff < 0)
                {
                    if(!mCards.isEmpty())
                        delete mCards.takeLast();
                }
            }
        }
    }

protected:
    void resizeEvent(QResizeEvent *)override
    {
        // Timer to issue the resize as last event
        mTimerResize->stop();
        mTimerResize->start();
    }

private:
    qreal mWithHeightRatio;
    QPixmap mImage;
    QList<W_BoardLawCard*> mCards;
    QTimer *mTimerResize;
};

class W_DeathEaterBoard : public W_LawBoard
{
    Q_OBJECT

public:
    W_DeathEaterBoard(QWidget *parent = nullptr)
        : W_LawBoard(parent, SPOT_NUMBER_MANGEMORT_BOARD)
        , mHover(NO_HOVER)
    {
        setMouseTracking(true);
    }
    ~W_DeathEaterBoard(){};

protected:
    void paintEvent(QPaintEvent *)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void leaveEvent(QEvent*)override{ mHover = NO_HOVER; setToolTip(""); update(); };

private:
    virtual QRect getLawSlot(quint8 slotIndex)override;

private:
    QRect mArea;
    quint8 mHover;
};

class W_PhenixOrderBoard : public W_LawBoard
{
    Q_OBJECT

public:
    W_PhenixOrderBoard(QWidget *parent = nullptr):W_LawBoard(parent, SPOT_NUMBER_PHENIX_ORDER_BOARD)
    {
        connect(this, &C_LawBoard::sig_updateGUI, this, &W_PhenixOrderBoard::onUpdateGUI);
    }
    ~W_PhenixOrderBoard(){}

public:
    virtual E_POWER checkForPower()override{ mFlagPowerChecked = true; return E_POWER::none; };
    virtual E_POWER checkLawSpotPower(const quint8 &)const override{ return E_POWER::none; };

protected:
    void paintEvent(QPaintEvent *)override;

private:
    virtual QRect getLawSlot(quint8 slotIndex)override;

private:
    QRect mArea;
};

#endif // W_LAWBOARD_H
