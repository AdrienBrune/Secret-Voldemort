#include "w_screenlaws.h"
#include "../../Shared/debug.h"
#include "Handlers/c_boardhandler.h"
#include "../../Shared/c_textdrawinghelper.h"

#include <QTimer>

#define LAW_CARD_LEVEL      (height()/4+20)
#define LAW_CARD_MARGIN     (wLaws.size() == 3 ? width()*0.150 : width()*0.254)
#define LAW_CARD_SPACING    (wLaws.size() == 3 ? width()*0.059 : width()*0.104)
#define LAW_CARD_RATIO      (1.42)
#define LAW_CARD_WIDTH      (width()*0.187)

W_ScreenLaws::W_ScreenLaws(QWidget *parent, QList<C_LawCard::E_FACTION> laws, bool readOnly)
    : QWidget(parent)
    , wLaws(QList<W_LawCard*>())
    , mReadOnly(readOnly)
    , mButtonVeto(nullptr)
{
    setGeometry(0, 0, parent->width(), parent->height());
    for(int i = 0; i < laws.size(); i++)
    {
        wLaws.append(new W_LawCard(this, laws[i], readOnly));
        if(!readOnly)
            connect(wLaws.last(), &W_LawCard::clicked, this, &W_ScreenLaws::onDiscard);
        wLaws.last()->setGeometry(getLawCardPosition(i));
    }

    if(laws.size() == 2) // Director discarding
    {
        if(C_BoardHandler::getInstance()->getDeathEaterBoard()->getCardsOnBoard() >= 5)
        {
            mButtonVeto = new QPushButton(this);
            mButtonVeto->setStyleSheet("QPushButton{color:white;background-color:rgba(250,250,250,80);padding:5px;}"
                                       "QPushButton::hover{background-color:rgba(250,250,250,100);}"
                                       "QPushButton::pressed{background-color:rgba(250,250,250,120);}");
            mButtonVeto->setFont(QFont("Niagara Solid", 20));
            mButtonVeto->setText("Proposer un veto");
            mButtonVeto->setGeometry((width()-mButtonVeto->width())/2, height()-50-20, 300, 50);
            connect(mButtonVeto, &QPushButton::clicked, this, &W_ScreenLaws::sig_askVeto);
        }
    }
    show();

    if(readOnly)
        QTimer::singleShot(5000, this, [&](){ emit sig_timeout(C_LawCard::E_FACTION::notDefined); });
}

W_ScreenLaws::~W_ScreenLaws()
{

}

void W_ScreenLaws::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setOpacity(0.8);
    painter.setBrush(QColor(20, 20, 20));
    painter.drawRect(rect());


    QRect boundingArea(20, 0, width()-40, height()/4-20);
    QString string = QString(mReadOnly ? "Voici les trois prochaines cartes de loi à piocher" : "Cliquer sur la carte de loi à défausser");
    painter.setFont(C_AutoScaleFont("Germania", string, boundingArea));
    painter.setPen(QPen(QColor(250, 250, 250), 1));
    painter.drawLine(QLine(0, height()/4, width(), height()/4));
    painter.drawText(boundingArea, Qt::AlignCenter|Qt::AlignBottom, string);
}

void W_ScreenLaws::resizeEvent(QResizeEvent *)
{
    for(int i = 0; i < wLaws.size(); i++)
    {
        wLaws.at(i)->setGeometry(getLawCardPosition(i));
    }
    if(mButtonVeto)
        mButtonVeto->setGeometry((width()-mButtonVeto->width())/2, height()-50-20, 300, 50);
}

void W_ScreenLaws::onDiscard()
{
    W_LawCard* vote = qobject_cast<W_LawCard*>(sender());
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::cardSelection);
    emit sig_discard(vote->getFaction());
}

QRect W_ScreenLaws::getLawCardPosition(quint8 index)
{
    if(index > 2)
    {
        LOG_DBG("Error position does not exist");
        return QRect(0,0,0,0);
    }

    return QRect(LAW_CARD_MARGIN + index*(LAW_CARD_WIDTH+LAW_CARD_SPACING), LAW_CARD_LEVEL, LAW_CARD_WIDTH, LAW_CARD_WIDTH*LAW_CARD_RATIO);
}
