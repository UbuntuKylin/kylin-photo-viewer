QT       += core gui
QT += dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
    src/global/log.cpp \
    src/model/dbus.cpp \
    src/controller/interaction.cpp \
    src/controller/core.cpp \
    src/model/file.cpp \
    src/view/tmp.cpp \
    src/global/variable.cpp

HEADERS += \
    src/global/log.h \
    src/model/dbus.h \
    src/controller/interaction.h \
    src/controller/core.h \
    src/model/file.h \
    src/view/tmp.h \
    src/global/variable.h

TRANSLATIONS += \
    translations/kylin-photo-viewer_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
