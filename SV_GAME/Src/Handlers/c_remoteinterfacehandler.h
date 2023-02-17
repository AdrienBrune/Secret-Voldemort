#ifndef C_REMOTEINTERFACEHANDLER_H
#define C_REMOTEINTERFACEHANDLER_H

#include <QObject>

#include "Remote/c_clienttcp.h"

class C_RemoteInterfaceHandler : public QObject
{
    Q_OBJECT

public:
    C_RemoteInterfaceHandler(QObject *parent = nullptr);
    C_RemoteInterfaceHandler(C_RemoteInterfaceHandler &other) = delete;
    void operator=(const C_RemoteInterfaceHandler &) = delete;
    ~C_RemoteInterfaceHandler();

signals:
    void sig_event(C_Message_Event::E_EVENT, QByteArray);
    void sig_notif(QString);
    void sig_connected();
    void sig_disconnected();
    void sig_updateView();

public:
    static C_RemoteInterfaceHandler *getInstance();

public:
    C_ClientTcp *getClient()const{ return mClientTcp; };

private slots:
    void onMessageReceived(C_Message *message);

protected:
    static C_RemoteInterfaceHandler *mRemoteInterfaceHandler;

private:
    C_ClientTcp *mClientTcp;
};

#endif // C_REMOTEINTERFACEHANDLER_H
