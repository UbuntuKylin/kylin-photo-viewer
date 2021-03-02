#include "variable.h"
#include <QDebug>

const QString Variable::PROGRAM_NAME = QString("kylin-photo-viewer");
const QString Variable::PHOTO_VIEW_DBUS_SERVICENAME = QString("org.ukui.kylin_photo_viewer");
const QString Variable::PHOTO_VIEW_DBUS_PARH = QString("/");
const QString Variable::PHOTO_VIEW_DBUS_INTERFACE = QString("kylin_photo_viewer.commands");
const QString Variable::ENV_LOGLEVEL = QString("UKPV_DEBUG");
const QMap<QString,QString> Variable::SUPPORT_CMD = Variable::_getSupportCmd();
const QStringList Variable::SUPPORT_FORMATS={"jpg","jpeg","png","gif","bmp"}; //支持的格式列表


QSettings * Variable::_settings = Variable::_getSettings();
const bool Variable::LOGTOFILE = Variable::_settings->value("logToFile").toBool();

QSettings *Variable::_getSettings()
{
    QString filePath = QDir::homePath()+"/.config/"+PROGRAM_NAME+"/";
    QDir dir;
    if(!dir.exists(filePath))
        dir.mkdir(filePath);
    QString fileName = filePath + "settings";
    //读取配置文件
    QSettings *_setting = new QSettings(fileName,QSettings::IniFormat);
    _setting->beginGroup("setting");
    //读取配置
    QString logLevel = _setting->value("logLevel").toString();
    if(logLevel==""){
        logLevel="DEBUG";
        _setting->setValue("logLevel",logLevel);
    }
    setenv(ENV_LOGLEVEL.toLocal8Bit().data(),logLevel.toLocal8Bit().data(),1);
    return _setting;
}

QMap<QString, QString> Variable::_getSupportCmd()
{
    QMap<QString, QString> cmds;
    cmds.insert("-next","下一张图片");
    cmds.insert("-back","上一张图片");
    cmds.insert("-rotate","旋转图片");
    cmds.insert("-fullscreen","全屏");
    return cmds;
}
