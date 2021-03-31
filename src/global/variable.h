#ifndef VARIABLE_H
#define VARIABLE_H

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <QString>
#include <QGSettings>
#include <QDir>
#include <QSize>
#include <QPixmap>
#include <QStandardPaths>
#include <QDebug>
#include <unistd.h>

struct ImageAndInfo
{
    QFileInfo info; //信息
    QPixmap image; //图片
    int type; //标签
    int proportion; //比例
    QString colorSpace;//图片空间
    QString imageSize;//图片尺寸
};
Q_DECLARE_METATYPE(ImageAndInfo)//QVarant 注册自定义类型

class Variable
{

public:
    static const QString PROGRAM_NAME; //项目名称
    static const QString PHOTO_VIEW_DBUS_SERVICENAME; //DBUS服务名称
    static const QString PHOTO_VIEW_GSETTINGS_SERVICENAME; //QGSettings服务名称
    static const QString TEMP_PATH; //临时文件路径
    static const QString PHOTO_VIEW_DBUS_PARH; //DBUS路径
    static const QString PHOTO_VIEW_DBUS_INTERFACE;//DBUS接口
    static const QPair<QString,QString> LOG_PATH; //日志路径
    static QtMsgType g_logLevel; //日志级别
    static bool g_logToFile; //日志写入文件
    static const qint64 MAX_LOG_SIZE;//最大日志文件大小
    static const QMap<QString,QString> SUPPORT_CMD; //支持的命令列表
    static const QStringList SUPPORT_FORMATS; //支持的格式列表
    static const QStringList BACKGROUND_SUPPORT_FORMATS; //壁纸支持的格式列表
    static const QSize ALBUM_IMAGE_SIZE; //相册缩略图尺寸
    static const QSize NAVIGATION_SIZE; //导航器尺寸
    static const int PICTURE_DEEPEN_KEY; //图片加深值
    static const int REFRESH_RATE; //刷新间隔
    static const int RESIZE_KEY; //每次放大缩小的值
    static const int RESIZE_KEY_MAX; //能够放大的最大值
    static const int RESIZE_KEY_MIN; //能够缩小的最小值
    static const int DEFAULT_MOVIE_TIME_INTERVAL; //默认动图时间间隔
    static const int BAR_HEIGHT;//顶栏和工具栏的高度
    static void setSettings(const QString &key,const QVariant &vlue);//写入配置文件
    static QVariant getSettings(const QString &key);//读取配置文件

private:
    static const QStringList opencvCanSupportFormats; //opencv支持的格式列表
    static const QStringList opencvCannotSupportFormats; //opencv不支持的格式列表
    static QStringList creatSupportFormats();//所有支持格式
    static QGSettings *m_settings;//配置文件对象
    static QGSettings *getSettings();//初始化gsettings
    static const QString creatTempPath();//创建临时目录
    static QMap<QString,QString> getSupportCmd();//创建支持的命令列表
    static void onGsettingChange(const QString &key);//监听gsettings改变槽函数
    static QPair<QString,QString> getLogPath();//获取日志目录
    static QtMsgType getLogLevel();//获取日志级别

};

#endif // VARIABLE_H
