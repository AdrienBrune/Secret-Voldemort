#include "Widgets/w_playersmanagement.h"
#include "Handlers/c_playershandler.h"
#include "../../Shared/debug.h"

#include <QAbstractSlider>
#include <QTimer>

W_PlayersManagement::W_PlayersManagement(QWidget *parent)
    : QScrollArea(parent)
    , mPlayersList(nullptr)
    , mLayout(nullptr)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setStyleSheet("W_PlayersManagement{"
                    "background-color:rgba(0,0,0,0);"
                "}"
                "QScrollBar {"
                    "border: no border;"
                    "background-color:rgba(250,250,250,50);"
                    "width:10px;"
                    "margin: 0px 0px 0px 0px;"
                    "border-radius:5px;"
                "}"
                "QScrollBar::handle {"
                    "background-color:rgba(250,250,250,150);"
                    "min-height: 0px;"
                    "border-radius:5px;"
                "}"
                "QScrollBar::add-line {"
                    "margin: 0px 0px 0px 0px;"
                    "height: 0 px;"
                    "width: 0px;"
                    "subcontrol-position: bottom;"
                    "subcontrol-origin: margin;"
                "}"
                "QScrollBar::sub-line {"
                    "margin: 0px 0px 0px 0px;"
                    "height: 0 px;"
                    "width: 0px;"
                    "subcontrol-position: top;"
                    "subcontrol-origin: margin;"
                "}"
                "QScrollBar::up-arrow, QScrollBar::down-arrow"
                "{"
                    "background: none;"
                "}"
                "QScrollBar::add-page, QScrollBar::sub-page"
                "{"
                    "background: none;"
                "}");

    setFrameShape(NoFrame);
    setWidgetResizable(true);
    setLayoutDirection(Qt::RightToLeft);
    setContentsMargins(0, 20, 0, 0);
    show();

    if(!C_PlayersHandler::getInstance())
        LOG_DBG("Error C_PlayersHandler instance not created");

    mPlayersList = C_PlayersHandler::getInstance()->getPlayers();
    connect(C_PlayersHandler::getInstance(), &C_PlayersHandler::sig_playerListUpdated, this, &W_PlayersManagement::onUpdateList);

    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);

    mScrollAreaWidgetContent = new QWidget();
    mScrollAreaWidgetContent->setStyleSheet("QWidget{background-color:rgba(0,0,0,0);}");
    mLayout = new QVBoxLayout(mScrollAreaWidgetContent);
    mLayout->setSpacing(0);
    setWidget(mScrollAreaWidgetContent);

    onUpdateList();
}

W_PlayersManagement::~W_PlayersManagement()
{
    if(mScrollAreaWidgetContent)
        delete mScrollAreaWidgetContent;
}

void W_PlayersManagement::onUpdateList()
{
    // Add new player if needed
    for(int i = mLayout->count(); i < mPlayersList->size(); i++)
    {
        connect(dynamic_cast<W_Player*>(mPlayersList->at(i)), &W_Player::sig_clicked, this, &W_PlayersManagement::sig_playerClicked);
        mLayout->addWidget(mPlayersList->at(i));
    }

    for(int i = 0; i < mPlayersList->size(); i++)
        mPlayersList->at(i)->update();

    update();
}

void W_PlayersManagement::paintEvent(QPaintEvent *)
{
    double yTopLine = mScrollAreaWidgetContent->visibleRegion().boundingRect().y();
    double yBottomLine = mScrollAreaWidgetContent->visibleRegion().boundingRect().y() + mScrollAreaWidgetContent->visibleRegion().boundingRect().height();

    for(C_Player *player : *mPlayersList)
    {
        double yTopLineWidget = player->y();
        double yBottomLinewidget = player->y() + player->height();

        if(yTopLineWidget < yTopLine) // check top line
        {
            dynamic_cast<W_Player*>(player)->setGlobalOpacity((yBottomLinewidget - yTopLine)/player->height());
        }
        else if(yBottomLinewidget > yBottomLine) // check bottom line
        {
            dynamic_cast<W_Player*>(player)->setGlobalOpacity((yBottomLine - yTopLineWidget)/player->height());
        }
        else
        {
            dynamic_cast<W_Player*>(player)->setGlobalOpacity(1.0);
        }
    }
}
