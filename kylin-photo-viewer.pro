QT       += core gui
QT       += svg
QT       += dbus
QT       += x11extras KWindowSystem
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# 适配窗口管理器圆角阴影
LIBS +=-lpthread
LIBS +=-lX11
TARGET = kylin-photo-viewer
TEMPLATE = app

TRANSLATIONS += \
    translations/kylin-photo-viewer_zh_CN.ts

qm_files.files = translations/*.qm
qm_files.path = /usr/share/kylin-photo-viewer/translations/

target.path = /usr/bin
target.source +=$$TARGET

desktop.files = kylin-photo-viewer.desktop
desktop.path = /usr/share/applications/

appdesktop.files += kylin-photo-viewer.desktop
appdesktop.path = /usr/share/applications/

icons.files = res/kyview_logo.png
icons.path = /usr/share/pixmaps/

INSTALLS += target desktop appdesktop icons qm_files

CONFIG += link_pkgconfig
PKGCONFIG += gsettings-qt
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#opencv
INCLUDEPATH += /usr/include/opencv4/
LIBS += -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_imgproc \

SOURCES += src/main.cpp \
    src/controller/core/albumthumbnail.cpp \
    src/global/log.cpp \
    src/model/dbus.cpp \
    src/controller/interaction.cpp \
    src/controller/core/core.cpp \
    src/model/file.cpp \
    src/model/processing/flip.cpp \
    src/model/processing/processing.cpp \
    src/view/daemondbus.cpp \
    src/view/information.cpp \
    src/view/kyview.cpp \
    src/global/variable.cpp \
    src/view/menumodule.cpp \
    src/view/navigator.cpp \
    src/view/openimage.cpp \
    src/view/showimagewidget.cpp \
    src/view/sidebar.cpp \
    src/view/titlebar.cpp \
    src/view/toolbar.cpp \
    src/view/xatom-helper.cpp

HEADERS += \
    src/controller/core/albumthumbnail.h \
    src/global/log.h \
    src/model/dbus.h \
    src/controller/interaction.h \
    src/controller/core/core.h \
    src/model/file.h \
    src/model/processing/_processing.h \
    src/model/processing/flip.h \
    src/model/processing/processing.h \
    src/view/daemondbus.h \
    src/view/information.h \
    src/view/kyview.h \
    src/global/variable.h \
    src/view/menumodule.h \
    src/view/navigator.h \
    src/view/openimage.h \
    src/view/showimagewidget.h \
    src/view/sidebar.h \
    src/view/titlebar.h \
    src/view/toolbar.h \
    src/view/xatom-helper.h



# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    src/res/侧边缩略图.png \
#    src/res/侧边缩略图hover@1x.png \
#    src/res/信息.png \
#    src/res/信息hover.png \
#    src/res/删除.png \
#    src/res/删除hover.png \
#    src/res/原始大小.png \
#    src/res/原始大小hover.png \
#    src/res/向右旋转.png \
#    src/res/向右旋转hover.png \
#    src/res/放大.png \
#    src/res/放大hover.png \
#    src/res/标注.png \
#    src/res/标注hover.png \
#    src/res/滤镜.png \
#    src/res/滤镜hover.png \
#    src/res/箭头.png \
#    src/res/缩小.png \
#    src/res/缩小hover.png \
#    src/res/裁剪.png \
#    src/res/裁剪hover.png \
#    src/res/适应窗口.png \
#    src/res/适应窗口hover.png \
#    src/res/镜像.png \
#    src/res/镜像hover.png \
#    src/res/镜像上下.png \
#    src/res/镜像上下hover.png

RESOURCES += \
    res.qrc
