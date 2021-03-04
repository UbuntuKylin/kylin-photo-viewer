#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFileInfo>
#include <QVariant>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
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
    File();
    MatAndFileinfo loadImage(QString path);
    void saveImage(QVariant var);
};

#endif // FILE_H
