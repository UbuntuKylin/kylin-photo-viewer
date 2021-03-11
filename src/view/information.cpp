#include "information.h"

Information::Information(QWidget *parent) : QWidget(parent)
{
    this->resize(207,197);
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
    nameC->setWordWrap(true);
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
    nameC->setText(info.completeBaseName());
    formatC->setText(info.suffix());
    creationTimeC->setText(info.birthTime().toString("yyyy.MM.dd hh:mm"));
    revisionTimeC->setText(info.lastModified().toString("yyyy.MM.dd hh:mm"));

}

void Information::_setstyle()
{
    this->setStyleSheet("QWidget{background-color:rgba(255, 255, 255, 0.7);border-bottom-left-radius:4px;}");
}

void Information::_layout()
{
    gdLayout->addWidget(widName,0,0,1,1);
    gdLayout->addWidget(name,1,0,1,1);
    gdLayout->addWidget(nameC,1,3,2,4);
    gdLayout->addWidget(format,3,0,1,1);
    gdLayout->addWidget(formatC,3,3,1,4);
    gdLayout->addWidget(storageSize,4,0,1,1);
    gdLayout->addWidget(storageSizeC,4,3,1,4);
    gdLayout->addWidget(pixelSize,5,0,1,1);
    gdLayout->addWidget(pixelSizeC,5,3,1,4);
    gdLayout->addWidget(colorSpace,6,0,1,2);
    gdLayout->addWidget(colorSpaceC,6,3,1,4);
    gdLayout->addWidget(creationTime,7,0,1,2);
    gdLayout->addWidget(creationTimeC,7,3,1,4);
    gdLayout->addWidget(revisionTime,8,0,1,2);
    gdLayout->addWidget(revisionTimeC,8,3,1,4);

    gdLayout->setContentsMargins(14,10,10,18);
    inforWid->setLayout(gdLayout);
    inforWid->resize(this->width(),this->height());
    inforWid->move(0,0);
//    inforWid->show();

}
