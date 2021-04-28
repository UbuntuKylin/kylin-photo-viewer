#include "file.h"

MatAndFileinfo File::loadImage(QString path , ImreadModes modes)
{
    MatAndFileinfo maf;
    Mat mat;
    //获取文件信息
    QFileInfo info(path);
    maf.info=info;
    //svg、gif等opencv不支持的格式
    QString suffix = info.suffix().toLower();
    if (suffix == "gif" || suffix == "apng" || suffix == "png") {
        //如果是缩略图节则省资源
        if (modes == IMREAD_REDUCED_COLOR_8) {
            QImage img(path, "apng");
            mat = Mat(img.height(),img.width(),CV_8UC4,const_cast<uchar*>(img.bits()),static_cast<size_t>(img.bytesPerLine())).clone();
            if (!mat.data) {
                qDebug()<< "读取缩略图失败："<< path;
            }
            maf.mat=mat;
            return maf;
        }
        //获取帧率
        auto tmpMovie = new QMovie(path, "apng");
        if (tmpMovie->frameCount()>1) {
            maf.fps =getDelay(path,suffix);
        }
        QList<Mat> *frames = new QList<Mat>;  //存放gif的所有帧，每个frame都是Mat格式
        for (int i =0; i< tmpMovie->frameCount(); ++i) {
            tmpMovie->jumpToFrame(i);
            QImage image = tmpMovie->currentImage();
            frames->push_back(Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone());
        }
        tmpMovie->deleteLater();
        if (frames->length()>0) {
            maf.matList=frames;
            mat = frames->first();
        }
    } else if (suffix == "svg") {
        QSvgRenderer svgRender(path);
        QPixmap pix(svgRender.defaultSize());
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        svgRender.render(&p);
        QImage image = pix.toImage();
        mat = Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone();
    } else if (suffix == "tga") {
        int w,h,a;
        unsigned char *data =stbi_load(path.toLocal8Bit().data(),&w,&h,&a,0);
        if(!data) return maf;
        Mat tmpMat = Mat(h,w,CV_8UC4,const_cast<uchar*>(data)).clone();
        stbi_image_free(data);
        cvtColor(tmpMat,mat,cv::COLOR_BGRA2RGBA);
    } else if (suffix == "ico") {
        QImage image(path);
        mat = Mat(image.height(),image.width(),CV_8UC4,const_cast<uchar*>(image.bits()),static_cast<size_t>(image.bytesPerLine())).clone();
    } else if (suffix == "jp2") {

    }
    //    if (suffix == "hdr") {
    //        int w,h,a;
    //        unsigned char *data =stbi_load(path.toLocal8Bit().data(),&w,&h,&a,0);
    //        Mat tmpMat = Mat(h,w,CV_8UC3,const_cast<uchar*>(data)).clone();
    //        stbi_image_free(data);
    //        cvtColor(tmpMat,mat,cv::COLOR_BGRA2RGBA);
    //    }
    //其他情况下尝试正常读取图像
    if (!mat.data) {
        mat = imread(path.toLocal8Bit().data(), modes);
        //通用加载方法的缩略图尺寸优化
         if (mat.cols < Variable::ALBUM_IMAGE_SIZE.width() && mat.rows < Variable::ALBUM_IMAGE_SIZE.height()) {
             mat = imread(path.toLocal8Bit().data());
         }
    }
    //判断图像是否有效
    if (!mat.data) {
        qDebug() << "读取图片失败："<< path;
        return maf;
    }
    maf.mat = mat;
    return maf;
}

int File::gifDelay(const QString &path)
{
    int j, frame_delay;
    SavedImage *frame;//这个是系统中保存gif的扩展块的信息，不太了解gif的文件结构的可以看一下：http://blog.csdn.net/wzy198852/article/details/17266507
    ExtensionBlock *ext = nullptr;//这个保存gif延时代码块的结构体

    int err = 0;
    GifFileType *gifFileType = DGifOpenFileName(path.toLocal8Bit().data(),&err);
    if (err != 0) {
        DGifCloseFile(gifFileType,&err);
        return 0;
    }
    DGifSlurp(gifFileType);
    frame = &gifFileType->SavedImages[0];//拿到第一个图片相关信息
    for (j = 0;j<frame->ExtensionBlockCount; j++) {
        //找到含有延迟时间的代码块
        if(frame->ExtensionBlocks[j].Function==GRAPHICS_EXT_FUNC_CODE){
            ext = &(frame->ExtensionBlocks[j]);
        }
        //拿到延迟时间
        if (ext) {
            //延迟时间1-->10ms
            frame_delay = 10*(ext->Bytes[2]<<8 | ext->Bytes[1]);//拿到延迟时间
            break;
        }
    }
    DGifCloseFile(gifFileType,&err);
    return frame_delay;
}

