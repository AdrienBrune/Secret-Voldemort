#include "Widgets/Components/w_player.h"
#include "Handlers/c_playershandler.h"
#include "Handlers/c_gamehandler.h"
#include "../../Shared/Remote/c_message.h"

#define SMALL_BOX   width()/10, height()/10, width()*8/10, height()*8/10
#define BIG_BOX     width()/20, height()/20, width()*9/10, height()*9/10

#define BOX_HEIGHT 100

#define LOOP_ANIMATION_STEP (360.0/20.0)

W_Player::W_Player(QWidget *parent)
    : C_Player(parent)
    , mHover(false)
    , mClickable(false)
    , mGlobalOpacity(1.0)
    , mTimerLoop(nullptr)
    , mAnimationLoop(0)
{
    show();
    setAttribute(Qt::WA_Hover, true);
    setMinimumHeight(BOX_HEIGHT);
    setMaximumHeight(BOX_HEIGHT);
    mTimerLoop = new QTimer(this);
    connect(mTimerLoop, &QTimer::timeout, this, [&](){
        mAnimationLoop = static_cast<quint16>(mAnimationLoop + LOOP_ANIMATION_STEP) % 360;
        update();
    });
    connect(this, &C_Player::sig_actionRequested, this, &W_Player::onActionRequested);
}

W_Player::~W_Player()
{

}

void W_Player::leaveEvent(QEvent *)
{
    mHover = false;
}

void W_Player::enterEvent(QEvent *)
{
    mHover = true;
}

void W_Player::mousePressEvent(QMouseEvent *)
{
    // Get my player instance
    C_Player *myPlayerInstance = C_PlayersHandler::getInstance()->getMyPlayerInstance();

    // Get current game event
    C_Message_Event::E_EVENT gameEvent = C_GameHandler::getInstance()->getGameTracker()->getEvent();


    // Check if player is alive - Can't interact with dead/unconnected player
    if(mStatus == playing)
    {
        // Check if player is me - Can't interact with me
        if(myPlayerInstance != this)
        {
            // Check if widget is ready for interaction
            if(mHover && mClickable)
            {
                // Check for specific event - Don't allow interactions on everyone
                if(gameEvent == C_Message_Event::E_EVENT::SC_Director_selection_begin)
                {
                    // Check for ex elected Minister/Director - Can't interact with them
                    if(mEligible)
                    {
                        emit sig_clicked(this);
                    }
                }
                else
                {
                    emit sig_clicked(this);
                }
            }
        }
    }
}

void W_Player::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /*****************************************************************************************/
    /*                               Pens, brushes declaration                               */
    /*****************************************************************************************/

