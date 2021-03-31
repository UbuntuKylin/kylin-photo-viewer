#include "savemovie.h"

SaveMovie::SaveMovie(QList<Mat> *list, const int &fps, const QString &savepath, const QString &type)
{
    //结束回收资源
    connect(this,&SaveMovie::finished,this,&SaveMovie::deleteLater);
    m_list = new QList<Mat>;
    for (Mat &mat : *list) {
        m_list->append(mat.clone());
    }
    m_fps = fps;
    qDebug()<<"m_fps"<<m_fps;
    m_savepath = savepath;
    m_type = type;
}

void SaveMovie::run()
{
    QFileInfo info(m_savepath);
    QString name = info.completeBaseName();
    QString tmpName = info.completeBaseName()+"_tmp";
    QString suffix = info.suffix().toLower();
    QString tmpDir = Variable::TEMP_PATH + name +"/";
    //新建目录
    QDir dir;
    dir.mkdir(tmpDir);
    int num = 0;
    //存储所有帧
    for (Mat &mat : *m_list) {
        num++;
        QString str = tmpDir+QString::number(num)+".png";
        imwrite(str.toStdString(),mat);
    }
    //析构临时队列
    for (Mat &mat : *m_list) {
        mat.release();
    }
    delete m_list;
    //构造命令
    QString tmpFilePath = tmpDir + tmpName + "." + "apng";
    QString cmd = "apngasm ";
    cmd += tmpFilePath;
    cmd +=  " " + tmpDir+"*.png ";
    cmd +=  QString::number(m_fps);
    cmd += " 1000 -z0";
    //执行命令
    system(cmd.toLocal8Bit().data());
    //等待完成
    while (!QFileInfo::exists(tmpFilePath)) {
        msleep(1);
    }
    //删除临时帧
    QString deleteTmpImages = "rm " + tmpDir+"*.png";
    system(deleteTmpImages.toLocal8Bit().data());
    //转码
    if (suffix == "gif") {
        QString cmd2 = "apng2gif ";
        cmd2 += " "+tmpFilePath;
        system(cmd2.toLocal8Bit().data());
        tmpFilePath.chop(4);
        tmpFilePath += suffix;
    }
    while (!QFileInfo::exists(tmpFilePath)) {
        msleep(1);
    }
    //移动回原目录
    QString cmd3 = "mv ";
    cmd3 += tmpFilePath;
    cmd3 += " "+m_savepath;
    system(cmd3.toLocal8Bit().data());
    qDebug()<<"动图保存完成";
}
