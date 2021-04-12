#ifndef SIZEDATE_H
#define SIZEDATE_H
#include <QString>
#include <QSize>
#include <QColor>

static const int DEFAULTWIDTH = 1080;//主界面默认宽度
static const int DEFAULTHEIGHT = 720;//主界面默认高度
static const QSize MINISIZE = QSize(678,678);//主界面最小尺寸

static const QSize LOGOBTN = QSize(24,24);//标题栏logo按钮尺寸
static const QSize TITLEBTN = QSize(30,30);//标题栏按钮尺寸

static const int BARHEIGHT = 40;//顶栏和工具栏的高度

static const QSize ICONSIZE = QSize(30,30);//图标大小
static const QSize OPENIMAGESIZE = QSize(128, 128 + 50);//打开图片界面尺寸
static const QSize OPENINCENTER_SIZE = QSize(128, 128);//打开图片界面--中间打开圆形图标按钮尺寸
static const QSize ADDFILESIZE = QSize(40, 40);//打开图片界面--中间打开加号图标按钮尺寸
static const int OPENIMAGEFIX = 128;//打开图片界面的固定值

static const QSize NAVISIZE = QSize(200, 40);//导航栏尺寸
static const QSize INFORSIZE = QSize(207,197 + 18);//信息栏尺寸

static const QSize TOOLBARSIZE = QSize(479 +4 ,40 + 4);//工具栏尺寸
static const QSize TOOLBUTTON = QSize(24,24);//工具栏按钮尺寸
static const QSize TOOLPER = QSize(43,16);//工具栏缩放率尺寸
static const QSize TOOLLINE = QSize(1,22);//工具栏两条线的尺寸
static const QColor TOOLCOLOR = QColor(54,54,54);//工具栏阴影颜色
static const int BLURRADIUS = 10;//工具栏模糊度

static const QSize IMAGEBUTTON = QSize(56,56);//图片界面按钮尺寸
static const QSize IMAGEICON = QSize(56,56);//图片界面icon尺寸
static const int LEFTPOS = 43;//图片界面按钮距边距尺寸

static const QSize MICONSIZES = QSize(16,16);//菜单栏图标尺寸-小
static const QSize MICONSIZEM = QSize(24,24);//菜单栏图标尺寸-中
static const QSize MICONSIZEB = QSize(96,96);//菜单栏图标尺寸-大
static const QSize MBTNSIZE = QSize(30,30);//菜单栏按钮尺寸
static const QSize MABOUT = QSize(420,324);//菜单-关于界面尺寸
static const int NAMEHEIGHT = 28;//菜单-关于界面appname高度
static const int VERSIONHEI = 24;//菜单-关于界面AppVersion高度

static const QString VERSIONNEM = "1.0.0";//菜单-关于界面版本号
static const QString USERGUIDE = "kylin-photo-viewer";//菜单-关于界面用户手册

static const QSize SIDEBARSIZE = QSize(126 + 6,590);//相册尺寸
static const QSize SIDEITEMSIZE = QSize(94,56);//相册item尺寸

#endif // SIZEDATE_H
