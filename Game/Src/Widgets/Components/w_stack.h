#ifndef W_PILE_H
#define W_PILE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include "../../Shared/Components/c_stack.h"

class W_Stack : public C_Stack
{
    Q_OBJECT

public:
    explicit W_Stack(QWidget *parent = nullptr);
    ~W_Stack();

signals:
    void sig_draw();

private slots:
    void onUpdateGUI();

public:
    const bool &getClickable()const{ return mClickable; };
    void setClickable(const bool &toggle){ mClickable = toggle; };

protected:
    void leaveEvent(QEvent*)override;
    void enterEvent(QEvent*)override;
    void paintEvent(QPaintEvent*)override;
    void mousePressEvent(QMouseEvent*)override;

private:
    bool mHover;
    bool mClickable;
};

#endif // W_PILE_H
