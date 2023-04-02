#include "w_screenendgame.h"
#include "Handlers/c_soundhandler.h"

#include <QTimer>

W_ScreenEndGame::W_ScreenEndGame(QWidget *parent,C_LawCard::E_FACTION faction)
    : QWidget(parent)
    , mFactionWon(faction)
    , mAnimationStep(0)
{
    setGeometry(0, 0, parent->width(), parent->height());
    show();

    C_SoundHandler::getInstance()->playSound(E_SOUNDS::endGame);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &W_ScreenEndGame::onAnimate);
    timer->start(200);

    QTimer::singleShot(14000, this, [&](){
        emit sig_timeout();
    });
}

W_ScreenEndGame::~W_ScreenEndGame()
{

}

void W_ScreenEndGame::onAnimate()
{
    if(mAnimationStep < ANIMATION_STEP_MAX)
    {
        mAnimationStep++;
        update();
    }

    if(mAnimationStep == ANIMATION_STEP_MAX)
    {
        QTimer * timer = dynamic_cast<QTimer*>(sender());
        if(timer)
        {
            timer->stop();
        }
    }
}

void W_ScreenEndGame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QString strFaction;

    painter.setBrush(QColor(0,0,0));
    painter.setOpacity(0.8);
    painter.drawRect(rect());

    switch(mFactionWon)
    {
        case C_LawCard::E_FACTION::deathEater:
            strFaction = "Mangemort";
            break;

        case C_LawCard::E_FACTION::phenixOrder:
            strFaction = "PhenixOrder";
            break;

        default:
            return;
    }

    if(mAnimationStep > ANIMATION_STEP_MAX/2.0)
    {
        painter.setOpacity(0.4 + mAnimationStep/ANIMATION_STEP_MAX*0.6);
        painter.drawPixmap(rect(), QPixmap(QString(":/images/endGame_%1.png").arg(strFaction)));
    }
};
