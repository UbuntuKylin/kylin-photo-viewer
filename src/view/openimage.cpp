#include "openimage.h"
#include "kyview.h"

OpenImage::OpenImage(QWidget *parent) : QWidget(parent)
{
    openInCenter = new QPushButton(this);
    addFile = new QPushButton(openInCenter);
    addFile->setFocusPolicy(Qt::NoFocus);
    openText = new QLabel(this);
    openInCenter->setFixedSize(128,128);
    openInCenter->move(0,0);
    addFile->setFixedSize(40,40);
    addFile->move(openInCenter->geometry().center()-addFile->rect().center());
    openText->setText(tr("Load picture"));
    openText->setFixedWidth(128);
    openText->setAlignment(Qt::AlignCenter);
    openText->move(0,128+30);
    iconsize = QSize(30,30);
    this->setFixedSize(128,128+50);
    this->_setstyle();
    this->_initconnect();
    this->_initGsettings();

}

//设置样式
void OpenImage::_setstyle()
{
        openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
//        addFile->setStyleSheet("background-image: url(:/res/res/addplus.png);");
        addFile->setIcon(QIcon(":/res/res/addplus.png"));
        addFile->setIconSize(QSize(30,30));
}
//初始化连接
void OpenImage::_initconnect()
{
    connect(openInCenter,&QPushButton::clicked,this,&OpenImage::openimage);
    connect(addFile,&QPushButton::clicked,this,&OpenImage::openimage);

}
//打开图片
void OpenImage::openimage()
{
    QString file_path;
    QString format;
    //构造打开条件
    format = "Image Files(";
    for(const QString &str:Variable::SUPPORT_FORMATS )
        format += "*."+str +" ";
    format += ")";
    //打开文件夹中的图片文件
    file_path = QFileDialog::getOpenFileName(this,"打开图片",Variable::getSettings("imagePath").toString(),format);
    //空校验，点击“取消”
    if(file_path.isEmpty())
        return;
    //写入配置
    QFileInfo info(file_path);
    Variable::setSettings("imagePath",info.absolutePath());
    emit openImage(file_path);
}

void OpenImage::_initGsettings()
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW)){
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,[=] (const QString &key)
        {
            if(key == "styleName")
                _dealSystemGsettingChange();
        });
    }
    _dealSystemGsettingChange();
    return;
}

void OpenImage::_dealSystemGsettingChange()
{
    QString themeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == themeStyle || "ukui-black" == themeStyle){
        openInCenter->setStyleSheet("background-color:rgba(0, 0, 0, 0.5);border-radius:64;");
        openText->setStyleSheet("QLabel{background-color:transparent;color:rgba(255, 255, 255, 1);}");
        addFile->setStyleSheet("background-color:transparent;");
        addFile->setIcon(QIcon(":/res/res/addplus.png"));
        addFile->setIconSize(QSize(30,30));
    }else{
        openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
        openText->setStyleSheet("QLabel{background-color:transparent;color:rgba(0, 0, 0, 1);}");
        addFile->setStyleSheet("background-color:transparent;");
        addFile->setIcon(QIcon(":/res/res/addplus.png"));
        addFile->setIconSize(QSize(30,30));
    }
}
