#include "variable.h"

const QString Variable::PROGRAM_NAME = QString("kylin-photo-viewer"); //项目名称
//const QString Variable::TEMP_PATH = Variable::_creatTempPath(); //临时文件路径
const QString Variable::PHOTO_VIEW_DBUS_SERVICENAME = QString("org.ukui.kylin_photo_viewer"); //DBUS服务名称
const QString Variable::PHOTO_VIEW_DBUS_PARH = QString("/"); //DBUS路径
const QString Variable::PHOTO_VIEW_DBUS_INTERFACE = QString("kylin_photo_viewer.commands");//DBUS接口
const QString Variable::ENV_LOGLEVEL = QString("UKPV_DEBUG"); //日志级别环境变量名
const QMap<QString,QString> Variable::SUPPORT_CMD = Variable::_getSupportCmd(); //支持的命令列表
const QStringList Variable::SUPPORT_FORMATS={
    //opencv支持的格式列表
    "JPG","JPE","JPEG","JP2","EXR","PBM","PGM","PPM","SR","RAS","PNG","BMP","DIB","TIFF","TIF","TGA"
    ,"jpg","jpe","jpeg","jp2","exr","pbm","pgm","ppm","sr","ras","png","bmp","dib","tiff","tif","tga"
    //opencv不支持的格式列表
    ,"SVG","GIF","APNG"
    ,"svg","gif","apng"
    };
const QSize Variable::ALBUM_IMAGE_SIZE = QSize(94,58); //相册缩略图尺寸
const QSize Variable::NAVIGATION_SIZE = QSize(200,133); //导航器尺寸
const int Variable::PICTURE_DEEPEN_KEY = 50; //图片加深值
const int Variable::REFRESH_RATE = 15; //刷新间隔
const int Variable::RESIZE_KEY = 10; //每次放大缩小的值
const int Variable::RESIZE_KEY_MAX = 1000; //能够放大的最大值
const int Variable::RESIZE_KEY_MIN = 5; //能够缩小的最小值
const int Variable::DEFAULT_MOVIE_TIME_INTERVAL = 100; //默认动图时间间隔

const int Variable::BAR_HEIGHT = 40;

QSettings * Variable::_settings = Variable::_getSettings();
const bool Variable::LOGTOFILE = Variable::_settings->value("logToFile").toBool(); //日志写入文件

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
    cmds.insert("-big","放大显示图片");
    cmds.insert("-small","缩小显示图片");
    cmds.insert("-origin","显示原图");
    cmds.insert("-auto","自适应窗口大小显示图片");
    cmds.insert("-rotate","旋转图片");
    cmds.insert("-fullscreen","全屏");
    return cmds;
}

//const QString Variable::_creatTempPath()
//{
//    const QString filePath = "/tmp/."+PROGRAM_NAME+"/";
//    QDir dir;
//    if(!dir.exists(filePath))
//        dir.mkdir(filePath);
//    return filePath;
//}
