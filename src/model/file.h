#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFileInfo>
#include <QVariant>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
using namespace cv;

class File : public QObject
{
    Q_OBJECT
public:
    File();
    QVariant loadImage(QString path);
    void saveImage(QVariant var);
};
Q_DECLARE_METATYPE(Mat)//声明类型，使其可以放入QVariant

#endif // FILE_H
