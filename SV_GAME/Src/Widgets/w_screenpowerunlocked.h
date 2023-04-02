#ifndef W_SCREENPOWERUNLOCKED_H
#define W_SCREENPOWERUNLOCKED_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>

#include "../../Shared/Components/c_lawboard.h"
#include "Handlers/c_soundhandler.h"

#define TIMEOUT_SCREEN_POWER_TRIGGER    7000
#define ANIMATION_STEP_MAX  1.0

class W_Banner : public QWidget
{
    Q_OBJECT

public:
    explicit W_Banner(QWidget *parent = nullptr, C_LawBoard::E_POWER power = C_LawBoard::E_POWER::none)
        : QWidget(parent)
        , mPower(power)
        , mAnimationStep(0)
    {
        C_SoundHandler::getInstance()->playSound(E_SOUNDS::notify);

        show();

        /* Setup animation */
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &W_Banner::onAnimate);
        timer->start(1500);
    };
    ~W_Banner(){};

public:
    const C_LawBoard::E_POWER &getPower()const{ return mPower; };

private slots:
    void onAnimate()
    {
        if(mAnimationStep < ANIMATION_STEP_MAX)
        {
            mAnimationStep++;
            update();
        }
        if(mAnimationStep == ANIMATION_STEP_MAX)
        {
            QTimer *timer = dynamic_cast<QTimer*>(sender());
            if(timer)
                timer->stop();
            C_SoundHandler::getInstance()->playSound(E_SOUNDS::unlock);
        }
    };

protected:
    void paintEvent(QPaintEvent *)override;

private:
    C_LawBoard::E_POWER mPower;
    quint16 mAnimationStep;
    QColor mColor[C_LawBoard::E_POWER::powersNumber] = {
        QColor(0, 0, 0),
        QColor(71, 116, 46),
        QColor(138, 68, 203),
        QColor(81, 112, 225),
        QColor(186, 26, 26)
    };
    QString mLabelImage[C_LawBoard::E_POWER::powersNumber] = {
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
