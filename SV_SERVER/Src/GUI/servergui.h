#ifndef SERVERGUI_H
#define SERVERGUI_H

#include <QWidget>
#include <QPainter>
#include <QHoverEvent>

#include "../../Shared/Components/c_player.h"
#include "../../Shared/Components/c_stack.h"
#include "../../Shared/Components/c_lawboard.h"

class GUI_Player : public QWidget
{
    Q_OBJECT

#define SIZE_BIG    250
#define SIZE_SMALL  60

public:
    explicit GUI_Player(QWidget *parent = nullptr, C_Player *player = nullptr);
    ~GUI_Player(){}

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *){ mHover = true; if(mPlayer->getRole()){setHeight(SIZE_BIG);} update(); }
    void leaveEvent(QEvent *){ mHover = false; setHeight(SIZE_SMALL); update(); }

private slots:
    void onUpdateGUI();

private:
    void setHeight(int height)
    {
        setMinimumHeight(height);
        setMaximumHeight(height);
    }

private:
    C_Player *mPlayer;
    bool mHover;
};

class GUI_Stack : public QWidget
{
    Q_OBJECT

#define UNASSIGNED  (-1)

public:
    explicit GUI_Stack(QWidget *parent = nullptr, C_Stack *stack = nullptr)
        : QWidget(parent)
        , mStack(stack)
        , mHover(false)
        , mIndexCurrentLawDisplayed(UNASSIGNED)
    {}
    ~GUI_Stack(){}

public:
    C_Stack *getStack(){ return mStack; }
    void setStack(C_Stack *stack){ mStack = stack; }

protected:
    void paintEvent(QPaintEvent *event)override;
    void enterEvent(QEvent *)override{ mHover = true; mIndexCurrentLawDisplayed = mStack->getStack().size()-1; update(); }
    void leaveEvent(QEvent *)override{ mHover = false; mIndexCurrentLawDisplayed = UNASSIGNED; update(); }
    void wheelEvent(QWheelEvent *event)override
    {
        if(mHover)
        {
            if(event->angleDelta().y() > 0)
            {
                // Forward
                if(mIndexCurrentLawDisplayed < mStack->getStack().size()-1)
                {
                    mIndexCurrentLawDisplayed++;
                    update();
                }
            }
            else
            {
                // Backward
                if(mIndexCurrentLawDisplayed > 0)
                {
                    mIndexCurrentLawDisplayed--;
                    update();
                }
            }
        }
    }

private slots:
    void onUpdateGUI(){ update(); }

private:
    C_Stack *mStack;
    bool mHover;
    qint8 mIndexCurrentLawDisplayed;
};





class GUI_LawBoard : public QWidget
{
    Q_OBJECT

#define UNASSIGNED  (-1)

public:
    explicit GUI_LawBoard(QWidget *parent = nullptr, C_LawBoard *board = nullptr)
        : QWidget(parent)
        , mLawBoard(board)
    {}
    ~GUI_LawBoard(){}

public:
    C_LawBoard *getLawBoard(){ return mLawBoard; }
    void setLawBoard(C_LawBoard *stack){ mLawBoard = stack; }

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void onUpdateGUI(){ update(); }

private:
    QRect getDeathEaterLawSlot(quint8 slotIndex);
    QRect getPhenixOrderLawSlot(quint8 slotIndex);

private:
    C_LawBoard *mLawBoard;
};

#endif // SERVERGUI_H
