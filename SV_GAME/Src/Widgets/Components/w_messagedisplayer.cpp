#include "w_messagedisplayer.h"
#include "../../Shared/c_textdrawinghelper.h"

#include <QTimer>

W_MessageDisplayer::W_MessageDisplayer(QWidget *parent)
    : QLabel(parent)
    , mMessage("Appuyer sur <echap> pour afficher le menu principal")
    , mStep(iddle)
    , mDelay(nullptr)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &W_MessageDisplayer::onMachineState);
    timer->start(100);

    mDelay = new QTimer(this);
    mDelay->setSingleShot(true);

    setStyleSheet("W_MessageDisplayer{color:white;background-color:rgba(0,0,0,0);}");
    setFont(C_AutoScaleFont("Germania", mMessage, rect(), height()/2));
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setWordWrap(true);
    setText(mMessage);
    show();
}

W_MessageDisplayer::~W_MessageDisplayer()
{

}

void W_MessageDisplayer::addMessageInQueue(const QString &message)
{
    mWaitingList.append(message);
}

void W_MessageDisplayer::adjustMessageSize()
{
    setFont(QFont("Germania", parentWidget()->height()/35));
}

void W_MessageDisplayer::onMachineState()
{
    switch(mStep)
    {
    case iddle:
        mStep = waitLogMessage;
        mDelay->stop();
        mDelay->start(1500);
        break;

    case waitLogMessage:
        if(!mDelay->isActive())
        {
           mStep = logMessage;
        }
        break;

    case logMessage:
        if(!mWaitingList.isEmpty())
        {
            setText(mWaitingList.takeFirst());
            mStep = iddle;
        }
        break;

    default:
        mStep = iddle;
        break;
    }
}
