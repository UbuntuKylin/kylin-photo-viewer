#include "openimage.h"
#include "kyview.h"
#include "sizedate.h"
OpenImage::OpenImage(QWidget *parent) : QWidget(parent)
{
    //最外层的大圆形
    m_openInCenter = new QPushButton(this);
    m_openInCenter->setFocusPolicy(Qt::NoFocus);
    m_openInCenter->setFixedSize(OPENINCENTER_SIZE);
    m_openInCenter->move(0,0);
    //中间“+”号按钮
    m_addFile = new QPushButton(m_openInCenter);
    m_addFile->setFocusPolicy(Qt::NoFocus);
    m_addFile->setFixedSize(ADDFILESIZE);
    m_addFile->move(m_openInCenter->geometry().center()-m_addFile->rect().center());
    //按钮下的“打开图片”文字
    m_openText = new QLabel(this);   
    m_openText->setText(tr("Load picture"));
    m_openText->setFixedWidth(OPENIMAGEFIX);
    m_openText->setAlignment(Qt::AlignCenter);
    m_openText->move(0,OPENIMAGEFIX+30);

    this->setFixedSize(OPENIMAGESIZE);
    this->initconnect();
    this->initGsettings();

}

//信号槽
void OpenImage::initconnect()
{
    connect(m_openInCenter,&QPushButton::clicked,this,&OpenImage::openimage);
    connect(m_addFile,&QPushButton::clicked,this,&OpenImage::openimage);

}
//打开图片
void OpenImage::openimage()
{
    QString file_path;
    QString format;
    QString openFile = tr("Open Image");
    QString fileType = tr("Image Files(");
    //构造打开条件
    format = fileType;
    for(const QString &str:Variable::SUPPORT_FORMATS )
        format += "*."+str +" ";
    format += ")";
    //打开文件夹中的图片文件
    file_path = QFileDialog::getOpenFileName(this,openFile,Variable::getSettings("imagePath").toString(),format);
    //空校验，点击“取消”
    if(file_path.isEmpty())
    {
        return;
    }
    //写入配置
    QFileInfo info(file_path);
    Variable::setSettings("imagePath",info.absolutePath());
    emit openImage(file_path);
}

void OpenImage::initGsettings()
{
    if (QGSettings::isSchemaInstalled(FITTHEMEWINDOW)) {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,[=] (const QString &key)
        {
            if(key == "styleName")
                dealSystemGsettingChange();
        });
    }
    dealSystemGsettingChange();
    return;
}

void OpenImage::dealSystemGsettingChange()
{
    QString themeStyle = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == themeStyle || "ukui-black" == themeStyle) {
        m_openInCenter->setStyleSheet("background-color:rgba(0, 0, 0, 0.5);border-radius:64;");
        m_openText->setStyleSheet("QLabel{background-color:transparent;color:rgba(255, 255, 255, 1);}");
        m_addFile->setStyleSheet("background-color:transparent;");
        m_addFile->setIcon(QIcon(":/res/res/1add_black.png"));
        m_addFile->setIconSize(ICONSIZE);
    }else{
        m_openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 0.4);border-radius:64;");
        m_openText->setStyleSheet("QLabel{background-color:transparent;color:rgba(38, 38, 38, 0.85);}");
        m_addFile->setStyleSheet("background-color:transparent;");
        m_addFile->setIcon(QIcon(":/res/res/addplus.png"));
        m_addFile->setIconSize(ICONSIZE);
    }
}
