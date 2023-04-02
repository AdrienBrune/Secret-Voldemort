QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/$$PWD

SOURCES += \
    Handlers/c_soundhandler.cpp \
    Widgets/w_screenendgame.cpp \
    Widgets/w_screenlaws.cpp \
    Widgets/w_screenpowerunlocked.cpp \
    Widgets/w_screenspying.cpp \
    Widgets/w_screenvote.cpp \
    Widgets/w_secretrole.cpp \
    main.cpp \
    c_controller.cpp \
    c_guiinterface.cpp \
    Remote/c_clienttcp.cpp \
    Handlers/c_remoteinterfacehandler.cpp \
    Handlers/c_playershandler.cpp \
    Handlers/c_boardhandler.cpp \
    Handlers/c_gamehandler.cpp \
    Widgets/w_playersmanagement.cpp \
    Widgets/w_screenmenu.cpp \
    Widgets/Components/w_stack.cpp \
    Widgets/Components/w_lawboard.cpp \
    Widgets/Components/w_player.cpp \
    Widgets/Components/w_electiontracker.cpp \
    Widgets/Components/w_gametracker.cpp \
    Widgets/Components/w_easel.cpp \
    Widgets/Components/w_messagedisplayer.cpp \
    ../../Shared/Remote/c_message.cpp \
    ../../Shared/Remote/serialization.cpp \
    ../../Shared/Components/c_gametracker.cpp \
    ../../Shared/Components/c_stack.cpp \
    ../../Shared/Components/c_player.cpp \
    ../../Shared/Components/c_lawboard.cpp \
    ../../Shared/Components/c_electiontracker.cpp

HEADERS += \
    ../../Shared/c_textdrawinghelper.h \
    Handlers/c_soundhandler.h \
    Widgets/w_messagebox.h \
    Widgets/w_screenendgame.h \
    Widgets/w_screenlaws.h \
    Widgets/w_screenpowerunlocked.h \
    Widgets/w_screenspying.h \
    Widgets/w_screenvote.h \
    Widgets/w_secretrole.h \
    c_controller.h \
    c_guiinterface.h \
    Remote/c_clienttcp.h \
    Handlers/c_playershandler.h \
    Handlers/c_remoteinterfacehandler.h \
    Handlers/c_boardhandler.h \
    Handlers/c_gamehandler.h \
    Widgets/w_gameboard.h \
    Widgets/w_playersmanagement.h \
    Widgets/w_screenmenu.h \
    Widgets/Components/w_stack.h \
    Widgets/Components/w_lawboard.h \
    Widgets/Components/w_player.h \
    Widgets/Components/w_electiontracker.h \
    Widgets/Components/w_gametracker.h \
    Widgets/Components/w_easel.h \
    Widgets/Components/w_messagedisplayer.h \
    ../../Shared/debug.h \
    ../../Shared/Remote/serialization.h \
    ../../Shared/Remote/c_message.h \
    ../../Shared/Components/c_gametracker.h \
    ../../Shared/Components/c_stack.h \
    ../../Shared/Components/c_lawboard.h \
    ../../Shared/Components/c_player.h \
    ../../Shared/Components/c_electiontracker.h \
    ../../Shared/Remote/c_tcpsocketack.h \
    c_json.h

FORMS += \
    Widgets/w_confirmmessagebox.ui \
    Widgets/w_continuemessagebox.ui \
    Widgets/w_screensettings.ui \
    Widgets/w_vetomessagebox.ui \
    c_controller.ui \
    c_guiinterface.ui \
    Widgets/w_screenmenu.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    uml.qmodel

ICON += Resources/icon.ico
RC_ICONS += Resources/icon.ico

RESOURCES += \
    Resources/Ressources.qrc

DEFINES += \
    LOG_VIA_FILE \
    CLIENT_FEATURES
