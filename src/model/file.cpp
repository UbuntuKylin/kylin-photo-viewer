#include "file.h"
#include <QDebug>

MatAndFileinfo File::loadImage(QString path , ImreadModes modes)
{
    MatAndFileinfo maf;
    Mat mat;
    /*		参数2：图像文件解析的方式，支持的方式有如下：
            cv::IMREAD_UNCHANGED    不对图像文件进行任何转换，直接读取【读入完整图片，包括alpha通道(png有，jpg无）】
                IMREAD_GRAYSCALE    将任何图像均转换为灰度图像（单通道）进行读取
                IMREAD_COLOR        将任何图像均转为RGB彩色图像（三通道）进行读取【默认参数，读入一副彩色图片，忽略alpha通道】
                IMREAD_ANYDEPTH     如果不设置这个参数，16/32位图像将会自动转为8位图像
                IMREAD_ANYCOLOR     将按照图像文件设定的颜色格式进行图像读取
                IMREAD_LOAD_GDAL    调用gdal库进行图像文件读取。（可以简单地理解为读取TIFF图像文件）
    */

    //获取文件信息
    QFileInfo info(path);
    maf.info=info;

    //svg、gif等opencv不支持的格式
//    if(info.suffix().toLower() == "gif"){
//        QSvgRenderer svgRender(path);
//        QPixmap pix(svgRender.defaultSize());
//        pix.fill(Qt::transparent);
//        QPainter p(&pix);
//        svgRender.render(&p);
//        QImage image = pix.toImage();
//        mat = Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone();
//    }

    if(info.suffix().toLower() == "svg"){
        QSvgRenderer svgRender(path);
        QPixmap pix(svgRender.defaultSize());
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        svgRender.render(&p);
        QImage image = pix.toImage();
        mat = Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone();
    }

    //其他情况下尝试正常读取图像
    if(!mat.data)
        mat = imread(path.toLocal8Bit().data(), modes);
    //判断图像是否有效
    if (!mat.data) {
        qDebug() << "读取图片失败："<< path;
        return maf;
    }
    maf.mat = mat;
    return maf;
}

bool File::saveImage(const Mat &mat, const QString &savepath, bool replace)
{
    QFileInfo tmpFileInfo(savepath);
    //覆盖保存
    if(replace)
        return save(mat,savepath,tmpFileInfo.suffix());

    //备份保存
    QString tmpPath = savepath;
    int num = 1;
    while (QFileInfo::exists(tmpPath)) {
        tmpPath = tmpFileInfo.absolutePath() + "/" +
                tmpFileInfo.completeBaseName()+
                "(" + QString::number(num) + ")" + "." +
                tmpFileInfo.completeSuffix();
        num++;
    }
    return save(mat,savepath,tmpFileInfo.suffix());
}

bool File::save(const Mat &mat, const QString &savepath, const QString &type)
{
    if(type == "svg"){
        QPixmap pix = Processing::converFormat(mat);
        QPainter paint;
        QSvgGenerator svg;
        svg.setFileName(savepath);
        svg.setViewBox(QRect(QPoint(0,0),pix.size()));
        paint.begin(&svg);
        paint.drawImage(QPoint(0,0),pix.toImage());
        paint.end();
        return true;
    }
    //非特殊情况
    return imwrite(savepath.toStdString(),mat);
}

void File::deleteImage(const QString &savepath)
{
    processStart("gio",QStringList() << "trash" << savepath);
}

void File::processStart(const QString &cmd, QStringList arguments)
{
    QString cmdTmp = cmd;
    for(QString &x : arguments){
        cmdTmp += " ";
        cmdTmp += x;
    }
    system(cmdTmp.toLocal8Bit().data());
}
