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
    enum LOGLEVEL{ NOLOG , RELEASE , NORMAL , DEBUG , ALL }; //日志级别
    static const QString ENV_LOGLEVEL; //日志级别环境变量名
    static const bool LOGTOFILE; //日志写入文件

private:
    static QSettings *_getSettings();
    static QSettings *_settings;

};

#endif // VARIABLE_H
