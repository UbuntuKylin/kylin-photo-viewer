#ifndef SIZEDATE_H
#define SIZEDATE_H
#include <QString>
#include <QSize>
#include <QColor>

static const int DEFAULT_WIDTH = 1080;//主界面默认宽度
static const int DEFAULT_HEIGHT = 720;//主界面默认高度
static const QSize MINI_SIZE = QSize(678,678);//主界面最小尺寸
static const QSize LOGO_BTN = QSize(24,24);//标题栏logo按钮尺寸
static const QSize TITLE_BTN = QSize(30,30);//标题栏按钮尺寸
static const int BAR_HEIGHT = 40;//标题栏和工具栏的高度
static const QSize ICON_SIZE = QSize(30,30);//图标大小
static const QSize OPEN_IMAGESIZE = QSize(128, 128 + 50);//打开图片界面尺寸
static const QSize OPENINCENTER_SIZE = QSize(128, 128);//打开图片界面--中间打开圆形图标按钮尺寸
static const QSize ADD_FILESIZE = QSize(40, 40);//打开图片界面--中间打开加号图标按钮尺寸
static const int OPEN_IMAGEFIX = 128;//打开图片界面的固定值
static const QSize NAVI_SIZE = QSize(200, 40);//导航栏尺寸
static const QSize INFOR_SIZE = QSize(207 + 6,197 + 18 +20 + 5);//信息栏尺寸
static const QSize TOOLBAR_SIZE = QSize(479 +4 ,40 + 4);//工具栏尺寸
static const QSize TOOL_BUTTON = QSize(24,24);//工具栏按钮尺寸
static const QSize TOOL_PER = QSize(45,16);//工具栏缩放率尺寸
static const QSize TOOL_LINE = QSize(1,22);//工具栏两条线的尺寸
static const QColor TOOL_COLOR = QColor(0,0,0,80);//工具栏阴影颜色(54,54,54)
static const int BLUR_RADIUS = 10;//工具栏模糊度
static const QColor ALBUM_COLOR = QColor(5,15,25,133);//相册阴影颜色
static const int ALBUM_RADIUS = 20;//相册阴影模糊度
static const QSize IMAGE_BUTTON = QSize(56,56);//图片界面按钮尺寸
static const QSize IMAGE_ICON = QSize(56,56);//图片界面icon尺寸
static const int LEFT_POS = 43;//图片界面按钮距边距尺寸
static const QSize MICON_SIZES = QSize(16,16);//菜单栏图标尺寸-小
static const QSize MICON_SIZEM = QSize(24,24);//菜单栏图标尺寸-中
static const QSize MICON_SIZEB = QSize(96,96);//菜单栏图标尺寸-大
static const QSize MBTN_SIZE = QSize(30,30);//菜单栏按钮尺寸
static const QSize M_ABOUT = QSize(420,324);//菜单-关于界面尺寸
static const int NAME_HEIGHT = 28;//菜单-关于界面appname高度
static const int VERSION_HEI = 24;//菜单-关于界面AppVersion高度
static const QString VERSION_NEM = "1.1.0";//菜单-关于界面版本号
static const QString USER_GUIDE = "pictures";//菜单-关于界面用户手册
static const QSize SIDEBAR_SIZE = QSize(126 ,590);//相册尺寸
static const QSize SIDEITEM_SIZE = QSize(94 + 12,56 +8 );//相册item尺寸
static const QSize TOOLZOOM_SIZE = QSize(113,24);//工具栏缩放widget尺寸

#endif // SIZEDATE_H
