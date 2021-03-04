#include "openimage.h"

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

//    iconsize = new QSize();
    iconsize = QSize(30,30);
    this->setFixedSize(128,128+50);
    this->move(int((this->width()-this->width())/2),int((this->height()-this->height())/2));
    this->setstyle();
    this->initconnect();
}
void OpenImage::setstyle()
{
    qDebug()<<"open image file";
//        openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
//        addFile->setStyleSheet("background-image: url(:/res/res/addplus.png);");
        addFile->setIcon(QIcon(":/res/res/addplus.png"));
        addFile->setIconSize(QSize(30,30));
}
void OpenImage::initconnect()
{
    qDebug()<<"open image file";
    connect(openInCenter,&QPushButton::clicked,this,&OpenImage::openimage);\
    connect(addFile,&QPushButton::clicked,this,&OpenImage::openimage);
}
void OpenImage::openimage()
{
    qDebug()<<"open image file";
}
