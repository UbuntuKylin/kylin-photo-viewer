#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <QSettings>
#include <QDir>

class Variable
{

public:
    static const QString PROGRAM_NAME; //项目名称
    static const QString PHOTO_VIEW_DBUS_SERVICENAME; //DBUS服务名称
    static const QString PHOTO_VIEW_DBUS_PARH; //DBUS路径
    static const QString PHOTO_VIEW_DBUS_INTERFACE;//DBUS接口
    enum LOGLEVEL{ NOLOG , RELEASE , NORMAL , DEBUG , ALL }; //日志级别
    static const QString ENV_LOGLEVEL; //日志级别环境变量名
    static const bool LOGTOFILE; //日志写入文件
    static const QMap<QString,QString> SUPPORT_CMD; //支持的命令列表
    static const QStringList SUPPORT_FORMATS; //支持的格式列表

private:
    static QSettings *_getSettings();
    static QSettings *_settings;
    static QMap<QString,QString> _getSupportCmd();

};

#endif // VARIABLE_H
