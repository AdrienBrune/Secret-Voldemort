#ifndef W_SCREENMENU_H
#define W_SCREENMENU_H

#include <QWidget>
#include <QPainter>
#include <QSpinBox>


namespace Ui {
class W_ScreenSettings;
}

class W_ScreenSettings : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenSettings(QWidget *parent = nullptr);
    ~W_ScreenSettings();

signals:
    void sig_quit();

protected:
    void paintEvent(QPaintEvent *event)override;

private:
    Ui::W_ScreenSettings *ui;
};






namespace Ui {
class W_ScreenMenu;
}

class W_ScreenMenu : public QWidget
{
    Q_OBJECT

public:
    explicit W_ScreenMenu(QWidget *parent = nullptr);
    ~W_ScreenMenu();

signals:
    void sig_quit();
    void sig_openSettings();
    void sig_quitGame();

protected:
    void paintEvent(QPaintEvent *event)override;
    void resizeEvent(QResizeEvent *event)override;

private slots:
    void on_buttonSettings_clicked();
    void on_buttonQuitGame_clicked();
    void onCloseSettings();

private:
    W_ScreenSettings *wScreenSettings;

private:
    Ui::W_ScreenMenu *ui;
};

#endif // W_SCREENMENU_H
