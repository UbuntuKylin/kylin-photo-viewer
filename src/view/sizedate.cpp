#include "sizedate.h"

//主界面默认高度和宽度
const int SizeDate::DEFAULTWIDTH = 1080;
const int SizeDate::DEFAULTHEIGHT = 720;
//主界面最小尺寸
const QSize SizeDate::MINISIZE = QSize(678,678);
//两栏高度
const int SizeDate::BARHEIGHT = 40;
//图标大小
const QSize SizeDate::ICONSIZE = QSize(30,30);

//标题栏logo尺寸
const QSize SizeDate::LOGOBTN = QSize(24,24);
//标题栏按钮尺寸
const QSize SizeDate::TITLEBTN = QSize(30,30);

//打卡图片界面默认高度和宽度
const QSize SizeDate::OPENIMAGESIZE = QSize(128, 128 + 50);
//打卡图片界面--中间打开圆形图标的默认高度和宽度
const QSize SizeDate::OPENINCENTER_SIZE = QSize(128, 128);
//打卡图片界面--中间打开加好号图标的默认高度和宽度
const QSize SizeDate::ADDFILESIZE = QSize(40, 40);
//打开图片界面需要的固定值
const int SizeDate::OPENIMAGEFIX = 128;

//导航栏尺寸
const QSize SizeDate::NAVISIZE = QSize(200, 40);

//信息栏尺寸
const QSize SizeDate::INFORSIZE = QSize(207,197 + 18);

//工具栏尺寸
const QSize SizeDate::TOOLBARSIZE = QSize(479 +4+14 ,40 + 4);
//工具栏按钮尺寸
const QSize SizeDate::TOOLBUTTON = QSize(24,24);
//工具栏缩放率尺寸
const QSize SizeDate::TOOLPER = QSize(43,16);
//工具栏两条线尺寸
const QSize SizeDate::TOOLLINE = QSize(1,22);
//工具栏阴影颜色
const QColor SizeDate::TOOLCOLOR = QColor(54,54,54);
//工具栏模糊半径
const int SizeDate::BLURRADIUS = 10;

//图片界面按钮尺寸
const QSize SizeDate::IMAGEBUTTON = QSize(56,56);
//图片界面icon尺寸
const QSize SizeDate::IMAGEICON = QSize(56,56);
//图片界面按钮距边距尺寸
const int SizeDate::LEFTPOS = 43;

//菜单栏图标尺寸-小
const QSize SizeDate::MICONSIZES = QSize(16,16);
//菜单栏图标尺寸-中
const QSize SizeDate::MICONSIZEM = QSize(24,24);
//菜单栏图标尺寸-大
const QSize SizeDate::MICONSIZEB = QSize(96,96);
//菜单栏按钮尺寸
const QSize SizeDate::MBTNSIZE = QSize(30,30);
//菜单栏关于界面尺寸
const QSize SizeDate::MABOUT = QSize(420,324);
//菜单-关于界面appname高度
const int SizeDate::NAMEHEIGHT = 28;
//菜单-关于界面AppVersion高度
const int SizeDate::VERSIONHEI = 24;
//菜单-关于界面版本号
const QString SizeDate::VERSIONNEM = "1.0.0";
//菜单-关于界面用户手册
const QString SizeDate::USERGUIDE = "kylin-photo-viewer";
