#include "w_gametracker.h"
#include "Handlers/c_gamehandler.h"
#include "../../Shared/c_textdrawinghelper.h"

#define TOKEN_SIZE      (height()/6.0)
#define MARGIN          (width()/8.0)
#define SPACING(nb)     ((width() - (2.0*MARGIN) - (nb*TOKEN_SIZE))/(nb-1))
#define LEVEL           (height()/2.0)

W_GameTracker::W_GameTracker(QWidget *parent)
    : C_GameTracker(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    show();
}

W_GameTracker::~W_GameTracker()
{

}

void W_GameTracker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_GameHandler::getInstance()->getElectionTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    /* Draw event tokens */
    quint8 tokenNumber = 0;
    for(int i = 0; i < C_Message_Event::E_EVENT::event_number; i++)
    {
        if(!isSkipEvent(static_cast<C_Message_Event::E_EVENT>(i)))
        {
            mResumeEvent == static_cast<C_Message_Event::E_EVENT>(i) ? drawToken(&painter, tokenNumber, true) : drawToken(&painter, tokenNumber, false);
            tokenNumber++;
        }
    }

    /* Draw current event label */
    QRect boundingArea(0, height()*2/3, width(), height()/4);
    painter.setPen(QPen(QColor(250, 250, 250)));
    painter.setFont(C_AutoScaleFont("Germania", mResumeEventLabel, boundingArea, boundingArea.height()));
    painter.drawText(boundingArea, mResumeEventLabel, Qt::AlignCenter|Qt::AlignRight);
}

void W_GameTracker::drawToken(QPainter *painter, quint8 step, bool isCurrentStep)
{
    quint32 location = MARGIN + step * SPACING(getNumberStepToDisplay()) + step * TOKEN_SIZE + TOKEN_SIZE/2;
    quint32 tokenSize = isCurrentStep ? 2.0*TOKEN_SIZE : TOKEN_SIZE;
    QRect area(location - tokenSize/2.0, LEVEL - tokenSize/2.0, tokenSize, tokenSize);

    painter->setOpacity(1);
    painter->setPen(QPen());
    painter->setBrush(QBrush(QColor(250, 250, 250)));
    painter->drawEllipse(area);
}
