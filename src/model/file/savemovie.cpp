#include "savemovie.h"

SaveMovie::SaveMovie(QList<Mat> *list, const int &fps, const QString &savepath)
{
    //结束回收资源
    connect(this,&SaveMovie::finished,this,&SaveMovie::deleteLater);
    m_list = new QList<Mat>;
    for (Mat &mat : *list) {
        m_list->append(mat.clone());
    }
    m_fps = fps;
    m_savepath = savepath;
}

void SaveMovie::run()
{
    m_process =new QProcess;
    connect(m_process,&QProcess::readyReadStandardError,this,&SaveMovie::processLog);

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
    m_process->start(cmd);
    m_process->waitForStarted();
    m_process->waitForFinished();
    //转码
    if (suffix == "gif") {
        QString cmd2 = "apng2gif ";
        cmd2 += " "+tmpFilePath;
        m_process->start(cmd2);
        m_process->waitForStarted();
        m_process->waitForFinished();
        tmpFilePath.chop(4);
        tmpFilePath += suffix;
    }
    //移动回原目 录
    QString cmd3 = "mv ";
    cmd3 += tmpFilePath;
    cmd3 += " "+m_savepath;
    m_process->start(cmd3);
    m_process->waitForStarted();
    m_process->waitForFinished();
    //删除临时文件
    QString deleteTmpImages = "rm -rf "+tmpDir;
    m_process->start(deleteTmpImages);
    m_process->waitForStarted();
    m_process->waitForFinished();
    qDebug()<<"动图保存完成";
    m_process->deleteLater();
    emit saveMovieFinish(m_savepath);
}

void SaveMovie::processLog()
{
    qDebug()<<"*******process error*******\n"
           << QString::fromLocal8Bit(m_process->readAllStandardError())
           <<"\n*******process error*******";
}
