#include "titlebar.h"
#include "kyview.h"
TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{

    titleWid = new QWidget(this);
    titleLayout = new QHBoxLayout(this);

    logoBtn = new QPushButton(this);
    logolb = new QLabel(this);
    logolb->setFixedSize(100,24);
    logoBtn->setFixedSize(24,24);
    logoBtn->setIconSize(QSize(24,24));//重置图标大小
    logoBtn->setIcon(QIcon(":/res/control_icons/logo_24.png"));
    logolb->setText(tr("Kylin View"));
    logoBtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型


    m_menu = new menuModule(this);

    minibtn = new QPushButton(this);
    minibtn->setFixedSize(30,30);
    minibtn->setToolTip(tr("minimize"));
    minibtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    minibtn->setIcon(QIcon::fromTheme("window-minimize-symbolic"));//主题库的最小化图标

    fullscreen = new QPushButton(this);
    fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的最小化图标
    fullscreen->setFixedSize(30,30);
    fullscreen->setToolTip(tr("full screen"));
    fullscreen->setFocusPolicy(Qt::NoFocus);//设置焦点类型

    closebtn = new QPushButton(this);
    closebtn->setToolTip(tr("close"));
    closebtn->setIcon(QIcon::fromTheme("window-close-symbolic"));//主题库的叉子图标
    closebtn->setFixedSize(30,30);
    closebtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型

    this->setFixedHeight(40);
    this->setStyle();
    this->initConnect();
    this->initControlQss();

}
//样式
void TitleBar::setStyle()
{
    this->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");
    logolb->setStyleSheet("background-color:transparent;");
    minibtn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-color:rgba(0,0,0,0.1);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-color:rgba(0,0,0,0.15);}");
    fullscreen->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-color:rgba(0,0,0,0.1);}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-color:rgba(0,0,0,0.15);}");
    closebtn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                               "QPushButton:Hover{border:0px;border-radius:4px;background:transparent;background-color:#F86457;}"
                               "QPushButton:Pressed{border:0px;border-radius:4px;background:transparent;background-color:#E44C50;}");

}
//初始化顶栏布局
void TitleBar::initControlQss()
{


    titleLayout->addSpacing(4);
    titleLayout->addWidget(logoBtn);//logo
    titleLayout->addSpacing(4);
    titleLayout->addWidget(logolb);//标签
    titleLayout->addStretch();//添加伸缩
    titleLayout->addWidget(m_menu->menuButton);//设置按钮
    titleLayout->addWidget(minibtn);
    titleLayout->addWidget(fullscreen);
    titleLayout->addWidget(closebtn);
    titleLayout->setSpacing(4);
    titleLayout->setMargin(4);
    titleWid->setLayout(titleLayout);
//    titleWid->resize(KyView::mutual->width(),40);
//    titleWid->resize(800,40);
//    titleWid->setFixedHeight(40);
//    titleWid->setMinimumWidth(1920);
//    titleWid->setMaximumWidth(1920);
//    titleWid->move(0,0);
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

