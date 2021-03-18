#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFileInfo>
#include <QVariant>
#include <QSvgRenderer>
#include <QSvgGenerator>
#include <QPainter>
#include <QMovie>
#include "global/variable.h"
#include "model/processing/processing.h"

using namespace cv;

class MatAndFileinfo
{
public:
    Mat mat;
    QList<Mat> *matList = nullptr;
    unsigned int fps =Variable::DEFAULT_MOVIE_TIME_INTERVAL;
    QFileInfo info;
};

class File : public QObject
{
    Q_OBJECT
public:
    static MatAndFileinfo loadImage(QString path, ImreadModes modes = IMREAD_UNCHANGED);
    static bool saveImage(const Mat &mat , const QString &savepath , bool replace = true);
    static void deleteImage(const QString &savepath);

private:
    static void processStart(const QString &cmd , QStringList arguments = QStringList());
    static bool save(const Mat &mat , const QString &savepath , const QString &type);
};

#endif // FILE_H
