#include "titlebar.h"
#include "kyview.h"
#include "xatom-helper.h"
#include "sizedate.h"
TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{


    g_titleWid = new QWidget(this);
    m_titleLayout = new QHBoxLayout(this);

    m_logoBtn = new QPushButton(this);
    m_logolb = new QLabel(this);
    m_logoBtn->setFixedSize(SizeDate::LOGOBTN);
    m_logoBtn->setIconSize(SizeDate::LOGOBTN);//重置图标大小
    m_logoBtn->setIcon(QIcon(":/res/res/kyview_logo.png"));
    m_logolb->setText(tr("Kylin Photo Viewer"));

    g_imageName = new QLabel(this);
    g_imageName->hide();

    g_menu = new menuModule(this);
    g_menu->setFocusPolicy(Qt::NoFocus);
    m_minibtn = new QPushButton(this);
    m_minibtn->setFixedSize(SizeDate::TITLEBTN);
    m_minibtn->setToolTip(tr("minimize"));
    m_minibtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    m_minibtn->setIcon(QIcon::fromTheme("window-minimize-symbolic"));//主题库的最小化图标
    m_minibtn->setProperty("isWindowButton", 0x1);
    m_minibtn->setProperty("useIconHighlightEffect", 0x2);
    m_minibtn->setFlat(true);

    g_fullscreen = new QPushButton(this);
    g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的最小化图标
    g_fullscreen->setFixedSize(SizeDate::TITLEBTN);
    g_fullscreen->setToolTip(tr("full screen"));
    g_fullscreen->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    g_fullscreen->setProperty("isWindowButton", 0x1);
    g_fullscreen->setProperty("useIconHighlightEffect", 0x2);
    g_fullscreen->setFlat(true);

    m_closebtn = new QPushButton(this);
    m_closebtn->setToolTip(tr("close"));
    m_closebtn->setIcon(QIcon::fromTheme("window-close-symbolic"));//主题库的叉子图标
    m_closebtn->setFixedSize(SizeDate::TITLEBTN);
    m_closebtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    m_closebtn->setProperty("isWindowButton", 0x2);
    m_closebtn->setProperty("useIconHighlightEffect", 0x8);
    m_closebtn->setFlat(true);

    this->setFixedHeight(SizeDate::BARHEIGHT);
    this->setStyle();
    this->initConnect();
    this->initControlQss();
}
//顶栏图片名字
void TitleBar::showImageName(QString name)
{
    if(name == ""){
        return;
    }else{
        g_imageName->setText(name);
        g_imageName->show();
    }
}
//样式
void TitleBar::setStyle()
{

    m_logolb->setStyleSheet("background-color:transparent;");
    g_imageName->setStyleSheet("background-color:transparent;");
    m_logoBtn->setStyleSheet("background-color:transparent;");
    g_titleWid->setWindowOpacity(0.7);

}
//初始化顶栏布局
void TitleBar::initControlQss()
{
    m_titleLayout->addSpacing(4);
    m_titleLayout->addWidget(m_logoBtn);//logo
    m_titleLayout->addSpacing(4);
    m_titleLayout->addWidget(m_logolb);//标签
    m_titleLayout->addStretch();//添加伸缩
    m_titleLayout->addWidget(g_imageName);
    m_titleLayout->addStretch();//添加伸缩
    m_titleLayout->addWidget(g_menu->menuButton);//设置按钮
    m_titleLayout->addWidget(m_minibtn);
    m_titleLayout->addWidget(g_fullscreen);
    m_titleLayout->addWidget(m_closebtn);
    m_titleLayout->setSpacing(4);
    m_titleLayout->setMargin(4);
    this->setLayout(m_titleLayout);
}
//信号和槽
void TitleBar::initConnect()
{
    connect(m_minibtn, &QPushButton::clicked, KyView::mutual, &KyView::showMinimized);
    connect(g_fullscreen, &QPushButton::clicked, this, &TitleBar::recovery);
    connect(m_closebtn, &QPushButton::clicked, KyView::mutual, &KyView::close);
    connect(g_menu,&menuModule::openSignal,this,&TitleBar::openSignal);
    connect(g_menu,&menuModule::aboutShow,this,&TitleBar::aboutShow);

}



