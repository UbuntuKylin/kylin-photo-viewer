#ifndef PROCESSING_H
#define PROCESSING_H

#include "src/global/variable.h"
#include <QPixmap>

#include "flip.h"

using namespace cv;

class Processing
{
public:
    enum ProcessingType{flip};
    enum FlipWay{clockwise,counterclockwise,vertical,horizontal};
    static Mat processingImage(const ProcessingType &type, const Mat &mat, const QVariant &args);
    static QPixmap converFormat(const Mat &mat);
    static QPixmap resizePix(const QPixmap &pixmap ,const QSize &size);
    static void _pictureDeepen(QImage &image , const QSize &hightlightSize, const QPoint &point);//图片加深

private:
    static inline int minNumIsZero(const int &num1 ,const int &num2);
};

#endif // PROCESSING_H
