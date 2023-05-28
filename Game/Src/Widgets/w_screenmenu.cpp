#include "w_screenmenu.h"
#include "ui_w_screenmenu.h"
#include "ui_w_screensettings.h"
#include "Widgets/w_messagebox.h"
#include "Handlers/c_remoteinterfacehandler.h"
#include "Handlers/c_soundhandler.h"

#include <QJsonDocument>
#include <QJsonObject>

bool gSemaphoreMessageBox = false;

W_ScreenMenu::W_ScreenMenu(QWidget *parent)
    : QWidget(parent)
    , wScreenSettings(nullptr)
    , ui(new Ui::W_ScreenMenu)
{
    ui->setupUi(this);
    setGeometry(0, 0, parent->width(), parent->height());
    show();
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

W_ScreenMenu::~W_ScreenMenu()
{
    delete ui;
}

void W_ScreenMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setOpacity(0.8);
    painter.setBrush(QColor(20, 20, 20));
    painter.drawRect(rect());
}

void W_ScreenMenu::resizeEvent(QResizeEvent *)
{
    if(wScreenSettings)
    {
        wScreenSettings->setGeometry(0, 0, width(), height());
    }
}

void W_ScreenMenu::on_buttonQuitGame_clicked()
{
    gSemaphoreMessageBox = true;
    W_MessageBoxConfirmation popup(this, "Quitter le jeu ?");
    if(popup.exec() == QDialog::Accepted)
    {
        emit sig_quitGame();
    }
    gSemaphoreMessageBox = false;
}

void W_ScreenMenu::on_buttonSettings_clicked()
{
    if(wScreenSettings)
    {
        delete wScreenSettings;
        wScreenSettings = nullptr;
    }

    wScreenSettings = new W_ScreenSettings(this);
    C_SoundHandler::getInstance()->playSound(E_SOUNDS::screenOpen);
}

void W_ScreenMenu::onCloseSettings()
{

}








#define TXT_DISCONNECTED    "(deconnecte)"
#define TXT_CONNECTED       "(connecte)"
#define CLR_CONNECTED       "QLabel{color:#50EC8B;}"
#define CLR_DISCONNECTED    "QLabel{color:#EC6850;}"

W_ScreenSettings::W_ScreenSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::W_ScreenSettings)
{
    ui->setupUi(this);
    setGeometry(0, 0, parent->width(), parent->height());
    ui->input_ip->setText(C_RemoteInterfaceHandler::getInstance()->getClient()->getClientIP());
    ui->input_port->setValue(C_RemoteInterfaceHandler::getInstance()->getClient()->getClientPort());
    ui->input_name->setText(C_RemoteInterfaceHandler::getInstance()->getClient()->getName());
    ui->volumeSlider->setValue(C_SoundHandler::getInstance()->getGeneralSound());
    ui->input_music->setChecked(C_SoundHandler::getInstance()->getEnableMusic());
    ui->input_sounds->setChecked(C_SoundHandler::getInstance()->getEnableSounds());
    ui->input_connectionState->setText(C_RemoteInterfaceHandler::getInstance()->getClient()->isConnected() ? QString(TXT_CONNECTED) : QString(TXT_DISCONNECTED));
    ui->input_connectionState->setStyleSheet(C_RemoteInterfaceHandler::getInstance()->getClient()->isConnected() ? CLR_CONNECTED : CLR_DISCONNECTED);

    connect(ui->input_ip, &QLineEdit::textChanged, C_RemoteInterfaceHandler::getInstance()->getClient(), &C_ClientTcp::onSetIP);
    connect(ui->input_port, &QSpinBox::textChanged, C_RemoteInterfaceHandler::getInstance()->getClient(), &C_ClientTcp::onSetPort);
    connect(ui->buttonConnect, &QPushButton::clicked, C_RemoteInterfaceHandler::getInstance()->getClient(), &C_ClientTcp::onConnect);
    connect(ui->volumeSlider, &QSlider::valueChanged, C_SoundHandler::getInstance(), &C_SoundHandler::onSetGlobalVolume);
    connect(ui->input_name, &QLineEdit::textChanged, C_RemoteInterfaceHandler::getInstance()->getClient(), &C_ClientTcp::onSetName);
    connect(ui->input_music, &QCheckBox::toggled, C_SoundHandler::getInstance(), &C_SoundHandler::onEnableMusic);
    connect(ui->input_sounds, &QCheckBox::toggled, C_SoundHandler::getInstance(), &C_SoundHandler::onEnableSounds);
    connect(C_RemoteInterfaceHandler::getInstance(), &C_RemoteInterfaceHandler::sig_connected, this, [&](){ ui->input_connectionState->setStyleSheet(CLR_CONNECTED); ui->input_connectionState->setText(QString(TXT_CONNECTED)); });
    connect(C_RemoteInterfaceHandler::getInstance(), &C_RemoteInterfaceHandler::sig_disconnected, this, [&](){ ui->input_connectionState->setStyleSheet(CLR_DISCONNECTED); ui->input_connectionState->setText(QString(TXT_DISCONNECTED)); });

    show();
}

W_ScreenSettings::~W_ScreenSettings()
{
    delete ui;
}

void W_ScreenSettings::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setOpacity(1.0);
    painter.setBrush(QColor(40, 40, 40));
    painter.drawRect(rect());
}
