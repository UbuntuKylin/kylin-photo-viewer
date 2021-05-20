#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QPixmap>
#include <QColor>
#include <QTimer>
#include <QGSettings>
#include <QStandardItemModel>
#include "model/dbus.h"
#include "model/file/file.h"
#include "model/processing/processing.h"
#include "albumthumbnail.h"

class Enums{
public:
    enum ChamgeImageType{NEXT_IMAGE=-1,BACK_IMAGE=-2,ERROR_IMAGE=-9,ALBUM_IMAGE=-3};
    enum ChangeShowSizeType{BIG = 0,SMALL,ORIGIN,AUTO};//放大、缩小、原图、自适应
};

class ImageShowStatus : public Enums //显示图片细节相关，记录显示图片状态
{
public:
    QSize m_size;//记录当前窗口大小
    QFileInfo m_info;
    QString m_nowpath;//记录当前路径
    QString m_backpath;//记录上次路径
    Mat m_nowMat;//记录当前打开
    Mat m_backMat;//记录上次打开
    QPixmap m_nowImage;
    int m_proportion = 0;//图片显示比例
    QSize m_tmpSize;//按比例缩放后的图片大小
    QString m_imageSize;//原图大小
    QString m_colorSpace;//颜色域
    bool m_isNavigationShow;//是否显示导航器
    QTimer *m_playMovieTimer;//播放动图的计时器
    QList<Mat> *m_matList;//存储动图的每一帧
    int m_matListIndex = 0;//当前播放到哪一帧
    QPoint m_startShowPoint;
    int m_fps = 0;//帧率
    bool m_processed = false;//此图被操作过
    bool m_thisImageIsSaving = false;//此图片正在储存
    QStandardItemModel *m_albumModel = nullptr;//相册model
    QPixmap m_defultPixmap = QPixmap(":/res/res/kyview_logo.png");
};

class NavigationStatus : public ImageShowStatus//导航器相关单独写，提高可读性
{
public:
    QPoint m_clickBeforePosition;//记录上次点击区域，用于提升体验
    QPoint m_clickBeforeStartPosition;//记录上次点击区域，用于节省算力
    QPixmap m_showPix;//待显示图
    QImage m_navigationImage;//导航栏背景
    QSize m_hightlightSize; //高亮区域大小;
    int m_spaceWidth = 0;//导航栏窗口与缩略图左边缘距离
    int m_spaceHeight = 0;//导航栏窗口与缩略图上边缘距离
};

#endif // BASE_H
