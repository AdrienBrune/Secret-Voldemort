QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

INCLUDEPATH += $$PWD/$$PWD

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/servergui.cpp \
    c_controller.cpp \
    Remote/c_servertcp.cpp \
    c_tools.cpp \
    main.cpp \
    ../../Shared/Remote/c_message.cpp \
    ../../Shared/Remote/serialization.cpp \
    ../../Shared/Components/c_gametracker.cpp \
    ../../Shared/Components/c_stack.cpp \
    ../../Shared/Components/c_player.cpp \
    ../../Shared/Components/c_lawboard.cpp \
    ../../Shared/Components/c_electiontracker.cpp

HEADERS += \
    GUI/servergui.h \
    c_controller.h \
    Remote/c_servertcp.h \
    ../../Shared/debug.h \
    ../../Shared/Remote/serialization.h \
    ../../Shared/Remote/c_message.h \
    ../../Shared/Components/c_gametracker.h \
    ../../Shared/Components/c_stack.h \
    ../../Shared/Components/c_lawboard.h \
    ../../Shared/Components/c_player.h \
    ../../Shared/Components/c_electiontracker.h \
    ../../Shared/Remote/c_tcpsocketack.h \
    ../../Shared/Components/c_lawcard.h \
    c_tools.h

FORMS += \
    c_controller.ui

ICON += Ressources/icon.ico
RC_ICONS += Ressources/icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Ressources/Ressources.qrc

DEFINES += SERVER_FEATURES
