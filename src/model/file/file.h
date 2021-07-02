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
#include "loadmovie.h"
#include <gif_lib.h>
#include "global/computingtime.h"

class MatAndFileinfo
{
public:
    Mat mat;
    QList<Mat> *matList = nullptr;
    int fps = 0;
    QFileInfo info;
    int maxFrame = 0;
};

class File : public QObject
{
    Q_OBJECT

signals:
    void saveMovieFinish(const QString &path);

public:
    static MatAndFileinfo loadImage(QString path, ImreadModes modes = IMREAD_UNCHANGED);
    bool saveImage(const Mat &mat , const QString &savepath , bool replace = true);//静态图
    bool saveImage(QList<Mat> *list ,const int &fps, const QString &savepath , bool replace = true);//动态图
    void deleteImage(const QString &savepath);
    bool isSaving(const QString &path);
    bool allSaveFinish();

private:
    static int getDelay(const QString &path,const QString &suffix);
    static int gifDelay(const QString &path);
    void processStart(const QString &cmd , QStringList arguments = QStringList());
    QString saveWay(const QString &savepath ,bool replace);
    bool save(const Mat &mat , const QString &savepath , const QString &type);
    bool save(QList<Mat> *list , const int &fps, const QString &savepath );
    bool saveMovie(QList<Mat> *list, const int &fps, const QString &savepath);
    void saveMovieFinishSlot(const QString &path);
    QStringList m_list;//用来保存正在保存的动图队列
    QProcess *m_process = nullptr;//操作文件
    void processLog();//process的日志
};

#endif // FILE_H
