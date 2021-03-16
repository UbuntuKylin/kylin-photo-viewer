#include "processing.h"

Mat Processing::processingImage(const ProcessingType &type,const Mat &mat, const QVariant &args)
{
    switch (type) {
    case flip:
        Flip f;
        return f.processing(mat,args);
    }

    return Mat();
}

QPixmap Processing::converFormat(const Mat &mat)
{
    switch ( mat.type() )
    {
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return QPixmap::fromImage(image);
    }

    case CV_8UC3:
    {
        QImage tmp(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
        QImage image =tmp.rgbSwapped();
        QPixmap::fromImage(image);
        return QPixmap::fromImage(image);
    }

    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;
        if ( sColorTable.isEmpty() )
        {
            for ( int i = 0; i < 256; ++i )
                sColorTable.push_back( qRgb( i, i, i ) );
        }
        QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );
        image.setColorTable( sColorTable );
        return QPixmap::fromImage(image);
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
    }
    return QPixmap();
}

QPixmap Processing::resizePix(const QPixmap &pixmap , const QSize &size)
{
    return pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
}

QImage Processing::_pictureDeepen(const QImage &img , const QSize &hightlightSize , const QPoint &point)
{
    QImage image = img.copy();
    int key = Variable::PICTURE_DEEPEN_KEY;
    int left = point.x();
    int right = point.x()+hightlightSize.width();
    int top = point.y();
    int bottom = point.y()+hightlightSize.height();
    //如果有透明通道
    bool hasAlpha = false;
    if(image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_ARGB32_Premultiplied)
        hasAlpha=true;

    for(int j = 0 ; j < image.height() ; ++j){
        for(int i = 0 ; i < image.width() ; ++i){
            if(i>=left && i<right && j>=top && j<bottom)continue;//高亮区域不处理
            QColor color(image.pixel(i, j));
            if(hasAlpha && color.red()+color.green()+color.blue() == 0){//透明区域40%透明度的黑色
                color.setAlphaF(0.4);
                image.setPixel(i, j, color.rgba());
                continue;
            }
            color.setRed(minNumIsZero(color.red(),key));
            color.setGreen(minNumIsZero(color.green(),key));
            color.setBlue(minNumIsZero(color.blue(),key));
            image.setPixel(i, j, color.rgb());
        }
    }
    return image;
}

int Processing::minNumIsZero(const int &num1 ,const int &num2)
{
    int num = num1-num2;
    if(num<0)
        return 0;
    return num;
}
