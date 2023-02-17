#ifndef C_MESSAGE_H
#define C_MESSAGE_H

#include <QObject>

#include "../debug.h"
#include "../../Shared/Remote/serialization.h"
#ifdef CLIENT_FEATURES
#include "Handlers/c_playershandler.h"
#include "Handlers/c_boardhandler.h"
#endif

#define ID_MSG 0xABCDEF00

class C_Player;
class C_Stack;
class C_LawBoard;
class C_ElectionTracker;

class C_Message : public QObject
{
    Q_OBJECT

public:
    enum E_ID
    {
        unknown = ID_MSG | 0,
        update_players = ID_MSG | 1,
        update_board = ID_MSG | 2,
        update_game = ID_MSG | 3,
        notification = ID_MSG | 4,
        event = ID_MSG | 5,
        acknowledge = ID_MSG | 6
    };

public:
    C_Message(E_ID identifier = E_ID::unknown, quint8 target = 0xFF)
        : QObject(nullptr)
        , mIdentifier(identifier)
        , mTarget(target)
    {};
    C_Message(C_Message* message)
        : QObject(nullptr)
        , mIdentifier(message->getIdentifier())
        , mTarget(message->getTarget())
    {
        delete message;
    };
    ~C_Message(){};

public:
    const E_ID &getIdentifier()const{ return mIdentifier; };
    void setIdentifier(const E_ID &identifier){ mIdentifier = identifier; };

    const quint8 &getTarget()const{ return mTarget; };
    void setTarget(const quint8 &target){ mTarget = target; };

protected:
    virtual void serialize(QDataStream& stream)const
    {
        stream << mIdentifier << mTarget;
    }
    virtual void deserialize(QDataStream& stream)
    {
        stream >> mIdentifier >> mTarget;
    }
    friend QDataStream& operator<<(QDataStream& stream, const C_Message& object)
    {
        object.serialize(stream);
        return stream;
    }
    friend QDataStream& operator>>(QDataStream& stream, C_Message& object)
    {
        object.deserialize(stream);
        return stream;
    }

protected:
    E_ID mIdentifier; // Message identifier
    quint8 mTarget; // Target message player
};

/*-----------------------------------------------------------------------------*/
/*                          C_Message_Acknowledge                              */
/*-----------------------------------------------------------------------------*/

class C_Message_Acknowledge : public C_Message
{
    Q_OBJECT
public:
    C_Message_Acknowledge()
        : C_Message(E_ID::acknowledge)
    {}
    ~C_Message_Acknowledge(){};
};

/*-----------------------------------------------------------------------------*/
/*                         C_Message_UpdatePlayers                             */
/*-----------------------------------------------------------------------------*/

class C_Message_UpdatePlayers : public C_Message
{
    Q_OBJECT
public:
    // Server side
    C_Message_UpdatePlayers(quint8 target = 0xFF, QList<C_Player*>* players = nullptr)
        : C_Message(E_ID::update_players, target)
        , mPlayers(players)
    {};
    // Client side
    C_Message_UpdatePlayers(C_Message *message)
        : C_Message(message)
        , mPlayers(nullptr)
    {
#ifdef CLIENT_FEATURES
        if(C_PlayersHandler::getInstance())
        {
            mPlayers = C_PlayersHandler::getInstance()->getPlayers();
        }
#endif
    };
    ~C_Message_UpdatePlayers(){};

public:
    QList<C_Player*>* getPlayers()const{ return mPlayers; };

public:
    virtual void serialize(QDataStream& stream)const;
#ifdef CLIENT_FEATURES
    virtual void deserialize(QDataStream& stream);
#endif
    friend QDataStream& operator<<(QDataStream& stream, const C_Message_UpdatePlayers& object)
    {
        object.serialize(stream);
        return stream;
    }
#ifdef CLIENT_FEATURES
    friend QDataStream& operator>>(QDataStream& stream, C_Message_UpdatePlayers& object)
    {
        object.deserialize(stream);
        return stream;
    }
#endif

private:
    QList<C_Player*> *mPlayers;
};

/*-----------------------------------------------------------------------------*/
/*                          C_Message_UpdateBoard                              */
/*-----------------------------------------------------------------------------*/

class C_Message_UpdateBoard : public C_Message
{
    Q_OBJECT
public:
    // Server side
    C_Message_UpdateBoard(C_Stack *stack = nullptr, C_LawBoard *phenixOrderBoard = nullptr, C_LawBoard *deathEaterBoard = nullptr)
        : C_Message(E_ID::update_board)
        , mStack(stack)
        , mPhenixOrderBoard(phenixOrderBoard)
        , mDeathEaterBoard(deathEaterBoard)
    {};
    // Client side
    C_Message_UpdateBoard(C_Message *message)
        : C_Message(message)
        , mStack(nullptr)
        , mPhenixOrderBoard(nullptr)
        , mDeathEaterBoard(nullptr)
    {};
    ~C_Message_UpdateBoard(){};

public:
    virtual void serialize(QDataStream& stream)const;
#ifdef CLIENT_FEATURES
    virtual void deserialize(QDataStream& stream);
#endif
    friend QDataStream& operator<<(QDataStream& stream, const C_Message_UpdateBoard& object)
    {
        object.serialize(stream);
        return stream;
    }
#ifdef CLIENT_FEATURES
    friend QDataStream& operator>>(QDataStream& stream, C_Message_UpdateBoard& object)
    {
        object.deserialize(stream);
        return stream;
    }
#endif

private:
    C_Stack *mStack;
    C_LawBoard *mPhenixOrderBoard;
    C_LawBoard *mDeathEaterBoard;
};

