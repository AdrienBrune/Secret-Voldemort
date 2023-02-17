#include "c_gametracker.h"

C_GameTracker::C_GameTracker(QWidget *parent)
    : QWidget(parent)
    , mResumeEventLabel("")
    , mResumeEvent(C_Message_Event::ApplicationStarted)
    , mGameEvent(C_Message_Event::ApplicationStarted)
{

}

C_GameTracker::~C_GameTracker()
{

}
