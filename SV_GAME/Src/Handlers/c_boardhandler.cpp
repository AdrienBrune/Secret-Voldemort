#include "Handlers/c_boardhandler.h"
#include "../../Shared/debug.h"

C_BoardHandler *C_BoardHandler::mBoardHandler = nullptr;

C_BoardHandler::C_BoardHandler(QWidget * parent)
    : QWidget(parent)
    , mStack(nullptr)
    , mDeathEaterBoard(nullptr)
    , mPhenixOrderBoard(nullptr)
{
    mBoardHandler = this;
}

C_BoardHandler::~C_BoardHandler()
{

}

C_BoardHandler *C_BoardHandler::getInstance()
{
    if(!mBoardHandler)
        return mBoardHandler = new C_BoardHandler();
    else
        return mBoardHandler;
}
