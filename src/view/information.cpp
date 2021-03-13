#include "information.h"

Information::Information(QWidget *parent) : QWidget(parent)
{
    this->resize(207,197 + 18);
    this->adjustSize();
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    widName = new QLabel(this);
//    widName->setText(tr("Information"));
    widName->setText(tr("信息"));
    name = new QLabel(this);
//    name->setText(tr("Name"));
    name->setText(tr("名称"));
    format = new QLabel(this);
//    format->setText(tr("Format"));
    format->setText(tr("格式"));
    storageSize = new QLabel(this);
//    storageSize->setText(tr("Storage size"));
    storageSize->setText(tr("大小"));
    pixelSize = new QLabel(this);
//    pixelSize->setText(tr("Pixel Size"));
    pixelSize->setText(tr("尺寸"));
    colorSpace = new QLabel(this);
//    colorSpace->setText(tr("Color Space"));
    colorSpace->setText(tr("颜色空间"));
    creationTime = new QLabel(this);
//    creationTime->setText(tr("Create Time"));
    creationTime->setText(tr("创建时间"));
    revisionTime = new QLabel(this);
//    revisionTime->setText(tr("Revise Time"));
    revisionTime->setText(tr("修改时间"));

    nameC = new QLabel(this);
    formatC = new QLabel(this);
    storageSizeC = new QLabel(this);
    pixelSizeC = new QLabel(this);
    colorSpaceC = new QLabel(this);
    creationTimeC = new QLabel(this);
    revisionTimeC = new QLabel(this);

    inforWid = new QWidget(this);
    gdLayout =new QGridLayout(this);
    this->_layout();

}

void Information::contentText(QFileInfo info)
{
    QString imageSize;
    if (float(info.size())/1024 >= 1024){
        QString Size = QString("%1").arg(QString::asprintf("%.1f", float(info.size())/1024/1024));
        imageSize = Size + "Mib";
    }else{
        QString Size = QString("%1").arg(QString::asprintf("%.1f", float(info.size())/1024));
        imageSize = Size + "Kib";
    }
    if(info.completeBaseName().length() > info.birthTime().toString("yyyy.MM.dd hh:mm").length()){

    }
//    _longText(nameC,);
    nameC->setText(info.completeBaseName());
    formatC->setText(info.suffix());
    storageSizeC->setText(imageSize);
    creationTimeC->setText(info.birthTime().toString("yyyy.MM.dd hh:mm"));
    revisionTimeC->setText(info.lastModified().toString("yyyy.MM.dd hh:mm"));

}

void Information::_setstyle()
{
//    this->setStyleSheet("QWidget{background-color:rgba(255, 255, 255, 0.7);border-bottom-left-radius:4px;}");
}

void Information::_layout()
{
    gdLayout->addWidget(widName,0,0,1,2);
    gdLayout->addWidget(name,1,0,1,2);
    gdLayout->addWidget(nameC,1,3,2,4);
    gdLayout->addWidget(format,2,0,1,2);
    gdLayout->addWidget(formatC,2,3,1,4);
    gdLayout->addWidget(storageSize,3,0,1,2);
    gdLayout->addWidget(storageSizeC,3,3,1,4);
    gdLayout->addWidget(pixelSize,4,0,1,2);
    gdLayout->addWidget(pixelSizeC,4,3,1,4);
    gdLayout->addWidget(colorSpace,5,0,1,2);
    gdLayout->addWidget(colorSpaceC,5,3,1,4);
    gdLayout->addWidget(creationTime,6,0,1,2);
    gdLayout->addWidget(creationTimeC,6,3,1,4);
    gdLayout->addWidget(revisionTime,7,0,1,2);
    gdLayout->addWidget(revisionTimeC,7,3,1,4);

    gdLayout->setContentsMargins(12,10,10,16);
    inforWid->setLayout(gdLayout);
    inforWid->resize(this->width(),this->height());
    inforWid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}
