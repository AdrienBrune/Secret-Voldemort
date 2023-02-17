#ifndef W_EASEL_H
#define W_EASEL_H

#include <QWidget>
#include <QPainter>

class W_Easel : public QWidget
{
    Q_OBJECT

public:
    explicit W_Easel(QWidget *parent = nullptr);
    ~W_Easel();

protected:
    void paintEvent(QPaintEvent *event)override;
};

#endif // W_EASEL_H
