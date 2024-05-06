QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    LuaBinder.cpp \
    main.cpp \
    WorkspacePlayer.cpp \
    widgets/BlockViewer.cpp \
    widgets/Button.cpp \
    widgets/Dialog.cpp \
    widgets/Flow.cpp \
    widgets/Label.cpp \
    widgets/TextInput.cpp

HEADERS += \
    LuaBinder.h \
    WorkspacePlayer.h \
    widgets/BlockViewer.h \
    widgets/Button.h \
    widgets/Dialog.h \
    widgets/Flow.h \
    widgets/Label.h \
    widgets/TextInput.h

FORMS += \
    WorkspacePlayer.ui \
    widgets/Dialog.ui \
    widgets/TextInput.ui

INCLUDEPATH += /usr/include/lua5.4/
LIBS += -llua5.4

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
