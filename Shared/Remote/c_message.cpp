#include "c_message.h"
#include "../../Shared/Components/c_stack.h"
#include "../../Shared/Remote/serialization.h"
#ifdef CLIENT_FEATURES
#include "Handlers/c_gamehandler.h"
#endif

/*-----------------------------------------------------------------------------*/
/*                         C_Message_UpdatePlayers                             */
/*-----------------------------------------------------------------------------*/

void C_Message_UpdatePlayers::serialize(QDataStream& stream)const
{
    C_Message::serialize(stream);

    quint8 number = mPlayers->size();
    stream << number;
    for(quint32 i = 0; i < number; i++)
        stream << *(*mPlayers)[i];
}
#ifdef CLIENT_FEATURES
void C_Message_UpdatePlayers::deserialize(QDataStream& stream)
{
    //C_Message::deserialize(stream);

    quint8 number;
    C_Player* playerToAdd;
    stream >> number;
    for(int i = 0; i < number; i++)
    {
        if(mPlayers->size()-1 < i) // Player to add
        {
            playerToAdd = new W_Player(C_PlayersHandler::getInstance());
            stream >> *playerToAdd;
            C_PlayersHandler::getInstance()->addPlayerInList(playerToAdd);
        }
        else
        {
            stream >> *(*mPlayers)[i]; // Player to modify
        }
    }
    for(int i = number; i < mPlayers->size(); i++) // Player to remove
    {
        C_PlayersHandler::getInstance()->removePlayerInList();
    }

    // Set the client identifier to be identified in player list
    C_PlayersHandler::getInstance()->setMyIdentifier(mTarget);
}
#endif

/*-----------------------------------------------------------------------------*/
/*                          C_Message_UpdateBoard                              */
/*-----------------------------------------------------------------------------*/

void C_Message_UpdateBoard::serialize(QDataStream &stream) const
{
    C_Message::serialize(stream);
    stream << *mStack << *mPhenixOrderBoard << *mDeathEaterBoard;
}
#ifdef CLIENT_FEATURES
void C_Message_UpdateBoard::deserialize(QDataStream &stream)
{
    //C_Message::deserialize(stream);
    stream >> *C_BoardHandler::getInstance()->getStack() >> *C_BoardHandler::getInstance()->getPhenixOrderBoard() >> *C_BoardHandler::getInstance()->getDeathEaterBoard();
}
#endif

/*-----------------------------------------------------------------------------*/
/*                           C_Message_UpdateGame                              */
/*-----------------------------------------------------------------------------*/

void C_Message_UpdateGame::serialize(QDataStream &stream) const
{
    C_Message::serialize(stream);
    stream << *mElectionTracker << *mGameTracker;
}
#ifdef CLIENT_FEATURES
void C_Message_UpdateGame::deserialize(QDataStream &stream)
{
    //C_Message::deserialize(stream);
    stream >> *C_GameHandler::getInstance()->getElectionTracker() >> *C_GameHandler::getInstance()->getGameTracker();
}
#endif

/*-----------------------------------------------------------------------------*/
/*                         C_Message_Notification                              */
/*-----------------------------------------------------------------------------*/

void C_Message_Notification::serialize(QDataStream& stream)const
{
    C_Message::serialize(stream);
    stream << mStrMessage;
}
void C_Message_Notification::deserialize(QDataStream& stream)
{
    //C_Message::deserialize(stream);
    stream >> mStrMessage;
}

/*-----------------------------------------------------------------------------*/
/*                             C_Message_Event                                 */
/*-----------------------------------------------------------------------------*/

void C_Message_Event::serialize(QDataStream& stream)const
{
    C_Message::serialize(stream);
    stream << mEvent << mData;
}
void C_Message_Event::deserialize(QDataStream& stream)
{
    //C_Message::deserialize(stream);
    stream >> mEvent >> mData;
}




