#include "c_lawboard.h"
#ifdef CLIENT_FEATURES
#include "Handlers/c_playershandler.h"
#endif
#include "../../Shared/debug.h"

C_LawBoard::E_POWER gBoard5To6Players[SPOT_NUMBER_MANGEMORT_BOARD] = {
    C_LawBoard::E_POWER::none,
    C_LawBoard::E_POWER::none,
    C_LawBoard::E_POWER::clairvoyance,
    C_LawBoard::E_POWER::assassination,
    C_LawBoard::E_POWER::assassination,
    C_LawBoard::E_POWER::none
};

C_LawBoard::E_POWER gBoard7To8Players[SPOT_NUMBER_MANGEMORT_BOARD] = {
    C_LawBoard::E_POWER::none,
    C_LawBoard::E_POWER::spying,
    C_LawBoard::E_POWER::substituteMinister,
    C_LawBoard::E_POWER::assassination,
    C_LawBoard::E_POWER::assassination,
    C_LawBoard::E_POWER::none
};

C_LawBoard::E_POWER gBoard9To10Players[SPOT_NUMBER_MANGEMORT_BOARD] = {
    C_LawBoard::E_POWER::spying,
    C_LawBoard::E_POWER::spying,
    C_LawBoard::E_POWER::substituteMinister,
    C_LawBoard::E_POWER::assassination,
    C_LawBoard::E_POWER::assassination,
    C_LawBoard::E_POWER::none
};

C_LawBoard::C_LawBoard(QWidget *parent, quint8 lawSpotNumber)
    : QWidget(parent)
    , mLawSpotNumber(lawSpotNumber)
    , mCardsOnBoard(0)
    , mFlagPowerChecked(true)
{
    hide();
}

C_LawBoard::~C_LawBoard()
{

}

void C_LawBoard::addCardOnBoard()
{
    if(mCardsOnBoard >= mLawSpotNumber)
    {
        LOG_DBG("Error : Card not added, board already full");
        return;
    }

    mCardsOnBoard++;
    mFlagPowerChecked = false;
    emit sig_cardPut();
    emit sig_updateGUI();
}

#ifdef CLIENT_FEATURES
C_LawBoard::E_POWER C_LawBoard::checkForPower()
#else
C_LawBoard::E_POWER C_LawBoard::checkForPower(const quint8 &numberPlayers)
#endif
{
    E_POWER power = E_POWER::none;

    if(!mFlagPowerChecked)
    {

#ifdef CLIENT_FEATURES
        quint8 numberPlayers = C_PlayersHandler::getInstance()->getPlayers()->size();
#endif
        if(mCardsOnBoard <= SPOT_NUMBER_MANGEMORT_BOARD)
        {
            switch(numberPlayers)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    LOG_DBG(QString("Error : player number too low - %1").arg(numberPlayers));
                    return E_POWER::none;

                case 5:
                case 6:
                    power = gBoard5To6Players[mCardsOnBoard ? mCardsOnBoard-1 : 0];
                break;

                case 7:
                case 8:
                    power = gBoard7To8Players[mCardsOnBoard ? mCardsOnBoard-1 : 0];
                break;

                case 9:
                case 10:
                    power = gBoard9To10Players[mCardsOnBoard ? mCardsOnBoard-1 : 0];
                break;

                default:
                    LOG_DBG(QString("Error : player number too high - %1").arg(numberPlayers));
                    return E_POWER::none;
            }

            mFlagPowerChecked = true;
            return power;
        }
        else
        {
            LOG_DBG(QString("Error : card number on board out of range - %1").arg(mCardsOnBoard));
            return E_POWER::none;
        }
    }
    else
        return E_POWER::none;
}

#ifdef CLIENT_FEATURES
C_LawBoard::E_POWER C_LawBoard::checkLawSpotPower(const quint8 &spotIndex)const
#else
C_LawBoard::E_POWER C_LawBoard::checkLawSpotPower(const quint8 &spotIndex, const quint8 &numberPlayers)
#endif
{
    E_POWER power = E_POWER::none;
#ifdef CLIENT_FEATURES
    quint8 numberPlayers = C_PlayersHandler::getInstance()->getPlayers()->size();
#endif

    if(spotIndex < mLawSpotNumber)
    {
        switch(numberPlayers)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                LOG_DBG(QString("Error : player number too low - %1").arg(numberPlayers));
                return E_POWER::none;

            case 5:
            case 6:
                power = gBoard5To6Players[spotIndex];
            break;

            case 7:
            case 8:
                power = gBoard7To8Players[spotIndex];
            break;

            case 9:
            case 10:
                power = gBoard9To10Players[spotIndex];
            break;

            default:
                LOG_DBG(QString("Error : player number too high - %1").arg(numberPlayers));
                return E_POWER::none;
        }
        return power;
    }
    else
    {
        LOG_DBG(QString("Error : spotIndex out of range - %1").arg(spotIndex));
        return E_POWER::none;
    }
}
