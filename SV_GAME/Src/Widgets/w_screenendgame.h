#ifndef W_SCREENENDGAME_H
#define W_SCREENENDGAME_H

#include "../../Shared/Components/c_stack.h"
#include <QWidget>
#include <QPainter>

#define ANIMATION_STEP_MAX  50.0

class W_ScreenEndGame : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenEndGame(QWidget *parent = nullptr, C_LawCard::E_FACTION faction = C_LawCard::E_FACTION::notDefined);
    ~W_ScreenEndGame();

signals:
    void sig_timeout();

private slots:
    void onAnimate();

protected:
    void paintEvent(QPaintEvent *)override;

private:
    quint8 mFactionWon;
    quint8 mAnimationStep;
};

#endif // W_SCREENENDGAME_H
