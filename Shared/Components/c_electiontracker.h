#ifndef C_ELECTIONTRACKER_H
#define C_ELECTIONTRACKER_H

#include <QWidget>

class C_ElectionTracker : public QWidget
{
    Q_OBJECT

public:
    C_ElectionTracker(QWidget *parent = nullptr);
    ~C_ElectionTracker();

public:
    const quint8 &getCounter()const{ return mCounter; };
    void setCounter(const quint8 &counter){
        counter > 3 ? mCounter = 3 : mCounter = counter;
        if(mCounter >= 3)
            mFlag = true;
        else
            mFlag = false;
    };

    const bool &getFlag()const{ return mFlag; };
    void setFlag(const bool &flag){ mFlag = flag; };

protected:
    quint8 mCounter;
    bool mFlag;
};

#endif // C_ELECTIONTRACKER_H
