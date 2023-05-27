#include "c_gamehandler.h"
#include "../../Shared/debug.h"

C_GameHandler *C_GameHandler::mGameHandler = nullptr;

C_GameHandler::C_GameHandler(QWidget *parent)
    : QWidget(parent)
    , mElectionTracker(nullptr)
    , mGameTracker(nullptr)
{
    mGameHandler = this;
}

C_GameHandler::~C_GameHandler()
{

}

C_GameHandler *C_GameHandler::getInstance()
{
    if(!mGameHandler)
        return mGameHandler = new C_GameHandler();
    else
        return mGameHandler;
}
