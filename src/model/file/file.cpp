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
    QString suffix = info.suffix().toLower();
    if(suffix == "gif" || suffix == "apng" || suffix == "png"){
        //如果是缩略图节则省资源
        if(modes == IMREAD_REDUCED_COLOR_8){
            QImage img(path, "apng");
            mat = Mat(img.height(),img.width(),CV_8UC4,const_cast<uchar*>(img.bits()),static_cast<size_t>(img.bytesPerLine())).clone();
            if(!mat.data) qDebug()<< "读取缩略图失败："<< path;
            return maf;
        }
        //获取帧率
        maf.fps =_getDelay(path,suffix);
        auto tmpMovie = new QMovie(path, "apng");
//        if(tmpMovie->frameCount()>1)
//            maf.fps =_getDelay(path,suffix);
        QList<Mat> *frames = new QList<Mat>;  //存放gif的所有帧，每个frame都是Mat格式
        for (int i =0; i< tmpMovie->frameCount(); ++i) {
            tmpMovie->jumpToFrame(i);
            QImage image = tmpMovie->currentImage();
            frames->push_back(Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone());
        }
        tmpMovie->deleteLater();
        if(frames->length()>0){
            maf.matList=frames;
            mat = frames->first();
        }
    }
    if(suffix == "svg"){
        QSvgRenderer svgRender(path);
        QPixmap pix(svgRender.defaultSize());
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        svgRender.render(&p);
        QImage image = pix.toImage();
        mat = Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone();
    }
    if(suffix == "tga"){
        int w,h,a;
        unsigned char *data =stbi_load(path.toLocal8Bit().data(),&w,&h,&a,0);
        if(!data) return maf;
        Mat tmpMat = Mat(h,w,CV_8UC4,const_cast<uchar*>(data)).clone();
        stbi_image_free(data);
        cvtColor(tmpMat,mat,cv::COLOR_BGRA2RGBA);
    }
//    if(suffix == "hdr"){
//        int w,h,a;
//        unsigned char *data =stbi_load(path.toLocal8Bit().data(),&w,&h,&a,0);
//        Mat tmpMat = Mat(h,w,CV_8UC3,const_cast<uchar*>(data)).clone();
//        stbi_image_free(data);
//        cvtColor(tmpMat,mat,cv::COLOR_BGRA2RGBA);
//    }
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

int File::_getDelay(const QString &path, const QString &suffix)
{
    int fps = 0;

    if(suffix == "gif"){
//        int err = 0;
//        GifFileType *gif = DGifOpenFileName(path.toLocal8Bit().data(),&err);
//        qDebug()<<"========="<<gif->SWidth;
//        GraphicsControlBlock gcb;
//        err = DGifSavedExtensionToGCB(gif,0,&gcb);
//        err = EGifGCBToSavedExtension(gcb,gif,0);
//        if(err == 0) qDebug()<<"*******"<<gcb.TransparentColor;
    }

    if(fps != 0)
        return fps;

    //QMovie没有现成的方法，为求稳定，每次打开新动图的时候循环5次取最大值，此处待优化
    QMovie tmpMovie(path);
    if(fps == 0)
        for(int i=0;i<5;i++){
            tmpMovie.start();
            tmpMovie.stop();
            fps=tmpMovie.nextFrameDelay()>fps?tmpMovie.nextFrameDelay():fps;
        }
    //qDebug()<<"========"<<fps;

    return fps == 0 ? Variable::DEFAULT_MOVIE_TIME_INTERVAL : fps;
}

bool File::saveImage(const Mat &mat, const QString &savepath, bool replace)
{
    QFileInfo tmpFileInfo(savepath);
    //覆盖保存
    if(replace)
        return _save(mat,savepath,tmpFileInfo.suffix());

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
    return _save(mat,savepath,tmpFileInfo.suffix());
}

bool File::saveImage(QList<Mat> *list, const int &fps, const QString &savepath, bool replace)
{
    QFileInfo tmpFileInfo(savepath);
    //覆盖保存
    if(replace)
        return _save(list,fps,savepath,tmpFileInfo.suffix());

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
    return _save(list,fps,savepath,tmpFileInfo.suffix());
}

bool File::_save(const Mat &mat, const QString &savepath, const QString &type)
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
    if(type == "tga"){
        Mat tmpMat;
        cvtColor(mat,tmpMat,cv::COLOR_RGBA2BGRA);
        stbi_write_tga(savepath.toLocal8Bit().data(),tmpMat.cols,tmpMat.rows,4,tmpMat.data);
        return true;
    }
//    if(type == "hdr"){
//        Mat tmpMat;
//        cvtColor(mat,tmpMat,cv::COLOR_RGB2BGR);
//        auto img_array = tmpMat.data;
//        float *img_pt = new float[tmpMat.rows*tmpMat.cols];
//        for (int i = 0; i < tmpMat.rows*tmpMat.cols; i++)
//            img_pt[i] = (float)img_array[i];
//        stbi_write_hdr(savepath.toLocal8Bit().data(),tmpMat.cols,tmpMat.rows,3,img_pt);
//        return true;
//    }
    //单帧gif的场景
    if(type == "gif"){
        QList<Mat> *list = new QList<Mat>;
        list->append(mat);
        return _save(list,0,savepath,type,true);
    }
    //非特殊情况
    return imwrite(savepath.toStdString(),mat);
}

bool File::_save(QList<Mat> *list, const int &fps, const QString &savepath, const QString &type, bool special)
{
    if(list->length()<1)
        return false;

    return _saveMovie(list,fps,savepath,type,special);
}

bool File::_saveMovie(QList<Mat> *list,const int &fps, const QString &savepath, const QString &type ,bool special)
{

    SaveMovie *saveMovie = new SaveMovie(list,fps,savepath,type,special);
    saveMovie->start();

//    Gif_H m_Gif;
//    Gif_H::GifWriter m_GifWriter;
//    for (int i=0; i<list->length(); ++i){
//        Mat tmpMat;
//        cvtColor(list->at(i),tmpMat,cv::COLOR_RGBA2BGRA);
//        if(i == 0)
//            if (!m_Gif.GifBegin(&m_GifWriter, savepath.toLocal8Bit().data(), tmpMat.cols, tmpMat.rows, fps))
//                return false;
//        // 写入图片信息
//        m_Gif.GifWriteFrame(&m_GifWriter, tmpMat.data, tmpMat.cols, tmpMat.rows, fps);
//    }
//    m_Gif.GifEnd(&m_GifWriter);
    return true;
}

void File::deleteImage(const QString &savepath)
{
    _processStart("gio",QStringList() << "trash" << savepath);
}

void File::_processStart(const QString &cmd, QStringList arguments)
{
    QString cmdTmp = cmd;
    for(QString &x : arguments){
        cmdTmp += " ";
        cmdTmp += x;
    }
    system(cmdTmp.toLocal8Bit().data());
}
