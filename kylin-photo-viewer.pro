TEMPLATE = subdirs
SUBDIRS = \
    plugins \
    src \

TRANSLATIONS += \
    translations/kylin-photo-viewer_zh_CN.ts
!system($$PWD/translations/generate_translations_pm.sh): error("Failed to generate pm")
qm_files.files = translations/*.qm
qm_files.path = /usr/share/kylin-photo-viewer/translations/

settings.files = data/org.kylin.photo.viewer.gschema.xml
settings.path = /usr/share/glib-2.0/schemas/

desktop.files = data/kylin-photo-viewer.desktop
desktop.path = /usr/share/applications/


icons.files = res/kyview_logo.png
icons.path = /usr/share/pixmaps/

INSTALLS +=desktop settings icons qm_files

