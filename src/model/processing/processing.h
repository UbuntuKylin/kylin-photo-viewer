#ifndef PROCESSING_H
#define PROCESSING_H

#include "global/variable.h"
#include <QPixmap>

#include "flip.h"

using namespace cv;

class Processing : public ProcessingBase
{
public:
    enum ProcessingType{flip};
    static Mat processingImage(const ProcessingType &type, const Mat &mat, const QVariant &args);
    static QPixmap converFormat(const Mat &mat);
    static QPixmap resizePix(const QPixmap &pixmap ,const QSize &size);
    static QImage pictureDeepen(const QImage &img , const QSize &hightlightSize, const QPoint &point);//图片加深

private:
    static inline int minNumIsZero(const int &num1 ,const int &num2);
};

#endif // PROCESSING_H
