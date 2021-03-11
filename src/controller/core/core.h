#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QPixmap>
#include <QColor>
#include "src/model/dbus.h"
#include "src/model/file.h"
#include "src/model/processing/processing.h"
#include "albumthumbnail.h"

class ImageShowStatus //记录显示图片状态
{
public:
    enum ChangeShowSizeType{Big,Small,Origin,Auto};//放大、缩小、原图、自适应
    QMap<int,QString> _imageUrlMap;//图片队列
    QSize _size;//记录当前窗口大小
    QFileInfo _info;
    int _maxType;//记录动态分配标签的最大值
    int _nowType;//记录当前打开
    int _backType;//记录上次打开
    Mat _nowMat;//记录当前打开
    Mat _backMat;//记录上次打开
    QPixmap _nowImage;
    Mat _changeImage(Mat mat);
    void _changeImageType(int num = 0);
    int _proportion;//默认图片显示比例
    bool _isNavigationShow;//是否显示导航器
};

class Core : public QObject , ImageShowStatus
{
    Q_OBJECT

signals:
    void needOpenImage(QString path);
    void openFinish(QVariant var);
    void albumFinish(QVariant var);
    void showNavigation(QPixmap pix);

public:
    Core();
    void loadCoreModel(QStringList arguments);//初始化核心功能
    QVariant findAllImageFromeDir(QString fullPath);//寻找目录下所有支持的图片
    QVariant openImage(QString fullPath);//打开图片
    void changeImage(const int &type); //切换图片
    void changeWidgetSize(const QSize &size);//切换窗口大小
    void changeImageShowSize(ImageShowStatus::ChangeShowSizeType type);//图片显示状态（放大缩小）
    void clickNavigation(const QPoint &point);//导航器点击
    void flipImage(const Processing::FlipWay &way);//翻转处理
    void deleteImage();//删除图片

private:
    void _initCore();//初始化核心
    void _initDbus(const QStringList &arguments);//初始化Dbus模块
    Dbus *_dbus = nullptr;//DBus模块对象
    void _showImage(const QPixmap &pix);//显示图片
    void _creatImage(const int &proportion = -1);//生成图像
    void _navigation(const QPoint &point = QPoint(-1,-1));//导航器
    void _processingCommand(const QStringList &cmd);//处理终端命令
    void _loadAlbum();//加载相册
    QPoint _clickBeforePosition;//记录上次点击区域，用于节省算力
};

#endif // CORE_H
