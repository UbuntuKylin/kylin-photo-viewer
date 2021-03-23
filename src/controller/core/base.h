#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QGSettings>
#include "model/dbus.h"
#include "model/file/file.h"
#include "model/processing/processing.h"
#include "albumthumbnail.h"

class Enums{
public:
    enum ChamgeImageType{nextImage=-1,backImage=-2};
    enum ChangeShowSizeType{Big,Small,Origin,Auto};//放大、缩小、原图、自适应
};

class ImageUrlList  : public Enums//图片&地址映射的队列
{
public:
    ImageUrlList();
    typedef QPair<int,QString> Pair;
    QList<int> keys();
    void remove(const int &type);
    ChamgeImageType nextOrBack(const QString &oldPath,const QString &newPath);
    int backKey(const int &key);
    int nextKey(const int &key);
    QString getPath(const int &key);
    bool isEmpty();
    void clear();
    int length();
    void append(const int &key,const QString &path);
    void append(const ImageUrlList &list);

protected:
    QList<QPair<int,QString>> _list;
};

class ImageShowStatus : public Enums //显示图片细节相关，记录显示图片状态
{
public:
    ImageUrlList _imageUrlList;//图片队列
    QSize _size;//记录当前窗口大小
    QFileInfo _info;
    int _maxType = 0;//记录动态分配标签的最大值
    int _nowType = 0;//记录当前打开
    int _backType = 0;//记录上次打开
    QString _nowpath;//记录当前路径
    QString _backpath;//记录上次路径
    Mat _nowMat;//记录当前打开
    Mat _backMat;//记录上次打开
    QPixmap _nowImage;
    Mat _changeImage(Mat mat);
    void _changeImageType(int num = 0);
    int _proportion = 0;//图片显示比例
    QSize _tmpSize;//按比例缩放后的图片大小
    QString _imageSize;//原图大小
    QString _colorSpace;//颜色域
    bool _isNavigationShow;//是否显示导航器
    QTimer *_playMovieTimer;//播放动图的计时器
    QList<Mat> *_matList;//存储动图的每一帧
    int _matListIndex = 0;//当前播放到哪一帧
    QPoint _startShowPoint;
    int _fps = 0;//帧率
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

#endif // BASE_H
