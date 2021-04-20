#include "information.h"
#include "sizedate.h"
Information::Information(QWidget *parent) : QWidget(parent)
{
    if (m_local.system().name() == "en_US") {
        this->resize(INFOR_SIZE.width()+7,INFOR_SIZE.height() + 20);
    } else if (m_local.system().name() == "zh_CN") {
        this->resize(INFOR_SIZE.width(),INFOR_SIZE.height() +20);
    }
//    this->adjustSize();
    m_ft.setBold(true);
    m_widName = new QLabel(this);
    m_widName->setAttribute(Qt::WA_TranslucentBackground);
    m_widName->setFont(m_ft);
//    m_widName->setAttribute();
    m_widName->setText(tr("Info"));
//    m_widName->setText(tr("信息"));
    m_name = new QLabel(this);
    m_name->setAttribute(Qt::WA_TranslucentBackground);
    m_name->setText(tr("Name"));
//    m_name->setText(tr("名称"));
    m_format = new QLabel(this);
    m_format->setAttribute(Qt::WA_TranslucentBackground);
//    m_format->setFixedHeight(16);
    m_format->setText(tr("Type"));
//    m_format->setText(tr("格式"));
    m_storageSize = new QLabel(this);
    m_storageSize->setAttribute(Qt::WA_TranslucentBackground);
    m_storageSize->setText(tr("Capacity"));
//    storageSize->setText(tr("大小"));
    m_pixelSize = new QLabel(this);
    m_pixelSize->setAttribute(Qt::WA_TranslucentBackground);
    m_pixelSize->setText(tr("Size"));
//    pixelSize->setText(tr("尺寸"));
    m_colorSpace = new QLabel(this);
    m_colorSpace->setAttribute(Qt::WA_TranslucentBackground);
    m_colorSpace->setText(tr("Color"));
//    colorSpace->setText(tr("颜色空间"));
    m_creationTime = new QLabel(this);
    m_creationTime->setAttribute(Qt::WA_TranslucentBackground);
    m_creationTime->setText(tr("Created"));
//    creationTime->setText(tr("创建时间"));
    m_revisionTime = new QLabel(this);
    m_revisionTime->setAttribute(Qt::WA_TranslucentBackground);
    m_revisionTime->setText(tr("Modified"));
//    revisionTime->setText(tr("修改时间"));

    m_nameC = new QLabel(this);
    m_nameC->setAttribute(Qt::WA_TranslucentBackground);
    m_formatC = new QLabel(this);
    m_formatC->setAttribute(Qt::WA_TranslucentBackground);
    m_storageSizeC = new QLabel(this);
    m_storageSizeC->setAttribute(Qt::WA_TranslucentBackground);
    m_pixelSizeC = new QLabel(this);
    m_pixelSizeC->setAttribute(Qt::WA_TranslucentBackground);
    m_colorSpaceC = new QLabel(this);
    m_colorSpaceC->setAttribute(Qt::WA_TranslucentBackground);
    m_creationTimeC = new QLabel(this);
    m_creationTimeC->setAttribute(Qt::WA_TranslucentBackground);
    m_revisionTimeC = new QLabel(this);
    m_revisionTimeC->setAttribute(Qt::WA_TranslucentBackground);
    //布局
    m_inforWid = new QWidget(this);
    m_gdLayout =new QGridLayout(this);
    this->layout();

}
//为各控件设置text
void Information::contentText(QFileInfo info, QString sizeImage, QString spaceColor)
{
    QString imageSize;
    if (float(info.size())/1024 >= 1024) {
        QString Size = QString("%1").arg(QString::asprintf("%.1f", float(info.size())/1024/1024));
        imageSize = Size + "Mib";
    } else {
        QString Size = QString("%1").arg(QString::asprintf("%.1f", float(info.size())/1024));
        imageSize = Size + "Kib";
    }
    longText(m_nameC,info.completeBaseName());
    m_formatC->setText(info.suffix());
    m_storageSizeC->setText(imageSize);
    m_pixelSizeC->setText(sizeImage);
    m_colorSpaceC->setText(spaceColor);
    m_creationTimeC->setText(info.birthTime().toString("yyyy.MM.dd hh:mm"));
    m_revisionTimeC->setText(info.lastModified().toString("yyyy.MM.dd hh:mm"));

}
//布局
void Information::layout()
{
    m_gdLayout->addWidget(m_widName,0,0,1,2);
    m_gdLayout->addWidget(m_name,1,0,1,1);
    m_gdLayout->addWidget(m_nameC,1,3,1,4);
    m_gdLayout->addWidget(m_format,2,0,1,1);
    m_gdLayout->addWidget(m_formatC,2,3,1,4);
    m_gdLayout->addWidget(m_storageSize,3,0,1,1);
    m_gdLayout->addWidget(m_storageSizeC,3,3,1,4);
    m_gdLayout->addWidget(m_pixelSize,4,0,1,1);
    m_gdLayout->addWidget(m_pixelSizeC,4,3,1,4);
    m_gdLayout->addWidget(m_colorSpace,5,0,1,2);
    m_gdLayout->addWidget(m_colorSpaceC,5,3,1,4);
    m_gdLayout->addWidget(m_creationTime,6,0,1,2);
    m_gdLayout->addWidget(m_creationTimeC,6,3,1,4);
    m_gdLayout->addWidget(m_revisionTime,7,0,1,2);
    m_gdLayout->addWidget(m_revisionTimeC,7,3,1,4);

    m_gdLayout->setContentsMargins(12,10,10,16);
    m_inforWid->setLayout(m_gdLayout);
    m_inforWid->resize(this->width(),this->height());
//    m_inforWid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}
//文字过长时，显示...，且可鼠标悬浮显示全部内容
void Information::longText(QLabel *nameC, QString text)
{
    QFontMetrics fontWidth(nameC->font());//得到每个字符的宽度
    int wid = fontWidth.width(m_revisionTimeC->text());
    QString elideNote = fontWidth.elidedText(text, Qt::ElideRight, wid);//最大宽度wid像素
    nameC->setText(elideNote);//显示省略好的字符串
    if (elideNote.contains("…",Qt::CaseInsensitive)) {
         nameC->setToolTip(text);//设置tooltips
    }
}
