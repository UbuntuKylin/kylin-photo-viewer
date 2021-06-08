#include "variable.h"

//项目名称
const QString Variable::PROGRAM_NAME = QString("kylin-photo-viewer");
//临时文件路径
const QString Variable::TEMP_PATH = Variable::creatTempPath();

//QGSettings服务名称
const QString Variable::PHOTO_VIEW_GSETTINGS_SERVICENAME = QString("org.kylin-photo-viewer.settings");
//GSettings
QGSettings * Variable::m_settings = Variable::getSettings();

//DBUS服务名称
const QString Variable::PHOTO_VIEW_DBUS_SERVICENAME = QString("org.ukui.kylin_photo_viewer");
//DBUS路径
const QString Variable::PHOTO_VIEW_DBUS_PARH = QString("/");
//DBUS接口
const QString Variable::PHOTO_VIEW_DBUS_INTERFACE = QString("kylin_photo_viewer.commands");

//日志级别
QtMsgType Variable::g_logLevel = Variable::getLogLevel();
//日志路径
const QPair<QString,QString> Variable::LOG_PATH = Variable::getLogPath();
//日志是否写入文件
bool Variable::g_logToFile = Variable::getSettings("log-to-file").toBool();
//最大日志文件大小
const qint64 Variable::MAX_LOG_SIZE = 1024*1024; //1MB

//支持的命令列表
const QMap<QString,QString> Variable::SUPPORT_CMD = Variable::getSupportCmd();

//opencv支持的格式列表
const QStringList Variable::opencvCanSupportFormats={//"exr","EXR",有问题，opencv只支持其查看，不支持写入。此格式支持设置为壁纸
    "jpg","jpe","jpeg","pbm","pgm","ppm","sr","ras","png","bmp","dib","tiff","tif","pnm","webp"};
//const QStringList Variable::opencvCanSupportFormats={//"exr","EXR",有问题，opencv只支持其查看，不支持写入。此格式支持设置为壁纸
//    "JPG","JPE","JPEG","PBM","PGM","PPM","SR","RAS","PNG","BMP","DIB","TIFF","TIF","PNM","WEBP",
//    "jpg","jpe","jpeg","pbm","pgm","ppm","sr","ras","png","bmp","dib","tiff","tif","pnm","webp"};
//opencv不支持的格式列表
const QStringList Variable::opencvCannotSupportFormats={//"JP2"待添加,xbm不支持
    "tga","svg","gif","apng","ico"};
//const QStringList Variable::opencvCannotSupportFormats={//"JP2"待添加,xbm不支持
//    "TGA","SVG","GIF","APNG","ICO",
//    "tga","svg","gif","apng","ico"};
const QString Variable::SUPPORT_FORMATS_CLASSIFY="(*.jpg *.jpe *.jpeg);;"
                                                 "(*.pnm *.pbm);;"
                                                 "(*.pgm *.ppm);;"
                                                 "(*.tiff *.tif);;"
                                                 "(*.png *.apng);;"
                                                 "(*.ico);;(*.sr);;"
                                                 "(*.ras);;(*.tga);;"
                                                 "(*.svg);;(*.gif);;"
                                                 "(*.webp);;(*.bmp *.dib)";
//壁纸支持的格式列表
const QStringList Variable::BACKGROUND_SUPPORT_FORMATS={//经测试 "jp2","tga","dib","pbm","ppm"，"sr","ras",这几种格式不支持设置为壁纸
                                                        "jpg","jpe","jpeg","pgm","pnm","png","bmp","tiff","tif","svg","gif","apng"};
//支持的格式列表
const QStringList Variable::SUPPORT_FORMATS=Variable::creatSupportFormats();

