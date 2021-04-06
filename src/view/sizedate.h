#ifndef SIZEDATE_H
#define SIZEDATE_H
#include <QString>
#include <QSize>
#include <QColor>

class SizeDate
{
public:
    static const int DEFAULTWIDTH;//主界面默认宽度
    static const int DEFAULTHEIGHT;//主界面默认高度
    static const QSize MINISIZE;//主界面最小尺寸

    static const QSize LOGOBTN;//标题栏logo按钮尺寸
    static const QSize TITLEBTN;//标题栏按钮尺寸

    static const int BARHEIGHT;//顶栏和工具栏的高度

    static const QSize ICONSIZE;//图标大小
    static const QSize OPENIMAGESIZE;//打开图片界面尺寸
    static const QSize OPENINCENTER_SIZE;//打开图片界面--中间打开圆形图标按钮尺寸
    static const QSize ADDFILESIZE;//打开图片界面--中间打开加号图标按钮尺寸
    static const int OPENIMAGEFIX;//打开图片界面的固定值

    static const QSize NAVISIZE;//导航栏尺寸
    static const QSize INFORSIZE;//信息栏尺寸

    static const QSize TOOLBARSIZE;//工具栏尺寸
    static const QSize TOOLBUTTON;//工具栏按钮尺寸
    static const QSize TOOLPER;//工具栏缩放率尺寸
    static const QSize TOOLLINE;//工具栏两条线的尺寸
    static const QColor TOOLCOLOR;//工具栏两条线的尺寸
    static const int BLURRADIUS;//工具栏两条线的尺寸

    static const QSize IMAGEBUTTON;//图片界面按钮尺寸
    static const QSize IMAGEICON;//图片界面icon尺寸
    static const int LEFTPOS;//图片界面按钮距边距尺寸

    static const QSize MICONSIZES;//菜单栏图标尺寸-小
    static const QSize MICONSIZEM;//菜单栏图标尺寸-中
    static const QSize MICONSIZEB;//菜单栏图标尺寸-大
    static const QSize MBTNSIZE;//菜单栏按钮尺寸
    static const QSize MABOUT;//菜单-关于界面尺寸
    static const int NAMEHEIGHT;//菜单-关于界面appname高度
    static const int VERSIONHEI;//菜单-关于界面AppVersion高度
    static const QString VERSIONNEM;//菜单-关于界面版本号
    static const QString USERGUIDE;//菜单-关于界面用户手册
};

#endif // SIZEDATE_H
