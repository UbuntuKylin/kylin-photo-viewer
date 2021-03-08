#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFileInfo>
#include <QVariant>
#include "src/global/variable.h"

using namespace cv;

class MatAndFileinfo
{
public:
    Mat mat;
    QFileInfo info;
};

class File : public QObject
{
    Q_OBJECT
public:
    static MatAndFileinfo loadImage(QString path, ImreadModes modes = IMREAD_UNCHANGED);
    void saveImage(QVariant var);
};

#endif // FILE_H
