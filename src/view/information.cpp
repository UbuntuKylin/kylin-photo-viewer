#include "information.h"

Information::Information(QWidget *parent) : QWidget(parent)
{
    this->resize(207,197 + 18);
    this->adjustSize();
//    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    widName = new QLabel(this);
    widName->setAttribute(Qt::WA_TranslucentBackground);
    widName->setText(tr("Information"));
//    widName->setText(tr("信息"));
    name = new QLabel(this);
    name->setAttribute(Qt::WA_TranslucentBackground);
    name->setText(tr("Name"));
//    name->setText(tr("名称"));
    format = new QLabel(this);
    format->setAttribute(Qt::WA_TranslucentBackground);
    format->setText(tr("Format"));
//    format->setText(tr("格式"));
    storageSize = new QLabel(this);
    storageSize->setAttribute(Qt::WA_TranslucentBackground);
    storageSize->setText(tr("Storage size"));
//    storageSize->setText(tr("大小"));
    pixelSize = new QLabel(this);
    pixelSize->setAttribute(Qt::WA_TranslucentBackground);
    pixelSize->setText(tr("Pixel Size"));
//    pixelSize->setText(tr("尺寸"));
    colorSpace = new QLabel(this);
    colorSpace->setAttribute(Qt::WA_TranslucentBackground);
    colorSpace->setText(tr("Color Space"));
//    colorSpace->setText(tr("颜色空间"));
    creationTime = new QLabel(this);
    creationTime->setAttribute(Qt::WA_TranslucentBackground);
    creationTime->setText(tr("Create Time"));
//    creationTime->setText(tr("创建时间"));
    revisionTime = new QLabel(this);
    revisionTime->setAttribute(Qt::WA_TranslucentBackground);
    revisionTime->setText(tr("Revise Time"));
//    revisionTime->setText(tr("修改时间"));

    nameC = new QLabel(this);
    nameC->setAttribute(Qt::WA_TranslucentBackground);
    formatC = new QLabel(this);
    formatC->setAttribute(Qt::WA_TranslucentBackground);
    storageSizeC = new QLabel(this);
    storageSizeC->setAttribute(Qt::WA_TranslucentBackground);
    pixelSizeC = new QLabel(this);
    pixelSizeC->setAttribute(Qt::WA_TranslucentBackground);
    colorSpaceC = new QLabel(this);
    colorSpaceC->setAttribute(Qt::WA_TranslucentBackground);
    creationTimeC = new QLabel(this);
    creationTimeC->setAttribute(Qt::WA_TranslucentBackground);
    revisionTimeC = new QLabel(this);
    revisionTimeC->setAttribute(Qt::WA_TranslucentBackground);

    inforWid = new QWidget(this);
    gdLayout =new QGridLayout(this);
    this->_layout();

}

void Information::contentText(QFileInfo info, QString sizeImage, QString spaceColor)
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
    _longText(nameC,info.completeBaseName());
    formatC->setText(info.suffix().toUpper());
    storageSizeC->setText(imageSize);
    pixelSizeC->setText(sizeImage);
    colorSpaceC->setText(spaceColor);
    creationTimeC->setText(info.birthTime().toString("yyyy.MM.dd hh:mm"));
    revisionTimeC->setText(info.lastModified().toString("yyyy.MM.dd hh:mm"));

}

void Information::_setstyle()
{

}

void Information::_layout()
{
    gdLayout->addWidget(widName,0,0,1,2);
    gdLayout->addWidget(name,1,0,1,1);
    gdLayout->addWidget(nameC,1,3,1,4);
    gdLayout->addWidget(format,2,0,1,1);
    gdLayout->addWidget(formatC,2,3,1,4);
//    label_9->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    gdLayout->addWidget(storageSize,3,0,1,1);
    gdLayout->addWidget(storageSizeC,3,3,1,4);
    gdLayout->addWidget(pixelSize,4,0,1,1);
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

void Information::_longText(QLabel *nameC, QString text)
{
    QFontMetrics fontWidth(nameC->font());//得到每个字符的宽度
    int wid = fontWidth.width(revisionTimeC->text());
    QString elideNote = fontWidth.elidedText(text, Qt::ElideRight, wid);//最大宽度150像素
    nameC->setText(elideNote);//显示省略好的字符串
    if(elideNote.contains("…",Qt::CaseInsensitive))
    {
         nameC->setToolTip(text);//设置tooltips
    }
}
