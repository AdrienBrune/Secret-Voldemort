#include "w_easel.h"
#include "Handlers/c_playershandler.h"

W_Easel::W_Easel(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    show();
}

W_Easel::~W_Easel()
{

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
        size = xWidth;
    else
        size = yHeight*RATIO_EASEL;
    QRect area(MARGIN_LEFT_POURCENT*width() + (xWidth - size)/2, MARGIN_TOP_POURCENT*height() + (yHeight - size/RATIO_EASEL)/2, size, size/RATIO_EASEL);
    C_Player::E_POSITION position = C_PlayersHandler::getInstance()->getMyPlayerInstance()->getPosition();

    if(position == C_Player::E_POSITION::Minister || position == C_Player::E_POSITION::Director)
    {
        painter.drawPixmap(area, QPixmap(QString(":/images/easel_%1.png").arg(position == C_Player::E_POSITION::Minister ? "Minister" : "Director")));
    }
}
