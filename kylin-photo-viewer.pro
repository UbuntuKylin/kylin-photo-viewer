TEMPLATE = subdirs
SUBDIRS = \
   # plugins \
    src \

TRANSLATIONS += \
    translations/kylin-photo-viewer_zh_CN.ts

qm_files.files = translations/*.qm
qm_files.path = /usr/share/kylin-photo-viewer/translations/

desktop.files = kylin-photo-viewer.desktop
desktop.path = /usr/share/applications/

appdesktop.files += kylin-photo-viewer.desktop
appdesktop.path = /usr/share/applications/

icons.files = res/kyview_logo.png
icons.path = /usr/share/pixmaps/

INSTALLS +=desktop appdesktop icons qm_files

