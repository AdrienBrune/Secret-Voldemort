#include "w_electiontracker.h"
#include "Handlers/c_gamehandler.h"
#include "../../Shared/c_textdrawinghelper.h"

W_ElectionTracker::W_ElectionTracker(QWidget *parent)
    : C_ElectionTracker(parent)
    , mHover(false)
{
    setToolTip("Le Ministère de la magie entrera en état de chaos si 3 jetons sont posés\n"
               "Un jeton sera posé à chaque fois qu'un tour se termine sans avoir élu de Directeur de Poudlard\n"
               "Les jetons sont retirés dès qu'un Directeur est élu\n"
               "Le chaos entraine le vote de la loi du dessus de la pile et tout le monde devient éligible au prochain tour");
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    show();
}

W_ElectionTracker::~W_ElectionTracker()
{

}

void W_ElectionTracker::paintEvent(QPaintEvent *)
{
#define LEVEL           (height()*2/3 - 5)
#define MARGIN_LEFT     (width()/2)
#define MARGIN_RIGHT    (width()/8)
#define TOKEN_SIZE      (height()/4)
#define SPACING         (width() - MARGIN_LEFT - MARGIN_RIGHT - 3.0*TOKEN_SIZE)/2.0

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if(!C_GameHandler::getInstance()->getElectionTracker())
        return;

    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::ApplicationStarted)
        return;

    // Draw cancel vote status.

    QPen penSelected;
    QPen penInvisible(QColor(0,0,0,0));
    QPen penWhite(QBrush(QColor(250, 250, 250)), 2, Qt::SolidLine, Qt::SquareCap, Qt::PenJoinStyle::MPenJoinStyle);

    QRect boundingArea(width()/2, height()/2, width()/2, height()/4);
    QRect textArea(0, height()/2, width()/2, height()/4);
    QRect area1(boundingArea.x(), boundingArea.y(), boundingArea.width()/6, boundingArea.height());
    QRect area2(boundingArea.x() + 2*boundingArea.width()/6, boundingArea.y(), boundingArea.width()/6, boundingArea.height());
    QRect area3(boundingArea.x() + 4*boundingArea.width()/6, boundingArea.y(), boundingArea.width()/6, boundingArea.height());

    // Setting text fields.

    QFont font("Germania");
    font.setPointSize(height()/10);
    painter.setFont(font);
    QFontMetrics tool(font);
    QRect bounding;

    bounding = tool.boundingRect("rejeté");
    QRect textShape1(area1.x() + area1.width()/2 - bounding.width()/2, area1.y() - 1.5*bounding.height(), bounding.width(), bounding.height());
    QRect textShape2(area2.x() + area2.width()/2 - bounding.width()/2, area2.y() - 1.5*bounding.height(), bounding.width(), bounding.height());
    bounding = tool.boundingRect("chaos");
    QRect textShape3(area3.x() + area3.width()/2 - bounding.width()/2, area3.y() - 1.5*bounding.height(), bounding.width(), bounding.height());

    // Text shape left.
    penSelected = (mCounter >= 1) ? penWhite : penInvisible;
    painter.setPen(penSelected);
    painter.drawText(textShape1, Qt::AlignCenter, "rejeté");

    // Text shape middle.
    penSelected = (mCounter >= 2) ? penWhite : penInvisible;
    painter.setPen(penSelected);
    painter.drawText(textShape2, Qt::AlignCenter, "rejeté");

    // Text shape right.
    penSelected = (mCounter >= 3) ? penWhite : penInvisible;
    painter.setPen(penSelected);
    painter.drawText(textShape3, Qt::AlignCenter, "chaos");

    // Draw text
    QString string("Jetons d'élection");
    C_AutoScaleFont fontText("Germania", string, textArea);
    if(mHover)
        fontText.setPointSize(fontText.pointSize());
    else
        fontText.setPointSize(fontText.pointSize()-fontText.pointSize()*0.1);
    painter.setFont(fontText);
    painter.setPen(QPen(QColor(250, 250, 250), 1));
    painter.drawText(textArea, Qt::AlignCenter, string);

    drawToken(&painter, 0, mCounter >= 1 ? true : false);
    drawToken(&painter, 1, mCounter >= 2 ? true : false);
    drawToken(&painter, 2, mCounter >= 3 ? true : false);

    // Draw underline
    painter.setBrush(QColor(250, 250, 250, 0));
    painter.setPen(QPen(QColor(250, 250, 250), 2));
    painter.drawLine(textArea.x()+20, boundingArea.y() + boundingArea.height()+10, width(), boundingArea.y() + boundingArea.height()+10);
}

void W_ElectionTracker::drawToken(QPainter *painter, quint8 index, bool visible)
{
    quint32 location = MARGIN_LEFT + index * SPACING + index * TOKEN_SIZE + TOKEN_SIZE/2;
    QRect area(location - TOKEN_SIZE/2.0, LEVEL - TOKEN_SIZE/2.0, TOKEN_SIZE, TOKEN_SIZE);

    QVector<QPoint> token = {
        QPoint(area.x() + area.width()/6, area.y() + area.height()/2),
        QPoint(area.x() + area.width()/2, area.y()),
        QPoint(area.x() + area.width()*5/6, area.y() + area.height()/2),
        QPoint(area.x() + area.width()/2, area.y() + area.height())
    };

    painter->setBrush(QColor(250, 250, 250));
    painter->setPen(QPen(QColor(0, 0, 0), 2));
    if(visible)
        painter->drawPolygon(token);
    else
        painter->drawEllipse(QRect(area.x() + area.width()*3/8, area.y() + area.height()*3/8, area.width()/4, area.height()/4));
}
