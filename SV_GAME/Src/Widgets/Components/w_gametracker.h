#ifndef W_GAMETRACKER_H
#define W_GAMETRACKER_H

#include <QWidget>
#include <QPainter>

#include "../../Shared/Components/c_gametracker.h"

class W_GameTracker : public C_GameTracker
{
    Q_OBJECT

private:
    struct S_GAME_STEP{
        QString label;
        quint8 step;
    };

public:
    explicit W_GameTracker(QWidget *parent = nullptr);
    ~W_GameTracker();

protected:
    void paintEvent(QPaintEvent *event)override;

private:
    void drawToken(QPainter *painter, quint8 step, bool isCurrentStep);

};

#endif // W_GAMETRACKER_H
