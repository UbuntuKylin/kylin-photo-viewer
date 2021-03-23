#ifndef VARIABLE_H
#define VARIABLE_H

//#include <opencv4/opencv2/opencv.hpp>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <QString>
#include <QSettings>
#include <QDir>
#include <QSize>
#include <QPixmap>
#include <QDebug>

class Variable
{

public:
    static const QString PROGRAM_NAME; //项目名称
    static const QString PHOTO_VIEW_DBUS_SERVICENAME; //DBUS服务名称
    static const QString TEMP_PATH; //临时文件路径
    static const QString PHOTO_VIEW_DBUS_PARH; //DBUS路径
    static const QString PHOTO_VIEW_DBUS_INTERFACE;//DBUS接口
    enum LOGLEVEL{ NOLOG , FATAL , CRITICAL , INFO , DEBUG , WARNING }; //日志级别
    static const QString ENV_LOGLEVEL; //日志级别环境变量名
    static const bool LOGTOFILE; //日志写入文件
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
    static QSettings *_settings;
    static const QString IMAGEPATH;
    static const int BAR_HEIGHT;//顶栏和工具栏的高度

private:
    static const QStringList _OPENCV_CAN_SUPPORT_FORMATS; //opencv支持的格式列表
    static const QStringList _OPENCV_CANNOT_SUPPORT_FORMATS; //opencv不支持的格式列表

    static QSettings *_getSettings();
    static QStringList _creatSupportFormats();
    static const QString _creatTempPath();
    static QMap<QString,QString> _getSupportCmd();

};

#endif // VARIABLE_H
