#ifndef C_MANGEMORTBOARD_H
#define C_MANGEMORTBOARD_H

#include <QWidget>

#define SPOT_NUMBER_MANGEMORT_BOARD     6
#define SPOT_NUMBER_PHENIX_ORDER_BOARD  5

class C_LawBoard : public QWidget
{
    Q_OBJECT

public:
    enum E_POWER
    {
        none,
        substituteMinister,
        clairvoyance,
        spying,
        assassination,
        powersNumber
    };

public:
    C_LawBoard(QWidget *parent = nullptr, quint8 lawSpotNumber = 0);
    virtual~C_LawBoard();

signals:
    void sig_cardPut();
    void sig_updateGUI();
    void sig_printLog(QString);

public:
    const quint8 &getCardsOnBoard()const{ return mCardsOnBoard; };
    void setCardsOnBoard(const quint8 &cardsOnBoard)
    {
        mCardsOnBoard = cardsOnBoard;
        if(mCardsOnBoard > mLawSpotNumber)
            mCardsOnBoard = mLawSpotNumber;
        emit sig_updateGUI();
    };

    const quint8 &getLawSpotNumber()const{ return mLawSpotNumber; };

    const bool &getFlagPower()const{ return mFlagPowerChecked; };
    void setFlagPower(const bool &flag){ mFlagPowerChecked = flag; };

public:
    void addCardOnBoard();
#ifdef CLIENT_FEATURES
    virtual E_POWER checkForPower();
    virtual E_POWER checkLawSpotPower(const quint8 &spotIndex)const;
#else
    virtual E_POWER checkForPower(const quint8 &numberPlayers);
    virtual E_POWER checkLawSpotPower(const quint8 &spotIndex, const quint8 &numberPlayers);
#endif

protected:
    quint8 mLawSpotNumber;
    quint8 mCardsOnBoard;
    bool mFlagPowerChecked;
};

class C_DeathEaterBoard : public C_LawBoard
{
    Q_OBJECT

public:
    C_DeathEaterBoard(QWidget *parent = nullptr):C_LawBoard(parent, SPOT_NUMBER_MANGEMORT_BOARD){};
    ~C_DeathEaterBoard(){};
};

class C_PhenixOrderBoard : public C_LawBoard
{
    Q_OBJECT

public:
    C_PhenixOrderBoard(QWidget *parent = nullptr):C_LawBoard(parent, SPOT_NUMBER_PHENIX_ORDER_BOARD){};
    ~C_PhenixOrderBoard(){};

public:
#ifdef CLIENT_FEATURES
    virtual E_POWER checkForPower()override{ mFlagPowerChecked = true; return E_POWER::none; };
    virtual E_POWER checkLawSpotPower(const quint8 &)const override{ return E_POWER::none; };
#else
    virtual E_POWER checkForPower(const quint8 &)override{ mFlagPowerChecked = true; return E_POWER::none; };
    virtual E_POWER checkLawSpotPower(const quint8 &, const quint8 &)override{ return E_POWER::none; };
#endif
};

#endif // C_MANGEMORTBOARD_H
