#ifndef PROCESSING_H
#define PROCESSING_H

#include "src/global/variable.h"
#include <QPixmap>

using namespace cv;

class Processing
{
public:
    static Mat processingImage(const QString &cmd, const QVariant &arg ,const Mat &mat);
    static QPixmap converFormat(const Mat &mat);
    static QPixmap resizePix(const QPixmap &pixmap ,const QSize &size);
};

class _Processing
{
public:

};

#endif // PROCESSING_H