#define PEN_WIDTH   4

    // Focus pen.
    QPen penFocus(QColor(255, 222, 92), PEN_WIDTH);

    // Minister brush.
    QBrush brushMinister(QColor(220, 220, 220));
    // Minister Pen.
    QPen penMinister(QColor(160, 120, 70), PEN_WIDTH);
    // Director brush.
    QBrush brushDirector(QColor(115, 115, 115));
    // Director pen.
    QPen penDirector(QColor(37, 37, 37), PEN_WIDTH);
    // Player brush.
    QBrush brushPlayer(QColor(37, 37, 37));
    // Player pen.
    QPen penPlayer(QColor(200, 200, 200), PEN_WIDTH);

    // Voldemort brush.
    QBrush brushVoldemort(QColor(28, 63, 56));
    // Voldemort pen.
    QPen penVoldemort(QColor(70, 130, 115), PEN_WIDTH);
    // Death eater brush.
    QBrush brushDeathEater(QColor(89, 53, 119));
    // Death eater pen.
    QPen penDeathEater(QColor(112, 74, 137), PEN_WIDTH);
    // Phenix order brush.
    QBrush brushPhenixOrder(QColor(143, 20, 31));
    // Phenix order pen.
    QPen penPhenixOrder(QColor(153, 64, 55), PEN_WIDTH);

    // Unconnected player brush.
    QBrush brushUnconnected(QColor(71, 48, 45));
    // Unconnected player pen.
    QPen penUnconnected(QColor(98, 68, 65), PEN_WIDTH);
    // Dead player brush.
    QBrush brushDead(QColor(71, 48, 45));
    // Dead player pen.
    QPen penDead(QColor(200, 200, 200), PEN_WIDTH);

    // White text pen.
    QPen penWhite(QColor(220, 220, 220), PEN_WIDTH);
    // Black text pen.
    QPen penBlack(QColor(40, 40, 40), PEN_WIDTH);


    if(!C_PlayersHandler::getInstance())
        return;

    // Get my player instance
    C_Player *myPlayerInstance = C_PlayersHandler::getInstance()->getMyPlayerInstance();
    if(!myPlayerInstance)
        return;

    // Get current game event
    if(!C_GameHandler::getInstance()->getGameTracker())
        return;
    C_Message_Event::E_EVENT gameEvent = C_GameHandler::getInstance()->getGameTracker()->getEvent();

    /*****************************************************************************************/
    /*                                    Geometry setting                                   */
    /*****************************************************************************************/

    /// Set box geometry
    QRect box = QRect(SMALL_BOX);
    // Check if player is alive - Can't interact with dead player
    if(mStatus == playing)
    {
        // Check if player is me - Can't interact with me
        if(myPlayerInstance != this)
        {
            // Check if widget is ready for interaction
            if(mHover && mClickable)
            {
                // Check for specific event - Don't allow interactions on everyone
                if(gameEvent == C_Message_Event::E_EVENT::SC_Director_selection_begin)
                {
                    // Check for ex elected Minister/Director - Can't interact with them
                    if(mEligible)
                    {
                        box = QRect(BIG_BOX);
                    }
                }
                else
                {
                    // Power Interaction
                    box = QRect(BIG_BOX);
                }
            }
        }
    }
    if(mFlagFocus)
    {
        box = QRect(BIG_BOX);
    }

    /// Set label geometry
    QRect labelName, labelRole;
    labelName = QRect(box.x(), 0, box.width(), box.height()*2/3);
    labelRole = QRect(box.x(), box.height()*2/3, box.width(), box.height()/3);

    /*****************************************************************************************/
    /*                                Game finished display                                  */
    /*****************************************************************************************/
    /// Box drawing
    /// Name drawing
    if(C_GameHandler::getInstance()->getGameTracker()->getEvent() == C_Message_Event::E_EVENT::SC_game_finished)
    {
        // draw box and texts.
        painter.setOpacity(0.98*mGlobalOpacity);
        painter.setPen(penWhite);
        switch(mRole)
        {
            case Beatrix:
            case Drago:
            case Lucius:
                painter.setBrush(brushDeathEater);
                painter.setPen(penDeathEater);
                painter.drawRect(box);
                painter.setOpacity(1*mGlobalOpacity);
                painter.setFont(QFont("Harry Potter", 20));
                painter.drawText(labelName, Qt::AlignCenter, mName);
                painter.setFont(QFont("Times New Roman", 8));
                painter.drawText(labelRole, Qt::AlignCenter, "Mangemort");
                break;

            case Dumbledor:
            case Harry:
            case Hermione:
            case Neville:
            case Ron:
            case Sirius:
                painter.setBrush(brushPhenixOrder);
                painter.setPen(penPhenixOrder);
                painter.drawRect(box);
                painter.setOpacity(1*mGlobalOpacity);
                painter.setFont(QFont("Harry Potter", 20));
                painter.drawText(labelName, Qt::AlignCenter, mName);
                painter.setFont(QFont("Times New Roman", 8));
                painter.drawText(labelRole, Qt::AlignCenter, "Ordre du Phénix");
                break;

            case Voldemort:
                painter.setBrush(brushVoldemort);
                painter.setPen(penVoldemort);
                painter.drawRect(box);
                painter.setOpacity(1*mGlobalOpacity);
                painter.setFont(QFont("Harry Potter", 20));
                painter.drawText(labelName, Qt::AlignCenter, mName);
                painter.setFont(QFont("Times New Roman", 8));
                painter.drawText(labelRole, Qt::AlignCenter, "Voldemort");
                break;

            case notAttributed:
                painter.setBrush(brushUnconnected);
                painter.setPen(penUnconnected);
                painter.drawRect(box);
                painter.setOpacity(1*mGlobalOpacity);
                painter.setFont(QFont("Harry Potter", 20));
                painter.drawText(labelName, Qt::AlignCenter, mName);
                painter.setFont(QFont("Times New Roman", 8));
                painter.drawText(labelRole, Qt::AlignCenter, "En attente");
                break;

            default:
                break;
        }
        return;
    }

    /*****************************************************************************************/
    /*                              Game in progress display                                 */
    /*****************************************************************************************/

    /// Box drawing
    painter.setOpacity(0.95*mGlobalOpacity);
    painter.setPen(penBlack);
    switch(mStatus)
    {
        case dead:
            painter.setBrush(brushDead);
            painter.setPen(QPen());
            painter.drawRect(box);
            break;

        case playing:
            switch(mPosition)
            {
                case Minister:
                    painter.setBrush(brushMinister);
                    painter.setPen(penMinister);
                    painter.drawRect(box);
                    break;

                case Director:
                    painter.setBrush(brushDirector);
                    painter.setPen(penDirector);
                    painter.drawRect(box);
                    break;

                default:
                    painter.setBrush(brushPlayer);
                    painter.setPen(QPen());
                    painter.drawRect(box);
                    break;
            }
            break;

        case unconnected:
        case connected:
            painter.setBrush(brushUnconnected);
            painter.setPen(QPen());
            painter.drawRect(box);
            break;
    }

    /// Focus drawing
    if(mFlagFocus)
    {
        painter.setOpacity(1*mGlobalOpacity);
        painter.setBrush(QBrush());
        painter.setPen(penFocus);
        painter.drawRect(box);
    }

    /// Label drawing
    painter.setOpacity(1*mGlobalOpacity);
    painter.setFont(QFont("Times New Roman", 9));
    switch(mStatus)
    {
        case dead:
            painter.setPen(penDead);
            painter.drawText(labelRole, Qt::AlignCenter, "Mort");
            break;

        case playing:
            switch(mPosition)
            {
                case Minister:
                    painter.setPen(penMinister);
                    painter.drawText(labelRole, Qt::AlignCenter, "Ministre de la magie");
                    break;

                case Director:
                    painter.setPen(penDirector);
                    painter.drawText(labelRole, Qt::AlignCenter, "Directeur de Poudlard");
                    break;

                default:
                    if(!mEligible)
                    {
                        painter.setPen(penPlayer);
                        painter.drawText(labelRole, Qt::AlignCenter, "non-éligible");
                    }
                    painter.setPen(penPlayer);
                    break;
            }
            break;

        case unconnected:
        case connected:
            painter.setPen(penUnconnected);
            painter.drawText(labelRole, Qt::AlignCenter, "En attente");
            break;
    }
    painter.setFont(QFont("Harry Potter", 20));
    painter.drawText(labelName, Qt::AlignCenter, mName);

    /// Vote drawing
    if(!mFlagVote)
    {
        if(mVote)
        {
            painter.setBrush(QColor(0,0,0,0));
            switch(mVote)
            {
                case E_VOTE::lumos:
                    painter.setPen(QPen(QColor(40, 120, 40), 10));
                    break;

                case E_VOTE::nox:
                    painter.setPen(QPen(QColor(120, 40, 40), 10));
                    break;

                default:
                    painter.setPen(QPen(QColor(0,0,0,0), 0));
                    break;
            }
            QPoint p1(box.x() + box.width() - box.height()/2, box.y()),
                   p2(box.x() + box.width(), box.y() + box.height()/2);
            painter.drawLine(p1, p2);
        }
    }

    /// Power drawing
    // Check if player is alive - Can't interact with dead player
    if(mStatus != dead)
    {
        // Check if player is me - Can't interact with me
        if(myPlayerInstance != this)
        {
            // Check if widget is ready for interaction
            if(mHover && mClickable)
            {
                // Check if the player who's interacting is the minister
                if(myPlayerInstance->getPosition() == Minister)
                {
                    painter.setOpacity(1*mGlobalOpacity);
                    switch(C_GameHandler::getInstance()->getGameTracker()->getEvent())
                    {
                        case C_Message_Event::SC_power_substitute_Minister:
                            painter.drawPixmap(QRect(box.x(), box.y(), box.height(), box.height()), QPixmap(":/images/Power_Coup d'état.png"));
                            break;

                        case C_Message_Event::SC_power_assassination:
                            painter.drawPixmap(QRect(box.x(), box.y(), box.height(), box.height()), QPixmap(":/images/Power_Assassinat.png"));
                            break;

                        case C_Message_Event::SC_power_spying:
                            painter.drawPixmap(QRect(box.x(), box.y(), box.height(), box.height()), QPixmap(":/images/Power_Espionnage.png"));
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    /// Draw action requested
    if(mActionRequested)
    {
        QRect loopArea(box.x() + 10, box.y() + box.height()/4, box.height()/2, box.height()/2);
        painter.setBrush(QBrush(QColor(0,0,0,0)));
        painter.setPen(QPen(QColor(215, 153, 44), 4));
        painter.setOpacity(0.4);
        painter.drawEllipse(loopArea);
        painter.setOpacity(1);
        painter.drawArc(loopArea, mAnimationLoop - 30 > 0 ? 16*(mAnimationLoop-30) : 0, 16*mAnimationLoop);
    }
}
