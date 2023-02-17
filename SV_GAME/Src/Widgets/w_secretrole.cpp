#include "w_secretrole.h"
#include "Handlers/c_playershandler.h"

#define MARGIN_ROLE 20
#define MARGIN_MATE 5

#define POP_UP_HEIGHT               (parentWidget()->height()/2.0)
#define ROLE_HEIGHT                 (POP_UP_HEIGHT - 2*MARGIN_ROLE)
#define ROLE_WIDTH                  (0.7*ROLE_HEIGHT)
#define MATE_SIZE                   ((POP_UP_HEIGHT - 2*MARGIN_MATE)/3.0)
#define POP_UP_WIDTH(teamAccess)    (teamAccess ? ((MARGIN_ROLE+2*MARGIN_MATE) + ROLE_WIDTH + MATE_SIZE + 20) : (2*MARGIN_ROLE + ROLE_WIDTH + 20))

W_SecretRole::W_SecretRole(QWidget *parent)
    : QWidget(parent)
{
    show();
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);

    mAnimation = new QPropertyAnimation(this, "geometry");
    mAnimation->setDuration(1000);
    mAnimation->setEasingCurve(QEasingCurve::InExpo);
}

W_SecretRole::~W_SecretRole()
{

}

void W_SecretRole::display(bool toggle)
{
    if(toggle)
    {
        mAnimation->setStartValue(getStartLocation());
        mAnimation->setEndValue(getStopLocation());
        mAnimation->start();
    }
    else
    {
        mAnimation->stop();
        setGeometry(getStartLocation());
    }
}

QRect W_SecretRole::getStartLocation()
{
    quint32 x, y, width, height;
    x = parentWidget()->width() - 20;
    y = parentWidget()->height()/4;
    width = POP_UP_WIDTH(false);
    height = POP_UP_HEIGHT;
    return QRect(x, y, width, height);
}

QRect W_SecretRole::getStopLocation()
{
    quint32 x, y, width, height;
    bool accessTeam = false;

    C_Player *me = C_PlayersHandler::getInstance()->getMyPlayerInstance();
    if(!me)
        return QRect(0,0,0,0);

    if((IS_DEATHEATER(me) && !IS_VOLDEMORT(me))
    || (IS_VOLDEMORT(me) && C_PlayersHandler::getInstance()->getPlayers()->size() < 6))
    {
        accessTeam = true;
    }

    x = parentWidget()->width() - POP_UP_WIDTH(accessTeam) + 20;
    y = parentWidget()->height()/4;
    width = POP_UP_WIDTH(accessTeam);
    height = POP_UP_HEIGHT;
    return QRect(x, y, width, height);
}

void W_SecretRole::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_PlayersHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance()->getMyPlayerInstance())
        return;

    /* Draw background */
    painter.setOpacity(0.7);
    painter.setBrush(QColor(250,250,250));
    painter.drawRoundedRect(rect(), 12, 12, Qt::RelativeSize);

    /* Draw role image */
    painter.setOpacity(0.9);
    QRect roleArea(MARGIN_ROLE, MARGIN_ROLE, ROLE_WIDTH, ROLE_HEIGHT);
    C_Player::E_ROLE role = C_PlayersHandler::getInstance()->getMyPlayerInstance()->getRole();
    painter.drawPixmap(roleArea, QPixmap(QString(":/images/Role_%1").arg(mNames[role])));

    /* Draw mates */
    QList<C_Player*> deathEaterTeam = C_PlayersHandler::getInstance()->getDeathEaterTeam();
    for(int i = 0; i < deathEaterTeam.size(); i++)
    {
        if(deathEaterTeam[i] == C_PlayersHandler::getInstance()->getMyPlayerInstance())
            deathEaterTeam.removeAt(i);
    }
    for(int i = 0; i < (deathEaterTeam.size() > 3 ? 3 : deathEaterTeam.size()); i++)
    {
        QRect area = getTeamMateArea(i);
        painter.drawPixmap(area, QPixmap(QString(":/images/Team_%1.png").arg(mNames[deathEaterTeam[i]->getRole()])));
        painter.setFont(QFont("Times New Roman", 10));
        painter.setPen(QPen(QColor(0, 0, 0)));
        painter.drawText(QRect(area.x(), area.y() + area.height()*2/3, area.width(), area.height()/3), Qt::AlignCenter, deathEaterTeam[i]->getName());
    }
}

QRect W_SecretRole::getTeamMateArea(quint8 index)
{
    return QRect(MARGIN_ROLE + ROLE_WIDTH + MARGIN_MATE, MARGIN_MATE + index*MATE_SIZE, MATE_SIZE, MATE_SIZE);
}


