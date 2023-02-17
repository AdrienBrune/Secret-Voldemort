#ifndef W_MESSAGEDISPLAYER_H
#define W_MESSAGEDISPLAYER_H

#include <QLabel>
#include <QPainter>

class W_MessageDisplayer : public QLabel
{
    Q_OBJECT

private:
    enum E_STATE{
        iddle = 0,
        waitLogMessage,
        logMessage,
    };

public:
    explicit W_MessageDisplayer(QWidget *parent = nullptr);
    ~W_MessageDisplayer();

public:
    const QString &getMessage()const{ return mMessage; };
    void setMessage(const QString &message){ mMessage = message; };

public:
    void addMessageInQueue(const QString &message);
    void adjustMessageSize();

private slots:
    void onMachineState();

private:
    QString mMessage;
    QList<QString> mWaitingList;
    quint8 mStep;
    QTimer *mDelay;
};

#endif // W_MESSAGEDISPLAYER_H
