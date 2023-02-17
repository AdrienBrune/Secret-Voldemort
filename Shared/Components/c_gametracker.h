#ifndef C_GAMETRACKER_H
#define C_GAMETRACKER_H

#include <QWidget>
#include "../Remote/c_message.h"
#include "../../Shared/debug.h"

#define STEP_SKIPPED ""



class C_GameTracker : public QWidget
{
    Q_OBJECT

public:
    C_GameTracker(QWidget *parent = nullptr);
    ~C_GameTracker();

public:
    const C_Message_Event::E_EVENT &getEvent()const{ return mGameEvent; };
    void setEvent(C_Message_Event::E_EVENT event)
    {
        mGameEvent = event;

        if(event == C_Message_Event::E_EVENT::SC_power_assassination
        || event == C_Message_Event::E_EVENT::SC_power_clairvoyance
        || event == C_Message_Event::E_EVENT::SC_power_spying
        || event == C_Message_Event::E_EVENT::SC_power_substitute_Minister)
            event = C_Message_Event::E_EVENT::SC_power_trigger;

        if(!isSkipEvent(event))
        {
            mResumeEvent = event;
            mResumeEventLabel = mSteps[event];
            update();
        }
    }

protected:
    quint8 getNumberStepToDisplay()const
    {
        quint8 number = 0;
        for(int i = 0; i < C_Message_Event::E_EVENT::event_number; i++)
        {
            if(!isSkipEvent(static_cast<C_Message_Event::E_EVENT>(i)))
                number++;
        }
        return number;
    };
    bool isSkipEvent(C_Message_Event::E_EVENT event)const
    {
        if(event < C_Message_Event::E_EVENT::event_number)
        {
            if( mSteps[static_cast<quint8>(event)] == STEP_SKIPPED)
            {
                return true;
            }
            else
                return false;
        }
        else
        {
            LOG_DBG("Error : Event out of bounds");
            return true;
        }
    };

protected:
    QString mSteps[C_Message_Event::E_EVENT::event_number] {
        STEP_SKIPPED,
        "> Election du Directeur", //    SC_Director_selection_begin,
        STEP_SKIPPED, //    CS_Director_selected,
        STEP_SKIPPED, //    SC_Director_election_begin,
        STEP_SKIPPED, //    CS_player_voted,
        STEP_SKIPPED, //    SC_Director_not_elected,
        STEP_SKIPPED, //    SC_Director_elected,
        STEP_SKIPPED, //    SC_stack_shuffled,
        "> Vote d'une loi",     //    SC_Minister_draw,
        STEP_SKIPPED, //    CS_Minister_drew,
        STEP_SKIPPED, //    SC_Minister_discard,
        STEP_SKIPPED, //    CS_Minister_discarded,
        STEP_SKIPPED, //    SC_Director_discard,
        STEP_SKIPPED, //    CS_Director_asked_veto,
        STEP_SKIPPED, //    SC_Director_asked_veto,
        STEP_SKIPPED, //    CS_Minister_accept_veto,
        STEP_SKIPPED, //    CS_Minister_refuse_veto,
        STEP_SKIPPED, //    SC_Minister_accepted_veto,
        STEP_SKIPPED, //    SC_Minister_refused_veto,
        STEP_SKIPPED, //    SC_election_tracker_incremented,
        STEP_SKIPPED, //    SC_election_tracker_triggered,
        "> Utilisation d'un pouvoir", //    SC_power_trigger,
        STEP_SKIPPED, //    SC_power_substitute_Minister,
        STEP_SKIPPED, //    CS_substitute_Minister_selected,
        STEP_SKIPPED, //    SC_power_clairvoyance,
        STEP_SKIPPED, //    CS_clairvoyance_used,
        STEP_SKIPPED, //    SC_power_spying,
        STEP_SKIPPED, //    CS_spying_carried_out,
        STEP_SKIPPED, //    SC_player_spied,
        STEP_SKIPPED, //    SC_power_assassination,
        STEP_SKIPPED, //    CS_player_murdered,
        STEP_SKIPPED, //    SC_player_murdered,
        "> Fin du tour", //    SC_end_of_turn,
        STEP_SKIPPED, //    SC_next_turn_begin,
        "> Fin de la partie",  //    SC_game_finished,
        STEP_SKIPPED  //    CS_set_name
    };
    QString mResumeEventLabel;
    C_Message_Event::E_EVENT mResumeEvent;
    C_Message_Event::E_EVENT mGameEvent;
};

#endif // C_GAMETRACKER_H
