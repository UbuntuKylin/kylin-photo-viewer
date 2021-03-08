#include "openimage.h"
#include "kyview.h"

OpenImage::OpenImage(QWidget *parent) : QWidget(parent)
{
    openInCenter = new QPushButton(this);
    addFile = new QPushButton(openInCenter);
    openText = new QLabel(this);
    openInCenter->setFixedSize(128,128);
    openInCenter->move(0,0);
    addFile->setFixedSize(40,40);
    addFile->move(openInCenter->geometry().center()-addFile->rect().center());
    openText->setText(tr("载入图片或文件夹"));
    openText->move(8,128+30);

    iconsize = QSize(30,30);
    this->setFixedSize(128,128+50);
    this->setstyle();
    this->initconnect();

//    _initInteraction();//一定要放到构造函数末尾
}

//设置样式
void OpenImage::setstyle()
{
        openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
//        addFile->setStyleSheet("background-image: url(:/res/res/addplus.png);");
        addFile->setIcon(QIcon(":/res/res/addplus.png"));
        addFile->setIconSize(QSize(30,30));
}
//初始化连接
void OpenImage::initconnect()
{
    connect(openInCenter,&QPushButton::clicked,this,&OpenImage::openimage);
    connect(addFile,&QPushButton::clicked,this,&OpenImage::openimage);

}
//打开图片
void OpenImage::openimage()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    //打开文件夹中的图片文件
    QString file_path = QFileDialog::getOpenFileName(this,"打开图片",defaultPath,"Image Files(*.jpg *.png *.bmp *.jpeg *.gif)");
    qDebug() << "file_path " << file_path;
    emit openImage(file_path);

}
