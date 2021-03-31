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
#include <gif_lib.h>

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

signals:
    void saveMovieFinish(const QString &path);

public:
    static MatAndFileinfo loadImage(QString path, ImreadModes modes = IMREAD_UNCHANGED);
    bool saveImage(const Mat &mat , const QString &savepath , bool replace = true);
    bool saveImage(QList<Mat> *list ,const int &fps, const QString &savepath , bool replace = true);
    static void deleteImage(const QString &savepath);
    bool isSaving(const QString &path);

private:
    static int getDelay(const QString &path,const QString &suffix);
    static int gifDelay(const QString &path);
    static void processStart(const QString &cmd , QStringList arguments = QStringList());
    QString saveWay(const QString &savepath ,bool replace);
    bool save(const Mat &mat , const QString &savepath , const QString &type);
    bool save(QList<Mat> *list ,const int &fps, const QString &savepath , const QString &type);
    bool saveMovie(QList<Mat> *list, const int &fps, const QString &savepath, const QString &type );
    void saveMovieFinishSlot(const QString &path);
    QStringList m_list;//用来保存正在保存的动图队列
};

#endif // FILE_H
