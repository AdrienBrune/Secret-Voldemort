#ifndef C_BOARDHANDLER_H
#define C_BOARDHANDLER_H

#include <QWidget>
#include <QPainter>

#include "Widgets/Components/w_stack.h"
#include "Widgets/Components/w_lawboard.h"

/* Singleton implementation */
class C_BoardHandler : public QWidget
{
    Q_OBJECT

protected:
    C_BoardHandler(QWidget *parent = nullptr);
public:
    C_BoardHandler(C_BoardHandler &other) = delete;
    void operator=(const C_BoardHandler &) = delete;
    ~C_BoardHandler();

public:
    static C_BoardHandler *getInstance();

public:
    C_Stack *getStack()const{ return mStack; };
    void setStack(C_Stack* stack){ mStack = stack; };;
    C_LawBoard *getDeathEaterBoard()const{ return mDeathEaterBoard; };
    void setDeathEaterBoard(C_LawBoard *board){ mDeathEaterBoard = board; };
    C_LawBoard *getPhenixOrderBoard()const{ return mPhenixOrderBoard; };
    void setPhenixOrderBoard(C_LawBoard *board){ mPhenixOrderBoard = board; };

protected:
    static C_BoardHandler *mBoardHandler;

private:
    C_Stack *mStack;
    C_LawBoard *mDeathEaterBoard;
    C_LawBoard *mPhenixOrderBoard;
};

#endif // C_BOARDHANDLER_H
