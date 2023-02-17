#ifndef W_SCREENVOTE_H
#define W_SCREENVOTE_H

#include <QPushButton>
#include <QPainter>

#include "Handlers/c_soundhandler.h"

class W_VoteCard : public QPushButton
{
    Q_OBJECT

public:
    enum E_VOTE
    {
        notDefined,
        lumos,
        nox
    };

public:
    explicit W_VoteCard(QWidget *parent = nullptr, E_VOTE type = lumos)
        : QPushButton(parent)
        , mVote(type)
        , mHover(false)
    {
        setAttribute(Qt::WA_Hover, true);
        show();
    };
    ~W_VoteCard(){};

public:
    const E_VOTE &getVote()const{ return mVote; };

protected:
    void leaveEvent(QEvent*)override{ mHover = false; };
    void enterEvent(QEvent*)override{ C_SoundHandler::getInstance()->playSound(E_SOUNDS::hover); mHover = true; };
    void paintEvent(QPaintEvent *)override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect area;

        if(mHover)
            area.setRect(0, 0, width(), height());
        else
            area.setRect(width()/10, height()/10, width()*8/10, height()*8/10);

        painter.drawPixmap(area, QPixmap(QString(":/images/Vote_%1").arg(mVote == lumos ? "Lumos" : "Nox")));
    };

private:
    E_VOTE mVote;
    bool mHover;
};


class W_ScreenVote : public QWidget
{
    Q_OBJECT
public:
    explicit W_ScreenVote(QWidget *parent = nullptr);
    ~W_ScreenVote();

signals:
    void sig_vote(W_VoteCard::E_VOTE);

protected:
    void paintEvent(QPaintEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;

private slots:
    void onVote();

private:
    QRect getVoteCardPosition(quint8 index);

private:
    W_VoteCard *wLumosCard;
    W_VoteCard *wNoxCard;
};

#endif // W_SCREENVOTE_H
