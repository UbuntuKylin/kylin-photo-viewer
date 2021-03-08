#include "processing.h"

Mat Processing::processingImage(const QString &cmd, const QVariant &arg, const Mat &mat)
{

}

QPixmap Processing::converFormat(const Mat &mat)
{
    switch ( mat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
        return QPixmap::fromImage(image);
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage tmp(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
        QImage image =tmp.rgbSwapped();QPixmap::fromImage(image);
        return QPixmap::fromImage(image);
    }

    // 8-bit, 1 channel
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
