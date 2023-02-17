#include "../../Shared/Components/c_stack.h"
#include <QRandomGenerator>
#include <QtDebug>

#define NUMBER_OF_CARDS             27.0
#define MANGEMORT_NUMBER_OF_CARD    22.0
#define PHENIXORDER_NUMBER_OF_CARD  8.0

C_Stack::C_Stack(QWidget *parent)
    : QWidget(parent)
    , mLawCards(QList<C_LawCard>())
{
    hide();
}

C_Stack::~C_Stack()
{

}

void C_Stack::reshuffleStack()
{
    while(!mLawCards.isEmpty())
        mLawCards.removeLast();

    quint8 deathEaterCardNumber = MANGEMORT_NUMBER_OF_CARD;
    quint8 phenixOrderCardNumber = PHENIXORDER_NUMBER_OF_CARD;
    quint8 cardPutNumber = 0;
    qreal cardRatioThreshold, randomRatio;

    do{
        cardRatioThreshold = static_cast<quint8>((static_cast<qreal>(deathEaterCardNumber) / static_cast<qreal>(MANGEMORT_NUMBER_OF_CARD + PHENIXORDER_NUMBER_OF_CARD - cardPutNumber)) * 100);
        randomRatio = static_cast<quint8>(QRandomGenerator::global()->bounded(100+1));

        if(randomRatio <= cardRatioThreshold)
        {
            if(deathEaterCardNumber != 0)
            {
                mLawCards.append(C_LawCard(C_LawCard::E_FACTION::deathEater));
                cardPutNumber++;
            }
        }
        else
        {
            if(phenixOrderCardNumber != 0)
            {
                mLawCards.append(C_LawCard(C_LawCard::E_FACTION::phenixOrder));
                cardPutNumber++;
            }
        }

    }while(cardPutNumber <= MANGEMORT_NUMBER_OF_CARD + PHENIXORDER_NUMBER_OF_CARD);
}

C_LawCard C_Stack::drawCardFromTop()
{
    return mLawCards.takeLast();
}

void C_Stack::putCardOnTop(C_LawCard law)
{
    mLawCards.append(law);
}

C_LawCard C_Stack::seeCard(quint8 index) const
{
    if(index >= mLawCards.size())
    {
        qDebug() << "Error : C_Stack::seeCard";
        return C_LawCard::E_FACTION::deathEater;
    }

    return mLawCards.at(index);
}
