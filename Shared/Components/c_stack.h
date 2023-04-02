#ifndef C_PILE_H
#define C_PILE_H

#include <QWidget>

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
    C_LawCard(E_FACTION type):mType(type){};
    ~C_LawCard(){};

public:
    const E_FACTION &getFaction()const{ return mType; };
    void setFaction(const E_FACTION &type){ mType = type; };

private:
    E_FACTION mType;
};





class C_Stack : public QWidget
{
    Q_OBJECT

public:
    C_Stack(QWidget *parent = nullptr);
    ~C_Stack();

signals:
    void sig_updateGUI();
    void sig_printLog(QString);

public:
    QList<C_LawCard> &getStack(){ return mLawCards; };
    void setStack(const QList<C_LawCard> &stack)
    {
        while(!mLawCards.isEmpty())
            mLawCards.removeLast();
        for(int i = 0; i < stack.size(); i++)
            mLawCards.append(stack.at(i));
        emit sig_updateGUI();
    };

public:
    void reshuffleStack();
    C_LawCard drawCardFromTop();
    void putCardOnTop(C_LawCard law);
    C_LawCard seeCard(quint8 index);

protected:
    QList<C_LawCard> mLawCards;
};

#endif // C_PILE_H
