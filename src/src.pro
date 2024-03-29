QT       += core gui
QT       += svg
QT       += dbus
QT       += x11extras KWindowSystem printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# 适配窗口管理器圆角阴影
LIBS +=-lpthread
LIBS +=-lX11
#log
LIBS += -L/usr/lib/libukui-log4qt.so.1.0.0 -lukui-log4qt
TARGET = kylin-photo-viewer
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt

DEFINES += QT_DEPRECATED_WARNINGS

#opencv
INCLUDEPATH += /usr/include/opencv4/
LIBS += -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_imgproc \

#stb
LIBS += -lstb \

#gif
LIBS += -lgif \

target.path = /usr/bin
target.source +=$$TARGET
INSTALLS += target

SOURCES += main.cpp \
    controller/core/coreinteraction.cpp \
    controller/core/albumthumbnail.cpp \
    global/computingtime.cpp \
    global/log.cpp \
    model/dbus.cpp \
    controller/core/core.cpp \
    model/file/file.cpp \
    model/file/loadmovie.cpp \
    model/file/savemovie.cpp \
    model/processing/flip.cpp \
    model/processing/processing.cpp \
    view/daemondbus.cpp \
    view/edit.cpp \
    view/information.cpp \
    view/kyview.cpp \
    global/variable.cpp \
    view/menumodule.cpp \
    view/navigator.cpp \
    view/openimage.cpp \
    view/showimagewidget.cpp \
    view/sidebar.cpp \
    view/sidebardelegate.cpp \
    view/titlebar.cpp \
    view/toolbar.cpp \
    view/xatom-helper.cpp

HEADERS += \
    controller/core/coreinteraction.h \
    controller/core/albumthumbnail.h \
    controller/core/base.h \
    global/computingtime.h \
    global/log.h \
    model/dbus.h \
    controller/interaction.h \
    controller/core/core.h \
    model/file/file.h \
    model/file/loadmovie.h \
    model/file/savemovie.h \
    model/processing/processingbase.h \
    model/processing/flip.h \
    model/processing/processing.h \
    view/daemondbus.h \
    view/edit.h \
    view/information.h \
    view/kyview.h \
    global/variable.h \
    view/menumodule.h \
    view/navigator.h \
    view/openimage.h \
    view/showimagewidget.h \
    view/sidebar.h \
    view/sidebardelegate.h \
    view/sizedate.h \
    view/titlebar.h \
    view/toolbar.h \
    view/xatom-helper.h

RESOURCES += \
    ../res.qrc
