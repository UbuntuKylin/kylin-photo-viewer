#ifndef VARIABLE_H
#define VARIABLE_H

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <QString>
#include <QSettings>
#include <QDir>
#include <QSize>
#include <QPixmap>
#include <QDebug>

struct ImageAndInfo
{
    QFileInfo info; //信息
    QPixmap image; //图片
    unsigned int type; //标签
    unsigned int proportion; //比例
};
Q_DECLARE_METATYPE(ImageAndInfo)//QVarant 注册自定义类型

class Variable
{

public:
    static const QString PROGRAM_NAME; //项目名称
    static const QString PHOTO_VIEW_DBUS_SERVICENAME; //DBUS服务名称
    static const QString PHOTO_VIEW_DBUS_PARH; //DBUS路径
    static const QString PHOTO_VIEW_DBUS_INTERFACE;//DBUS接口
    enum LOGLEVEL{ NOLOG , FATAL , CRITICAL , INFO , DEBUG , WARNING }; //日志级别
    static const QString ENV_LOGLEVEL; //日志级别环境变量名
    static const bool LOGTOFILE; //日志写入文件
    static const QMap<QString,QString> SUPPORT_CMD; //支持的命令列表
    static const QStringList SUPPORT_FORMATS; //支持的格式列表
    static const QSize ALBUM_IMAGE_SIZE; //相册缩略图尺寸
    static const int REFRESH_RATE; //刷新间隔

private:
    static QSettings *_getSettings();
    static QSettings *_settings;
    static QMap<QString,QString> _getSupportCmd();

};

#endif // VARIABLE_H
