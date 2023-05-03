#include "w_easel.h"
#include "Handlers/c_playershandler.h"

#include <QTimer>

W_Easel::W_Easel(QWidget *parent)
    : QWidget(parent)
    , mHover(false)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    show();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &W_Easel::onPositionChanged);
    timer->start(5000);
}

W_Easel::~W_Easel()
{

}

void W_Easel::onPositionChanged()
{
    if(!C_PlayersHandler::getInstance())
        return;

    if(!C_PlayersHandler::getInstance()->getMyPlayerInstance())
        return;

    switch(C_PlayersHandler::getInstance()->getMyPlayerInstance()->getPosition())
    {
        case C_Player::E_POSITION::Director:
            setToolTip("Vous êtes Directeur de Poudlard\nVous voterez une loi parmis les deux proposées par le Ministre de la magie");
            break;

        case C_Player::E_POSITION::Minister:
            setToolTip("Vous êtes le Ministre de la magie\nPiochez trois lois et n'en faites parvenir que deux au Directeur de Poudlard\nLe déclenchement d'un pouvoir devra être utilisé par vous");
            break;

        default:
            setToolTip("");
            break;
    }
}

void W_Easel::paintEvent(QPaintEvent *)
{
#define MARGIN_LEFT_POURCENT    0.06
#define MARGIN_TOP_POURCENT     0.17
#define RATIO_EASEL             3.95

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_PlayersHandler::getInstance()->getMyPlayerInstance())
        return;

    double size = 0, xWidth = width()-2*MARGIN_LEFT_POURCENT*width(), yHeight = height()-2*MARGIN_TOP_POURCENT*height();
    if(xWidth < yHeight*RATIO_EASEL)
    {
        size = xWidth;
    }
    else
    {
        size = yHeight*RATIO_EASEL;
    }
    if(!mHover)
        size = size*0.95;

    QRect area(MARGIN_LEFT_POURCENT*width() + (xWidth - size)/2, MARGIN_TOP_POURCENT*height() + (yHeight - size/RATIO_EASEL)/2, size, size/RATIO_EASEL);
    C_Player::E_POSITION position = C_PlayersHandler::getInstance()->getMyPlayerInstance()->getPosition();

    if(position == C_Player::E_POSITION::Minister || position == C_Player::E_POSITION::Director)
    {
        painter.drawPixmap(area, QPixmap(QString(":/images/easel_%1.png").arg(position == C_Player::E_POSITION::Minister ? "Minister" : "Director")));
    }
}
