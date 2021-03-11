#include "file.h"
#include <QDebug>

MatAndFileinfo File::loadImage(QString path , ImreadModes modes)
{
    MatAndFileinfo maf;
    /*		参数2：图像文件解析的方式，支持的方式有如下：
            cv::IMREAD_UNCHANGED    不对图像文件进行任何转换，直接读取【读入完整图片，包括alpha通道(png有，jpg无）】
                IMREAD_GRAYSCALE    将任何图像均转换为灰度图像（单通道）进行读取
                IMREAD_COLOR        将任何图像均转为RGB彩色图像（三通道）进行读取【默认参数，读入一副彩色图片，忽略alpha通道】
                IMREAD_ANYDEPTH     如果不设置这个参数，16/32位图像将会自动转为8位图像
                IMREAD_ANYCOLOR     将按照图像文件设定的颜色格式进行图像读取
                IMREAD_LOAD_GDAL    调用gdal库进行图像文件读取。（可以简单地理解为读取TIFF图像文件）
    */
    //读取图像
    Mat mat = imread(path.toLocal8Bit().data(), modes);
    //判断图像是否有效
    if (!mat.data) {
        qDebug() << "读取图片失败："<< path;
        return maf;
    }
    //获取文件信息
    QFileInfo info(path);
    maf.mat = mat;
    maf.info=info;
    return maf;
}

bool File::saveImage(const Mat &mat, const QString &savepath, bool replace)
{
    //覆盖保存
    if(replace)
        return imwrite(savepath.toStdString(),mat);

    //备份保存
    QFileInfo tmpFileInfo = QFileInfo(savepath);
    QString tmpPath = savepath;
    int num = 1;
    while (QFileInfo::exists(tmpPath)) {
        tmpPath = tmpFileInfo.absolutePath() + "/" +
                tmpFileInfo.completeBaseName()+
                "(" + QString::number(num) + ")" + "." +
                tmpFileInfo.completeSuffix();
        num++;
    }
    return imwrite(tmpPath.toStdString(),mat);
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
