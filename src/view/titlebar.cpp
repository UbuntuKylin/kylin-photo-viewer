#include "titlebar.h"
#include "kyview.h"
#include "xatom-helper.h"
TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{

    titleWid = new QWidget(this);
    titleLayout = new QHBoxLayout(this);

    logoBtn = new QPushButton(this);
    logolb = new QLabel(this);
    logoBtn->setFixedSize(24,24);
    logoBtn->setIconSize(QSize(24,24));//重置图标大小
    logoBtn->setIcon(QIcon(":/res/res/kyview_logo.png"));
    logolb->setText(tr("Kylin Photo Viewer"));
    logoBtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型

    imageName = new QLabel(this);
    imageName->hide();

    m_menu = new menuModule(this);
    m_menu->setFocusPolicy(Qt::NoFocus);
    minibtn = new QPushButton(this);
    minibtn->setFixedSize(30,30);
    minibtn->setToolTip(tr("minimize"));
    minibtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    minibtn->setIcon(QIcon::fromTheme("window-minimize-symbolic"));//主题库的最小化图标
    minibtn->setProperty("isWindowButton", 0x1);
    minibtn->setProperty("useIconHighlightEffect", 0x2);
    minibtn->setFlat(true);

    fullscreen = new QPushButton(this);
    fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的最小化图标
    fullscreen->setFixedSize(30,30);
    fullscreen->setToolTip(tr("full screen"));
    fullscreen->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    fullscreen->setProperty("isWindowButton", 0x1);
    fullscreen->setProperty("useIconHighlightEffect", 0x2);
    fullscreen->setFlat(true);

    closebtn = new QPushButton(this);
    closebtn->setToolTip(tr("close"));
    closebtn->setIcon(QIcon::fromTheme("window-close-symbolic"));//主题库的叉子图标
    closebtn->setFixedSize(30,30);
    closebtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    closebtn->setProperty("isWindowButton", 0x2);
    closebtn->setProperty("useIconHighlightEffect", 0x8);
    closebtn->setFlat(true);

    this->setFixedHeight(40);
    this->setStyle();
    this->initConnect();
    this->initControlQss();
}

void TitleBar::showImageName(QString name)
{
    if(name == ""){
        return;
    }else{
        imageName->setText(name);
        imageName->show();
    }
}
//样式
void TitleBar::setStyle()
{

    logolb->setStyleSheet("background-color:transparent;");
    imageName->setStyleSheet("background-color:transparent;");
    logoBtn->setStyleSheet("background-color:transparent;");

}
//初始化顶栏布局
void TitleBar::initControlQss()
{


    titleLayout->addSpacing(4);
    titleLayout->addWidget(logoBtn);//logo
    titleLayout->addSpacing(4);
    titleLayout->addWidget(logolb);//标签
    titleLayout->addStretch();//添加伸缩
    titleLayout->addWidget(imageName);
    titleLayout->addStretch();//添加伸缩
    titleLayout->addWidget(m_menu->menuButton);//设置按钮
    titleLayout->addWidget(minibtn);
    titleLayout->addWidget(fullscreen);
    titleLayout->addWidget(closebtn);
    titleLayout->setSpacing(4);
    titleLayout->setMargin(4);
    this->setLayout(titleLayout);
}
//初始化连接
void TitleBar::initConnect()
{
    connect(minibtn, &QPushButton::clicked, KyView::mutual, &KyView::showMinimized);
    connect(fullscreen, &QPushButton::clicked, this, &TitleBar::fullRecovery);
    connect(closebtn, &QPushButton::clicked, KyView::mutual, &KyView::close);

}
//触发全屏事件
void TitleBar::fullRecovery()
{
    if (KyView::mutual->isFullScreen()){
        fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        fullscreen->setToolTip(tr("full srceen"));
        emit recovery();
    }else{
        fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        fullscreen->setToolTip(tr("recovery"));
        emit changeSize();
    }

}



