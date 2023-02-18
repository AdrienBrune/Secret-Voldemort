#include "serialization.h"
#include "../../Shared/Components/c_lawboard.h"
#include "../../Shared/Components/c_stack.h"
#include "../../Shared/Components/c_player.h"
#include "../../Shared/Components/c_electiontracker.h"
#include "../../Shared/Components/c_gametracker.h"

/*-----------------------------------------------------------------------------*/
/*                                C_Player                                     */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_Player &instance)
{
    dbg.nospace() << "C_Player{\n"
                  << "    Name     : " << instance.getName() << "\n"
                  << "    Role     : " << instance.getRole() << "\n"
                  << "    Status   : " << instance.getRole() << "\n"
                  << "    Position : " << instance.getPosition() << "\n"
                  << "    voted    : " << instance.getFlagVote() << "\n"
                  << "    focus    : " << instance.getFlagFocus() << "\n"
                  << "}\n";
    return dbg.maybeSpace();
}

QDataStream & operator<<(QDataStream & stream, const C_Player &player)
{
    return stream << player.getName() << player.getRole() << player.getStatus() << player.getPosition() << player.getFlagVote() << player.getFlagFocus();
}

QDataStream & operator>>(QDataStream & stream, C_Player &instance)
{
    QString name;
    C_Player::E_ROLE role;
    C_Player::E_STATUS status;
    C_Player::E_POSITION position;
    bool flag, focus;

    stream >> name >> role >> status >> position >> flag >> focus;
    instance.setName(name);
    instance.setRole(role);
    instance.setStatus(status);
    instance.setPosition(position);
    instance.setFlagVote(flag);
    instance.setFlagFocus(focus);

    return stream;
}

/*-----------------------------------------------------------------------------*/
/*                                  C_Stack                                     */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, C_Stack &instance)
{
    dbg.nospace() << "C_Stack{(under) ";
    for(int i = 0; i < instance.getStack().size(); i++)
        dbg << instance.seeCard(i).getFaction() << ", ";
    dbg.nospace() << "(above)}\n"
                  << "Legend -> [ Phenix Order : 1], "
                  << "[DeathEater : 2]\n";
    return dbg.maybeSpace();
}

QDataStream & operator<<(QDataStream & stream, C_Stack &instance)
{
    stream << static_cast<quint8>(instance.getStack().size());
    for(int i = 0; i < instance.getStack().size(); i++)
        stream << static_cast<quint8>(instance.seeCard(i).getFaction());

    return stream;
}

QDataStream & operator>>(QDataStream & stream, C_Stack &instance)
{
    quint8 stackSize, type;
    QList<C_LawCard> newStack;

    stream >> stackSize;
    for(int i = 0; i < stackSize; i++)
    {
        stream >> type;
        newStack.append(type == C_LawCard::E_FACTION::deathEater ? C_LawCard(C_LawCard::E_FACTION::deathEater) : C_LawCard(C_LawCard::E_FACTION::phenixOrder));
    }
    instance.setStack(newStack);

    return stream;
}

/*-----------------------------------------------------------------------------*/
/*                               C_LawBoard                                    */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_LawBoard &instance)
{
    C_LawBoard::E_POWER power = C_LawBoard::E_POWER::none;

    instance.getLawSpotNumber() == SPOT_NUMBER_PHENIX_ORDER_BOARD ? dbg.maybeSpace() << "Phenix Order Board\n" : dbg.maybeSpace() << "Death Eater Board\n";

    for(quint8 i = 0; i < instance.getCardsOnBoard(); i++)
    {
        dbg.maybeSpace() << "[Law] ";
    }
    for(quint8 i = instance.getCardsOnBoard(); i < instance.getLawSpotNumber(); i++)
    {
#ifdef CLIENT_FEATURE
        instance.checkLawSpotPower(i);
#else
        power = C_LawBoard::E_POWER::none;
#endif
        switch(power)
        {
            case C_LawBoard::E_POWER::assassination:
                dbg.maybeSpace() << "[Assassination]";
                break;
            case C_LawBoard::E_POWER::clairvoyance:
                dbg.maybeSpace() << "[Clairvoyance]";
                break;
            case C_LawBoard::E_POWER::spying:
                dbg.maybeSpace() << "[Spying]";
                break;
            case C_LawBoard::E_POWER::substituteMinister:
                dbg.maybeSpace() << "[Minister substitute]";
                break;
            default:
                dbg.maybeSpace() << "[ ]";
                break;
        }
    }
    return dbg.maybeSpace() << "\n" ;
}

QDataStream & operator<<(QDataStream & stream, const C_LawBoard &instance)
{
    quint8 cardsOnBoard = instance.getCardsOnBoard(), flagPower = instance.getFlagPower();

    return stream << cardsOnBoard << flagPower;
}

QDataStream & operator>>(QDataStream & stream, C_LawBoard &instance)
{
    quint8 cardsOnBoard, flagPower;

    stream >> cardsOnBoard >> flagPower;
    instance.setCardsOnBoard(cardsOnBoard);
    instance.setFlagPower(flagPower);
    return stream;
}

/*-----------------------------------------------------------------------------*/
/*                           C_ElectionTracker                                 */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_ElectionTracker &instance)
{
    dbg.maybeSpace() << "Election tracker : ";
    for(quint8 i = 0; i < instance.getCounter(); i++)
        dbg.maybeSpace() << "X ";
    return dbg.maybeSpace() << "\n" ;
}

QDataStream & operator<<(QDataStream & stream, const C_ElectionTracker &instance)
{
    quint8 counter = instance.getCounter();
    bool flag = instance.getFlag();

    return stream << counter << flag;
}

QDataStream & operator>>(QDataStream & stream, C_ElectionTracker &instance)
{
    quint8 counter;
    bool flag;

    stream >> counter >> flag;
    instance.setCounter(counter);
    instance.setFlag(flag);
    return stream;
}


/*-----------------------------------------------------------------------------*/
/*                               C_GameTracker                                 */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_GameTracker &instance)
{
    return dbg.maybeSpace() << "Game tracker : " << instance.getEvent();
}

QDataStream & operator<<(QDataStream & stream, const C_GameTracker &instance)
{
    quint8 event = instance.getEvent();

    return stream << event;
}

QDataStream & operator>>(QDataStream & stream, C_GameTracker &instance)
{
    quint8 event;

    stream >> event;
    instance.setEvent(static_cast<C_Message_Event::E_EVENT>(event));
    return stream;
}
