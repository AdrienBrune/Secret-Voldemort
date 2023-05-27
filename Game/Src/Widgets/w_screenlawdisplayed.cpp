#include "w_screenlawdisplayed.h"
#include "Handlers/c_boardhandler.h"
#include "../../Shared/c_textdrawinghelper.h"

#include <QTimer>

#define LAW_CARD_LEVEL      (height()/4+20)
#define LAW_CARD_MARGIN     (wLaws.size() == 3 ? width()*0.150 : wLaws.size() == 2 ? width()*0.254 : width()*0.407)
#define LAW_CARD_SPACING    (wLaws.size() == 3 ? width()*0.059 : wLaws.size() == 2 ? width()*0.104 : 0.0)
#define LAW_CARD_RATIO      (1.42)
#define LAW_CARD_WIDTH      (width()*0.187)

W_ScreenLawDisplayed::W_ScreenLawDisplayed(QWidget *parent, QList<C_LawCard> lawsToDisplay, bool enableInteraction)
    : QWidget(parent)
    , mSentence(QString())
    , wLaws(QList<W_LawCard*>())
    , mEnableInteraction(enableInteraction)
    , mButtonVeto(nullptr)
    , mButtonConfirm(nullptr)
{
    setGeometry(0, 0, parent->width(), parent->height());

    if(lawsToDisplay.size())
    {
        // Discard law card screen
        mSentence = "Choisir la loi à défausser";
        for(int i = 0; i < lawsToDisplay.size(); i++)
        {
            wLaws.append(new W_LawCard(this, lawsToDisplay[i].getFaction(), mEnableInteraction));
            connect(wLaws.last(), &W_LawCard::clicked, this, &W_ScreenLawDisplayed::onNewSelection);
            wLaws.last()->setGeometry(getLawCardPosition(i));
        }
    }
    else
    {
        // Draw pile screen
        mSentence = "Piocher trois cartes de loi";
        wLaws.append(new W_LawCard(this, C_LawCard::E_FACTION::notDefined, mEnableInteraction));
        connect(wLaws.last(), &W_LawCard::clicked, this, &W_ScreenLawDisplayed::sig_drawPile);
        wLaws.last()->setGeometry(getLawCardPosition(0));
    }


    // Add Veto button when Director discard + Veto enabled
    if(lawsToDisplay.size() == 2)
    {
        if(C_BoardHandler::getInstance()->getDeathEaterBoard()->getCardsOnBoard() >= 5)
        {
            mSentence += " - Veto disponible";
            mButtonVeto = new QPushButton(this);
            mButtonVeto->setStyleSheet("QPushButton{color:white;background-color:rgba(250,250,250,80);padding:5px;}"
                                       "QPushButton::hover{background-color:rgba(250,250,250,100);}"
                                       "QPushButton::pressed{background-color:rgba(250,250,250,120);}");
            mButtonVeto->setFont(QFont("Niagara Solid", 20));
            mButtonVeto->setText("Proposer un veto");
            mButtonVeto->setGeometry((width()-mButtonVeto->width())/2, height()-50-20-50-20, 300, 50);
            connect(mButtonVeto, &QPushButton::clicked, this, &W_ScreenLawDisplayed::sig_askVeto);
        }
    }

    // Clairvoyance power screen
    if(!enableInteraction)
    {
        mSentence = "Clairvoyance, voici les trois cartes du dessus de la pile";
        QTimer::singleShot(5000, this, [&](){ emit sig_timeout(); });
    }

    // Add confirm button
    mButtonConfirm = new QPushButton(this);
    mButtonConfirm->setStyleSheet("QPushButton{color:white;background-color:rgba(250,250,250,80);padding:5px;}"
                               "QPushButton::hover{background-color:rgba(250,250,250,100);}"
                               "QPushButton::pressed{background-color:rgba(250,250,250,120);}");
    mButtonConfirm->setFont(QFont("Niagara Solid", 20));
    mButtonConfirm->setText("Confirmer");
    mButtonConfirm->setGeometry((width()-mButtonConfirm->width())/2, height()-50-20, 300, 50);
    connect(mButtonConfirm, &QPushButton::clicked, this, &W_ScreenLawDisplayed::onConfirmSelection);
    mButtonConfirm->hide();

    show();
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

W_ScreenLawDisplayed::~W_ScreenLawDisplayed()
{

}

void W_ScreenLawDisplayed::onConfirmSelection()
{
    for(W_LawCard *law : qAsConst(wLaws))
    {
        if(law->getSelection() == W_LawCard::E_SELECTION::notSelected)
        {
            C_SoundHandler::getInstance()->playSound(E_SOUNDS::cardSelection);
            emit sig_discard(law);
            return;
        }
    }
}

void W_ScreenLawDisplayed::onNewSelection()
{
    W_LawCard *law = dynamic_cast<W_LawCard*>(sender());
    if(law)
    {
        selectAll();
        law->setSelection(W_LawCard::E_SELECTION::notSelected);
        mButtonConfirm->show();
    }
}

void W_ScreenLawDisplayed::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setOpacity(0.8);
    painter.setPen(QPen(QColor(0,0,0,0), 0));
    painter.setBrush(QColor(20, 20, 20));
    painter.drawRect(rect());

    QRect boundingArea(20, 0, width()-40, height()/4-20);
    painter.setFont(C_AutoScaleFont("Germania", mSentence, boundingArea));
    painter.setPen(QPen(QColor(250, 250, 250), 1));
    painter.drawLine(QLine(0, height()/4, width(), height()/4));
    painter.drawText(boundingArea, Qt::AlignCenter|Qt::AlignBottom, mSentence);
}

void W_ScreenLawDisplayed::resizeEvent(QResizeEvent*)
{
    for(int i = 0; i < wLaws.size(); i++)
    {
        wLaws.at(i)->setGeometry(getLawCardPosition(i));
    }
    if(mButtonVeto)
        mButtonVeto->setGeometry((width()-mButtonVeto->width())/2, height()-50-20-50-20, 300, 50);
    if(mButtonConfirm)
        mButtonConfirm->setGeometry((width()-mButtonConfirm->width())/2, height()-50-20, 300, 50);
}

QRect W_ScreenLawDisplayed::getLawCardPosition(quint8 index)
{
    return QRect(LAW_CARD_MARGIN + index*(LAW_CARD_WIDTH+LAW_CARD_SPACING), LAW_CARD_LEVEL, LAW_CARD_WIDTH, LAW_CARD_WIDTH*LAW_CARD_RATIO);
}


