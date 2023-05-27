#include "c_remoteinterfacehandler.h"

C_RemoteInterfaceHandler *C_RemoteInterfaceHandler::mRemoteInterfaceHandler = nullptr;

C_RemoteInterfaceHandler::C_RemoteInterfaceHandler(QObject *parent)
    : QObject(parent)
    , mClientTcp(nullptr)
{
    mRemoteInterfaceHandler = this;

    mClientTcp = new C_ClientTcp(this);
    connect(mClientTcp, &C_ClientTcp::sig_MessageReceived, this, &C_RemoteInterfaceHandler::onMessageReceived);
    connect(mClientTcp, &C_ClientTcp::sig_updateView, this, &C_RemoteInterfaceHandler::sig_updateView);
    connect(mClientTcp, &C_ClientTcp::sig_connected, this, &C_RemoteInterfaceHandler::sig_connected);
    connect(mClientTcp, &C_ClientTcp::sig_disconnected, this, &C_RemoteInterfaceHandler::sig_disconnected);
}

C_RemoteInterfaceHandler::~C_RemoteInterfaceHandler()
{

}

C_RemoteInterfaceHandler *C_RemoteInterfaceHandler::getInstance()
{
    if(!mRemoteInterfaceHandler)
        return mRemoteInterfaceHandler = new C_RemoteInterfaceHandler();
    else
        return mRemoteInterfaceHandler;
}

void C_RemoteInterfaceHandler::onMessageReceived(C_Message *message)
{
    C_Message_Notification *notif = dynamic_cast<C_Message_Notification*>(message);
    if(notif)
    {
        emit sig_notif(notif->getStrMessage());
    }
    C_Message_Event *event = dynamic_cast<C_Message_Event*>(message);
    if(event)
    {
        emit sig_event(event->getEvent(), event->getData());
    }

    if(message)
        delete message;
}