int File::getDelay(const QString &path, const QString &suffix)
{
    int fps = 0;

    if (suffix == "gif") {
        fps = gifDelay(path);
    }

    //    else if (suffix == "apng" || suffix == "png") {
    //    qDebug()<<"===此处待补充===";
    //}

    if (fps != 0) {
        return fps;
    }

    //以上都没获取到fps的保底机制
    //QMovie没有现成的方法，为求稳定，每次打开新动图的时候循环5次取最大值，此处待优化
    QMovie tmpMovie(path);
    if (fps == 0) {
        for (int i=0;i<5;i++) {
            tmpMovie.start();
            tmpMovie.stop();
            fps=tmpMovie.nextFrameDelay()>fps?tmpMovie.nextFrameDelay():fps;
        }
    }

    return fps == 0 ? Variable::DEFAULT_MOVIE_TIME_INTERVAL : fps;
}

QString File::saveWay(const QString &savepath ,bool replace)
{
    QFileInfo tmpFileInfo(savepath);
    //覆盖保存
    if (replace) {
        return savepath;
    }

    //备份保存
    QString tmpPath = savepath;
    int num = 1;
    while (QFileInfo::exists(tmpPath)) {
        tmpPath = tmpFileInfo.absolutePath() + "/" + tmpFileInfo.completeBaseName()+  "(" + QString::number(num) + ")" + "." +tmpFileInfo.completeSuffix();
        num++;
    }
    return tmpPath;
}

bool File::saveImage(const Mat &mat, const QString &savepath, bool replace)
{
    return save(mat,saveWay(savepath,replace),QFileInfo(savepath).suffix());
}

bool File::saveImage(QList<Mat> *list, const int &fps, const QString &savepath, bool replace)
{
    return save(list,fps,saveWay(savepath,replace));
}

bool File::save(const Mat &mat, const QString &savepath, const QString &type)
{
    if (type == "svg") {
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
    if (type == "tga") {
        Mat tmpMat;
        cvtColor(mat,tmpMat,cv::COLOR_RGBA2BGRA);
        return stbi_write_tga(savepath.toLocal8Bit().data(),tmpMat.cols,tmpMat.rows,4,tmpMat.data);
    }
    if (type == "ico") {
        QPixmap pix = Processing::converFormat(mat);
        return pix.save(savepath);
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
    if (type == "gif") {
        QList<Mat> *list = new QList<Mat>;
        list->append(mat);
        return save(list,0,savepath);
    }
    //非特殊情况
    return imwrite(savepath.toStdString(),mat);
}

bool File::save(QList<Mat> *list, const int &fps, const QString &savepath)
{
    if (list->length()<1) {
        return false;
    }

    return saveMovie(list,fps,savepath);
}

bool File::saveMovie(QList<Mat> *list,const int &fps, const QString &savepath)
{
    m_list.append(savepath);
    SaveMovie *saveMovie = new SaveMovie(list,fps,savepath);
    connect(saveMovie,&SaveMovie::saveMovieFinish,this,&File::saveMovieFinishSlot);
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

void File::saveMovieFinishSlot(const QString &path)
{
    m_list.removeOne(path);
    emit saveMovieFinish(path);
}

void File::deleteImage(const QString &savepath)
{
    processStart("gio",QStringList() << "trash" << savepath);
}

bool File::isSaving(const QString &path)
{
    return m_list.contains(path);
}

bool File::allSaveFinish()
{
    return m_list.isEmpty();
}

void File::processStart(const QString &cmd, QStringList arguments)
{
    if (!m_process) {
        //File构造函数是在主线程调用的，m_process如果在构造函数中实例化，会报错
        m_process = new QProcess;//操作文件
        connect(m_process,&QProcess::readyReadStandardError,this,&File::processLog);
    }

    m_process->start(cmd,arguments);
    m_process->waitForStarted();
    m_process->waitForFinished();
}

void File::processLog()
{
    qDebug()<<"*******process error*******\n"
           << QString::fromLocal8Bit(m_process->readAllStandardError())
           <<"\n*******process error*******";
}

