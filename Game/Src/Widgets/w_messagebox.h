#ifndef W_MESSAGEBOX_H
#define W_MESSAGEBOX_H

#include <QDialog>
#include <QPainter>
#include "ui_w_confirmmessagebox.h"
#include "ui_w_vetomessagebox.h"
#include "ui_w_continuemessagebox.h"
#include "Handlers/c_soundhandler.h"

extern bool gSemaphoreMessageBox;

class W_MessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit W_MessageBox(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    };
    ~W_MessageBox(){};

protected:
    void paintEvent(QPaintEvent*)override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setOpacity(0.98);
        painter.setBrush(QColor(30, 30, 30));
        painter.drawRoundedRect(rect(), 20, 20, Qt::AbsoluteSize);
    };
};

namespace Ui{
    class W_ConfirmBox;
}

class W_MessageBoxConfirmation : public W_MessageBox
{
    Q_OBJECT

public:
    explicit W_MessageBoxConfirmation(QWidget *parent = nullptr, QString label = "")
        : W_MessageBox(parent)
        , ui(new Ui::W_ConfirmMessageBox)
    {
        ui->setupUi(this);
        ui->label->setText(label);
        setGeometry(parent->width()/2 - width()/2, parent->height()/2 - height()/2, width(), height());

        connect(ui->buttonYes, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonNo, SIGNAL(clicked()), this, SLOT(reject()));
    };
    ~W_MessageBoxConfirmation(){ delete ui; };

private:
    Ui::W_ConfirmMessageBox *ui;
};

namespace Ui{
    class W_VetoMessageBox;
}

class W_MessageBoxVeto : public W_MessageBox
{
    Q_OBJECT

public:
    explicit W_MessageBoxVeto(QWidget *parent = nullptr)
        : W_MessageBox(parent)
        , ui(new Ui::W_VetoMessageBox)
    {
        ui->setupUi(this);
        setGeometry(parent->width()/2 - width()/2, parent->height()/2 - height()/2, width(), height());

        connect(ui->buttonYes, SIGNAL(clicked()), this, SLOT(accept()));
        connect(ui->buttonNo, SIGNAL(clicked()), this, SLOT(reject()));
    };
    ~W_MessageBoxVeto(){ delete ui; };

private:
    Ui::W_VetoMessageBox *ui;
};

class W_MessageBoxContinue : public W_MessageBox
{
    Q_OBJECT

public:
    explicit W_MessageBoxContinue(QWidget *parent = nullptr, QString label = "")
        : W_MessageBox(parent)
        , ui(new Ui::W_ContinueMessageBox)
    {
        ui->setupUi(this);
        ui->label->setText(label);
        setGeometry(parent->width()/2 - width()/2, parent->height()/2 - height()/2, width(), height());

        connect(ui->buttonContinue, SIGNAL(clicked()), this, SLOT(accept()));
    };
    ~W_MessageBoxContinue(){ delete ui; };

private:
    Ui::W_ContinueMessageBox *ui;
};



#endif // W_MESSAGEBOX_H
