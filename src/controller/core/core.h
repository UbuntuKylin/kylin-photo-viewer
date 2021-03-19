#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QGSettings>
#include "model/dbus.h"
#include "model/file.h"
#include "model/processing/processing.h"
#include "albumthumbnail.h"

#define SET_BACKGROUND_PICTURE_GSETTINGS_PATH "org.mate.background"
#define SET_BACKGROUND_PICTURE_GSETTINGS_NAME "pictureFilename"

class ImageShowStatus //显示图片细节相关，记录显示图片状态
{
public:
    enum ChangeShowSizeType{Big,Small,Origin,Auto};//放大、缩小、原图、自适应
    QMap<unsigned int,QString> _imageUrlMap;//图片队列
    QSize _size;//记录当前窗口大小
    QFileInfo _info;
    unsigned int _maxType;//记录动态分配标签的最大值
    unsigned int _nowType;//记录当前打开
    unsigned int _backType;//记录上次打开
    QString _nowpath;//记录当前路径
    QString _backpath;//记录上次路径
    Mat _nowMat;//记录当前打开
    Mat _backMat;//记录上次打开
    QPixmap _nowImage;
    Mat _changeImage(Mat mat);
    void _changeImageType(int num = 0);
    unsigned int _proportion;//图片显示比例
    QSize _tmpSize;//按比例缩放后的图片大小
    QString _imageSize;//原图大小
    QString _colorSpace;//颜色域
    bool _isNavigationShow;//是否显示导航器
    QTimer *_playMovieTimer;//播放动图的计时器
    QList<Mat> *_matList;//存储动图的每一帧
    unsigned int _matListIndex;//当前播放到哪一帧
    QPoint _startShowPoint;
    unsigned int _fps;//帧率
};

class NavigationStatus : public ImageShowStatus//导航器相关单独写，提高可读性
{
public:
    QPoint _clickBeforePosition;//记录上次点击区域，用于提升体验
    QPoint _clickBeforeStartPosition;//记录上次点击区域，用于节省算力
    void _creatNavigation();//创建导航器图片等数据，用于节省算力
    QPixmap _showPix;//待显示图
    QImage _navigationImage;//导航栏背景
    QSize _hightlightSize; //高亮区域大小;
    unsigned int _spaceWidth;//导航栏窗口与缩略图左边缘距离
    unsigned int _spaceHeight;//导航栏窗口与缩略图上边缘距离
};

class Core : public QObject , public NavigationStatus
{
    Q_OBJECT

signals:
    void needOpenImage(QString path);
    void openFinish(QVariant var);
    void albumFinish(QVariant var);
    void showNavigation(QPixmap pix);

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