/*-----------------------------------------------------------------------------*/
/*                           C_Message_UpdateGame                              */
/*-----------------------------------------------------------------------------*/

class C_Message_UpdateGame : public C_Message
{
    Q_OBJECT
public:
    // Server side
    C_Message_UpdateGame(C_ElectionTracker *electionTracker = nullptr, C_GameTracker *gameTracker = nullptr)
        : C_Message(E_ID::update_game)
        , mElectionTracker(electionTracker)
        , mGameTracker(gameTracker)
    {};
    // Client side
    C_Message_UpdateGame(C_Message *message)
        : C_Message(message)
        , mElectionTracker(nullptr)
        , mGameTracker(nullptr)
    {};
    ~C_Message_UpdateGame(){};

public:
    virtual void serialize(QDataStream& stream)const;
#ifdef CLIENT_FEATURES
    virtual void deserialize(QDataStream& stream);
#endif
    friend QDataStream& operator<<(QDataStream& stream, const C_Message_UpdateGame& object)
    {
        object.serialize(stream);
        return stream;
    }
#ifdef CLIENT_FEATURES
    friend QDataStream& operator>>(QDataStream& stream, C_Message_UpdateGame& object)
    {
        object.deserialize(stream);
        return stream;
    }
#endif

private:
    C_ElectionTracker *mElectionTracker;
    C_GameTracker *mGameTracker;
};

/*-----------------------------------------------------------------------------*/
/*                         C_Message_Notification                              */
/*-----------------------------------------------------------------------------*/

class C_Message_Notification : public C_Message
{
    Q_OBJECT
public:
    // Server side
    C_Message_Notification(QString string = "")
        : C_Message(E_ID::notification)
        , mStrMessage(string)
    {};
    // Client side
    C_Message_Notification(C_Message *message)
        : C_Message(message)
        , mStrMessage(QString())
    {};
    ~C_Message_Notification(){};

public:
    const QString &getStrMessage()const{ return mStrMessage; };
    void setStrMessage(const QString &str){ mStrMessage = str; };

public:
    virtual void serialize(QDataStream& stream)const;
    virtual void deserialize(QDataStream& stream);
    friend QDataStream& operator<<(QDataStream& stream, const C_Message_Notification& object)
    {
        object.serialize(stream);
        return stream;
    }
    friend QDataStream& operator>>(QDataStream& stream, C_Message_Notification& object)
    {
        object.deserialize(stream);
        return stream;
    }

private:
    QString mStrMessage;
};

/*-----------------------------------------------------------------------------*/
/*                             C_Message_Event                                 */
/*-----------------------------------------------------------------------------*/

class C_Message_Event : public C_Message
{
    Q_OBJECT

public:
    // SC : Server to Client
    // CS : Client to Server
    enum E_EVENT{
        ApplicationStarted,
        SC_Director_selection_begin,
        CS_Director_selected,
        SC_Director_election_begin,
        CS_player_voted,
        SC_Director_not_elected,
        SC_Director_elected,
        SC_stack_shuffled,
        SC_Minister_draw,
        CS_Minister_drew,
        SC_Minister_discard,
        CS_Minister_discarded,
        SC_Director_discard,
        CS_Director_discarded,
        CS_Director_asked_veto,
        SC_Director_asked_veto,
        CS_Minister_accept_veto,
        CS_Minister_refuse_veto,
        SC_Minister_accepted_veto,
        SC_Minister_refused_veto,
        SC_election_tracker_incremented,
        SC_election_tracker_triggered,
        SC_power_trigger,
        SC_power_substitute_Minister,
        CS_substitute_Minister_selected,
        SC_power_clairvoyance,
        CS_clairvoyance_used,
        SC_power_spying,
        CS_spying_player,
        CS_spying_carried_out,
        SC_power_assassination,
        CS_assassin_player,
        SC_player_murdered,
        SC_end_of_turn,
        SC_next_turn_begin,
        SC_game_finished,
        CS_set_name,

        event_number
    };

public:
    // Server side
    C_Message_Event(C_Message_Event::E_EVENT event = E_EVENT::ApplicationStarted, QByteArray data = QByteArray())
        : C_Message(E_ID::event)
        , mEvent(event)
        , mData(data)
    {};
    // Client side
    C_Message_Event(C_Message *message)
        : C_Message(message)
        , mEvent(C_Message_Event::E_EVENT::ApplicationStarted)
        , mData(QByteArray())
    {};
    ~C_Message_Event(){};

public:
    const E_EVENT &getEvent()const{ return mEvent; };
    void setEvent(const E_EVENT &event){ mEvent = event; };

    const QByteArray &getData()const{ return mData; };
    void setData(const QByteArray &data){ mData.clear(); mData.append(data); };

public:
    virtual void serialize(QDataStream& stream)const;
    virtual void deserialize(QDataStream& stream);
    friend QDataStream& operator<<(QDataStream& stream, const C_Message_Event& object)
    {
        object.serialize(stream);
        return stream;
    }
    friend QDataStream& operator>>(QDataStream& stream, C_Message_Event& object)
    {
        object.deserialize(stream);
        return stream;
    }

private:
    E_EVENT mEvent;
    QByteArray mData;
};

#endif // C_MESSAGE_H
