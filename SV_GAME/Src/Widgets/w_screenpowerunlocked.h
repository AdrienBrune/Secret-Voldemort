#ifndef W_SCREENPOWERUNLOCKED_H
#define W_SCREENPOWERUNLOCKED_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>

#include "../../Shared/Components/c_lawboard.h"
#include "Handlers/c_soundhandler.h"

#define TIMEOUT_SCREEN_POWER_TRIGGER    6000

#define DASH_OFFSET_MAX     20
#define ANIMATION_STEP_MAX  20

#define NUMBER_POWER    5

class W_Banner : public QWidget
{
    Q_OBJECT

public:
    explicit W_Banner(QWidget *parent = nullptr, C_LawBoard::E_POWER power = C_LawBoard::E_POWER::none)
        : QWidget(parent)
        , mPower(power)
        , mAnimationStep(0)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &W_Banner::onAnimate);
        timer->start(200);

        C_SoundHandler::getInstance()->playSound(E_SOUNDS::notify);

        show();
    };
    ~W_Banner(){};

public:
    const C_LawBoard::E_POWER &getPower()const{ return mPower; };

private slots:
    void onAnimate(){
        if(++mAnimationStep > ANIMATION_STEP_MAX)
            mAnimationStep = 0;
        update();
    };

protected:
    void paintEvent(QPaintEvent *)override;

private:
    C_LawBoard::E_POWER mPower;
    quint8 mAnimationStep;
    QColor mColor[NUMBER_POWER] = {
        QColor(0, 0, 0),
        QColor(71, 116, 46),
        QColor(138, 68, 203),
        QColor(81, 112, 225),
        QColor(186, 26, 26)
    };
    QString mLabelImage[NUMBER_POWER] = {
        "",
        "Coup d'état",
        "Clairvoyance",
        "Espionnage",
        "Assassinat"
    };
};

class W_ScreenPowerUnlocked : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenPowerUnlocked(QWidget *parent = nullptr, C_LawBoard::E_POWER power = C_LawBoard::E_POWER::none);
    ~W_ScreenPowerUnlocked();

signals:
    void sig_timeout(C_LawBoard::E_POWER);

protected:
    void paintEvent(QPaintEvent *)override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setOpacity(0.8);
        painter.setBrush(QColor(20, 20, 20));
        painter.drawRect(rect());
    };
    void resizeEvent(QResizeEvent *)override
    {
        wBanner->setGeometry(0, height()/4, width(), height()/2);
    };

private:
    W_Banner *wBanner;
};

#endif // W_SCREENPOWERUNLOCKED_H
