#include "w_screenpowerunlocked.h"
#include "../../Shared/c_textdrawinghelper.h"

#define COLOR_ORANGE QColor(215, 153, 44)
#define COLOR_BLACK  QColor(20, 20, 20)
#define COLOR_GREY   QColor(100, 100, 100)
#define COLOR_WHITE  QColor(250, 250, 250)

W_ScreenPowerUnlocked::W_ScreenPowerUnlocked(QWidget *parent, C_LawBoard::E_POWER power)
    : QWidget(parent)
    , wBanner(nullptr)
{
    setGeometry(0, 0, parent->width(), parent->height());

    wBanner = new W_Banner(this, power);
    wBanner->setGeometry(0, height()/4, width(), height()/2);

    show();

    QTimer::singleShot(TIMEOUT_SCREEN_POWER_TRIGGER, this, [&](){
        emit sig_timeout(wBanner->getPower());
    });
}

W_ScreenPowerUnlocked::~W_ScreenPowerUnlocked()
{

}

void W_Banner::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /* Draw background */
    QPen penBorders(mColor[mPower < C_LawBoard::E_POWER::powersNumber ? mPower : 0], 16);
    painter.setOpacity(0.9);
    painter.setBrush(QColor(250, 250, 250));
    painter.drawRect(-16, 0, width()+32, height());
    painter.setPen(penBorders);
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, height(), width(), height());

    /* Draw labels */
    QRect iconArea(width()/8, height()/4, height()/2, height()/2);
    QRect boundingArea;
    QString label;
    painter.setOpacity(1.0);

    painter.setPen(QPen(COLOR_BLACK, 4));
    painter.drawLine(0, height()*2/3, width(), height()*2/3);
    boundingArea.setRect(iconArea.x() + iconArea.width() + 20, height()/3, width(), height()/3);
    label = "Pouvoir débloqué";
    painter.setFont(C_AutoScaleFont("Germania", label, boundingArea, height()/8));
    painter.drawText(boundingArea, Qt::AlignBottom|Qt::AlignLeft, label);

    boundingArea.setRect(0, height()*2/3+10, width()-30, height()*2/3-20);
    label = mLabelImage[mPower < C_LawBoard::E_POWER::powersNumber ? mPower : 0];
    painter.setPen(QPen(mColor[mPower < C_LawBoard::E_POWER::powersNumber ? mPower : 0]));
    painter.setFont(C_AutoScaleFont("Germania", label, boundingArea, height()/10));
    painter.drawText(boundingArea, Qt::AlignTop|Qt::AlignRight, label);

    /* Draw locker animation */
    if(mAnimationStep < ANIMATION_STEP_MAX)
    {
        /* Draw power image */
        painter.setBrush(QBrush(COLOR_WHITE));
        painter.setPen(QPen(COLOR_BLACK, 12));
        painter.drawEllipse(iconArea);
        //painter.setBrush(QBrush(QColor(250,250,250,0)));
        //painter.setPen(QPen(COLOR_GREY, 12));
        //painter.drawArc(QRect(iconArea.x()+12,iconArea.y()+12,iconArea.width()-24,iconArea.height()-24), 0, 16*mAnimationStep/ANIMATION_STEP_MAX*360.0);
        /* Draw lock image */
        painter.drawPixmap(iconArea, QPixmap(QString(":/images/lock.png")));
    }
    else
    {
        /* Draw power image */
        painter.setBrush(QBrush(COLOR_WHITE));
        painter.setPen(QPen(COLOR_BLACK, 12));
        painter.drawEllipse(iconArea);
        painter.drawPixmap(iconArea, QPixmap(QString(":/images/Power_%1.png").arg(mLabelImage[mPower < C_LawBoard::E_POWER::powersNumber ? mPower : 0])));
        /* Draw unlock image */
        painter.drawPixmap(iconArea, QPixmap(QString(":/images/unlock.png")));
    }
}