const QSize Variable::ALBUM_IMAGE_SIZE = QSize(94 ,56); //相册缩略图尺寸
const QSize Variable::NAVIGATION_SIZE = QSize(200,133); //导航器尺寸
const int Variable::PICTURE_DEEPEN_KEY = 50; //图片加深值
const int Variable::REFRESH_RATE = 15; //刷新间隔
const int Variable::RESIZE_KEY = 5; //每次放大缩小的值
const int Variable::RESIZE_KEY_MAX = 1000; //能够放大的最大值
const int Variable::RESIZE_KEY_MIN = 5; //能够缩小的最小值
const int Variable::DEFAULT_MOVIE_TIME_INTERVAL = 100; //默认动图时间间隔

const QString Variable::API_TYPE = QString("$api$"); //是否为外部作为API调用的标识



QGSettings *Variable::getSettings()
{
    QGSettings *mysetting = nullptr;
    if (QGSettings::isSchemaInstalled(PHOTO_VIEW_GSETTINGS_SERVICENAME.toLocal8Bit())) {
        mysetting = new QGSettings(PHOTO_VIEW_GSETTINGS_SERVICENAME.toLocal8Bit());
        QObject::connect(mysetting, &QGSettings::changed,&Variable::onGsettingChange);
    }
    return mysetting;
}

QMap<QString, QString> Variable::getSupportCmd()
{
    QMap<QString, QString> cmds;
    cmds.insert("-next","下一张图片");
    cmds.insert("-back","上一张图片");
    cmds.insert("-big","放大显示图片");
    cmds.insert("-small","缩小显示图片");
    cmds.insert("-origin","显示原图");
    cmds.insert("-auto","自适应窗口大小显示图片");
    cmds.insert("-rotate","旋转图片");
    cmds.insert("-api","外部接口  -api -flip [path] [cmd] [saveway]\n"
                       "  ├-cmd: -v 垂直翻转 -h 水平翻转 -c 顺时针旋转90度 | 可重复使用，例如-vhcc \n"
                       "  └-saveway:  -b 备份保存 -r 覆盖保存 | 可不填，不填相当于-b");
    return cmds;
}

void Variable::onGsettingChange(const QString &key)
{
    if (key == "logLevel") {
        Variable::g_logLevel = getLogLevel();
        return;
    }
    if (key == "logToFile") {
        Variable::g_logToFile = m_settings->get("logToFile").toBool();
        return;
    }
}

QPair<QString, QString> Variable::getLogPath()
{
    QString logPath = "/run/user/";
    logPath += QString::number(getuid()) + "/";
    //创建目录
    QDir dir;
    if (!dir.exists(logPath)) {
        dir.mkdir(logPath);
    }
    QPair<QString, QString> pair;
    pair.first = logPath + PROGRAM_NAME + "_1"+ ".log";
    pair.second = logPath + PROGRAM_NAME + "_2"+ ".log";
    return pair;
}

QtMsgType Variable::getLogLevel()
{
    QString level = m_settings->get("logLevel").toString().toLower();
    if (level == "warning" || level == "1") {
        return QtWarningMsg;
    }
    if (level == "critical" || level == "2") {
        return QtCriticalMsg;
    }
    if (level == "fatal" || level == "3") {
        return QtFatalMsg;
    }
    if (level == "info" || level == "4") {
        return QtInfoMsg;
    }
    return QtDebugMsg;
}

const QString Variable::creatTempPath()
{
    const QString filePath = "/tmp/."+PROGRAM_NAME+"/";
    QDir dir;
    if (!dir.exists(filePath)) {
        dir.mkdir(filePath);
    }
    return filePath;
}

QStringList Variable::creatSupportFormats()
{
    QStringList list;
    list.append(Variable::opencvCanSupportFormats);
    list.append(Variable::opencvCannotSupportFormats);
    return list;
}

//写入配置文件
void Variable::setSettings(const QString &key, const QVariant &vlue)
{
    m_settings->set(key,vlue);
}

//读取配置文件
QVariant Variable::getSettings(const QString &key)
{
    QVariant setting;
    setting =m_settings->get(key);
    if (key == "imagePath") {
        if (setting.toString().isEmpty()) {
            return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
        }
    }
    return setting;
}
