#include "file.h"
#include <QDebug>
File::File()
{

}

QVariant File::loadImage(QString path)
{
    QVariant var;
    /*		参数2：图像文件解析的方式，支持的方式有如下：
            cv::IMREAD_UNCHANGED    不对图像文件进行任何转换，直接读取【读入完整图片，包括alpha通道(png有，jpg无）】
                IMREAD_GRAYSCALE    将任何图像均转换为灰度图像（单通道）进行读取
                IMREAD_COLOR        将任何图像均转为RGB彩色图像（三通道）进行读取【默认参数，读入一副彩色图片，忽略alpha通道】
                IMREAD_ANYDEPTH     如果不设置这个参数，16/32位图像将会自动转为8位图像
                IMREAD_ANYCOLOR     将按照图像文件设定的颜色格式进行图像读取
                IMREAD_LOAD_GDAL    调用gdal库进行图像文件读取。（可以简单地理解为读取TIFF图像文件）
    */
    //读取图像
    Mat mat = imread(path.toLocal8Bit().data(), IMREAD_UNCHANGED);
    //判断图像是否有效
    if (!mat.data) {
        qDebug() << "读取图片失败："<< path;
        return QVariant();
    }
    //获取文件信息
    QFileInfo info(path);
    var.setValue(mat);
    var.setValue(info);
    return var;

//            QImage  img2(mat.data,mat.cols,mat.rows, mat.step, QImage::Format_RGB888);
//            ui->label->setPixmap(QPixmap::fromImage(img2));
}

void File::saveImage(QVariant var)
{

}
