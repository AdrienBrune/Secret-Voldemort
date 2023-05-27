#ifndef W_PLAYER_H
#define W_PLAYER_H

#include <QWidget>
#include <QPainter>

#include "../../Shared/Components/c_player.h"

class W_Player : public C_Player
{
    Q_OBJECT

public:
    explicit W_Player(QWidget *parent = nullptr);
    ~W_Player();

signals:
    void sig_clicked(C_Player*);

private slots:
    void onActionRequested(bool request){ request ? mTimerLoop->start(100) : mTimerLoop->stop(); }

public:
    const bool &getClickable()const{ return mClickable; };
    void setClickable(const bool &toggle){ mClickable = toggle; };

    const qreal &getGlobalOpacity()const{ return mGlobalOpacity; };
    void setGlobalOpacity(const qreal &opacity){ mGlobalOpacity = opacity; };

protected:
    void leaveEvent(QEvent*)override;
    void enterEvent(QEvent*)override;
    void paintEvent(QPaintEvent*)override;
    void mousePressEvent(QMouseEvent*)override;

private:
    bool mHover;
    bool mClickable;
    qreal mGlobalOpacity;
    QTimer *mTimerLoop;
    quint16 mAnimationLoop;
};

#endif // W_PLAYER_H
