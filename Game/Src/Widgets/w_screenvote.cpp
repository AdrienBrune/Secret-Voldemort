#include "w_screenvote.h"
#include "../../Shared/debug.h"
#include "Handlers/c_playershandler.h"

#include "../../Shared/c_textdrawinghelper.h"

#define VOTE_CARD_LEVEL     (height()/4+20)
#define VOTE_CARD_MARGIN    (width()*0.19)
#define VOTE_CARD_SPACING   (width()*0.075)
#define VOTE_CARD_RATIO     (1.453)
#define VOTE_CARD_WIDTH     (width()*0.280)

W_ScreenVote::W_ScreenVote(QWidget *parent)
    : QWidget(parent)
    , wLumosCard(nullptr)
    , wNoxCard(nullptr)
{
    setGeometry(0, 0, parent->width(), parent->height());
    wLumosCard = new W_VoteCard(this, W_VoteCard::E_VOTE::lumos);
    wNoxCard = new W_VoteCard(this, W_VoteCard::E_VOTE::nox);
    connect(wLumosCard, &W_VoteCard::clicked, this, &W_ScreenVote::onVote);
    connect(wNoxCard, &W_VoteCard::clicked, this, &W_ScreenVote::onVote);
    wLumosCard->setGeometry(getVoteCardPosition(0));
    wNoxCard->setGeometry(getVoteCardPosition(1));
    show();
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

W_ScreenVote::~W_ScreenVote()
{

}

void W_ScreenVote::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setOpacity(0.8);
    painter.setPen(QPen(QColor(0,0,0,0), 0));
    painter.setBrush(QColor(20, 20, 20));
    painter.drawRect(rect());

    if(!C_PlayersHandler::getInstance())
        return;

    C_Player *playerFocus = C_PlayersHandler::getInstance()->getPlayerFocus();

    QRect boundingArea(20, 0, width()-40, height()/4-20);
    QString string = QString("Voter pour élire %1 Directeur de Poudlard").arg(playerFocus ? playerFocus->getName() : "le joueur");
    painter.setFont(C_AutoScaleFont("Germania", string, boundingArea));
    painter.setPen(QPen(QColor(250, 250, 250), 1));
    painter.drawLine(QLine(0, height()/4, width(), height()/4));
    painter.drawText(boundingArea, Qt::AlignCenter|Qt::AlignBottom, string);
}

void W_ScreenVote::resizeEvent(QResizeEvent *)
{
    wLumosCard->setGeometry(getVoteCardPosition(0));
    wNoxCard->setGeometry(getVoteCardPosition(1));
}

void W_ScreenVote::onVote()
{
    W_VoteCard* vote = qobject_cast<W_VoteCard*>(sender());
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::cardSelection);
    emit sig_vote(vote->getVote());
}

QRect W_ScreenVote::getVoteCardPosition(quint8 index)
{
    if(index > 1)
    {
        LOG_DBG("Error position does not exist");
        return QRect(0,0,0,0);
    }

    return QRect(VOTE_CARD_MARGIN + index*(VOTE_CARD_WIDTH+VOTE_CARD_SPACING), VOTE_CARD_LEVEL, VOTE_CARD_WIDTH, VOTE_CARD_WIDTH*(1/VOTE_CARD_RATIO));
}
