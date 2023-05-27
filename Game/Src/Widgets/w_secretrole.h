#ifndef W_SECRETROLE_H
#define W_SECRETROLE_H

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>

class W_SecretRole : public QWidget
{
    Q_OBJECT

public:
    QString mNames[11] = {
        "",
        "Harry Potter",
        "Hermione Granger",
        "Ron Weasley",
        "Sirius Black",
        "Neville Londubat",
        "Albus Dumbledor",
        "Drago Malfoy",
        "Beatrix Lestrange",
        "Lucius Malfoy",
        "Lord Voldemort"
    };
public:
    explicit W_SecretRole(QWidget *parent = nullptr);
    ~W_SecretRole();

public:
    void display(bool toggle);
    QSize getSize();
    QRect getStartLocation();
    QRect getStopLocation();

protected:
    void paintEvent(QPaintEvent *event)override;

private:
    QRect getTeamMateArea(quint8 index);

private:
    QPropertyAnimation *mAnimation;
};

#endif // W_SECRETROLE_H
