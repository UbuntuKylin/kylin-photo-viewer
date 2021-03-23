#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFileInfo>
#include <QVariant>
#include <QSvgRenderer>
#include <QSvgGenerator>
#include <QPainter>
#include <QMovie>
#include <stb/stb_image_write.h>
#include <stb/stb_image.h>
#include "model/processing/processing.h"
#include "savemovie.h"
//#include <gif_lib.h>

struct ImageAndInfo
{
    QFileInfo info; //信息
    QPixmap image; //图片
    int type; //标签
    int proportion; //比例
    QString colorSpace;//图片空间
    QString imageSize;//图片尺寸
};
Q_DECLARE_METATYPE(ImageAndInfo)//QVarant 注册自定义类型

class MatAndFileinfo
{
public:
    Mat mat;
    QList<Mat> *matList = nullptr;
    int fps = 0;
    QFileInfo info;
};

class File : public QObject
{
    Q_OBJECT
public:
    static MatAndFileinfo loadImage(QString path, ImreadModes modes = IMREAD_UNCHANGED);
    static bool saveImage(const Mat &mat , const QString &savepath , bool replace = true);
    static bool saveImage(QList<Mat> *list ,const int &fps, const QString &savepath , bool replace = true);
    static void deleteImage(const QString &savepath);

private:
    static void _processStart(const QString &cmd , QStringList arguments = QStringList());
    static bool _save(const Mat &mat , const QString &savepath , const QString &type);
    static bool _save(QList<Mat> *list ,const int &fps, const QString &savepath , const QString &type, bool special = false);
    static bool _saveMovie(QList<Mat> *list, const int &fps, const QString &savepath, const QString &type , bool special = false);
    static int _getDelay(const QString &path,const QString &suffix);
};

#endif // FILE_H
