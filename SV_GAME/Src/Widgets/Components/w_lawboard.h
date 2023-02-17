#ifndef W_LAWBOARD_H
#define W_LAWBOARD_H

#include <QWidget>
#include <QPainter>

#include "../../Shared/Components/c_lawboard.h"

class W_LawBoard : public C_LawBoard
{
    Q_OBJECT

public:
    explicit W_LawBoard(QWidget *parent = nullptr, quint8 lawSpotNumber = 0);
    ~W_LawBoard();

private:
    virtual QRect getLawSlot(quint8 slotIndex)=0;

private:
    qreal mWithHeightRatio;
    QPixmap mImage;
};

class W_DeathEaterBoard : public W_LawBoard
{
    Q_OBJECT

public:
    W_DeathEaterBoard(QWidget *parent = nullptr):W_LawBoard(parent, SPOT_NUMBER_MANGEMORT_BOARD){};
    ~W_DeathEaterBoard(){};

protected:
    void paintEvent(QPaintEvent *)override;

private:
    virtual QRect getLawSlot(quint8 slotIndex)override;

private:
    QRect mArea;
};

class W_PhenixOrderBoard : public W_LawBoard
{
    Q_OBJECT

public:
    W_PhenixOrderBoard(QWidget *parent = nullptr):W_LawBoard(parent, SPOT_NUMBER_PHENIX_ORDER_BOARD){};
    ~W_PhenixOrderBoard(){};

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
