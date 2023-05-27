#ifndef W_EASEL_H
#define W_EASEL_H

#include <QWidget>
#include <QPainter>
#include <Handlers/c_playershandler.h>

class W_Easel : public QWidget
{
    Q_OBJECT

public:
    explicit W_Easel(QWidget *parent = nullptr);
    ~W_Easel();

public slots:
    void onPositionChanged();

protected:
    void leaveEvent(QEvent*)override{ mHover = false; update(); };
    void enterEvent(QEvent*)override{ mHover = true; update(); };
    void paintEvent(QPaintEvent *event)override;

private:
    bool mHover;
};

#endif // W_EASEL_H
