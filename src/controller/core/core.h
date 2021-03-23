#ifndef CORE_H
#define CORE_H

#include "base.h"

#define SET_BACKGROUND_PICTURE_GSETTINGS_PATH "org.mate.background"
#define SET_BACKGROUND_PICTURE_GSETTINGS_NAME "pictureFilename"

class Core : public QObject , public NavigationStatus
{
    Q_OBJECT

signals:
    void needOpenImage(QString path);
    void openFinish(QVariant var);
    void albumFinish(QVariant var);
    void showNavigation(QPixmap pix);
    void deleteImageOnAlbum(int type);

public:
    Core();
    QString initDbus(const QStringList &arguments);//初始化Dbus模块
    QVariant findAllImageFromeDir(QString fullPath);//寻找目录下所有支持的图片
    QVariant openImage(QString fullPath);//打开图片
    void changeImage(const int &type); //切换图片
    void changeWidgetSize(const QSize &size);//切换窗口大小
    void changeImageShowSize(ImageShowStatus::ChangeShowSizeType type);//图片显示状态（放大缩小）
    void clickNavigation(const QPoint &point = QPoint(-1,-1));//导航器点击
    void flipImage(const Processing::FlipWay &way);//翻转处理
    void deleteImage();//删除图片
    void setAsBackground();//设置为背景图

private:
    void _initCore();//初始化核心
    Dbus *_dbus = nullptr;//DBus模块对象
    void _showImage(const QPixmap &pix);//显示图片
    void _showImageOrMovie();//显示图片或动图
    void _creatImage(const int &proportion = -1);//生成图像
    void _processingCommand(const QStringList &cmd);//处理终端命令
    void _loadAlbum();//加载相册
    void _navigation(const QPoint &point = QPoint(-1,-1));//导航器
    void _playMovie();//播放动图的槽函数

};

#endif // CORE_H
