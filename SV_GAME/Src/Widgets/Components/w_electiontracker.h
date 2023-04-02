#ifndef W_ELECTIONTRACKER_H
#define W_ELECTIONTRACKER_H

#include <QWidget>
#include <QPainter>

#include "../../Shared/Components/c_electiontracker.h"

class W_ElectionTracker : public C_ElectionTracker
{
    Q_OBJECT

public:
    explicit W_ElectionTracker(QWidget *parent = nullptr);
    ~W_ElectionTracker();

protected:
    void paintEvent(QPaintEvent *event)override;
    void leaveEvent(QEvent*)override{ mHover = false; update(); };
    void enterEvent(QEvent*)override{ mHover = true; update(); };

private:
    void drawToken(QPainter *painter, quint8 index, bool visible);

protected:
    bool mHover;
};

#endif // W_ELECTIONTRACKER_H
