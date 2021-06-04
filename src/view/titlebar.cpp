#include "titlebar.h"
#include "kyview.h"
#include "xatom-helper.h"
#include "sizedate.h"
TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{

    //布局
    g_titleWid = new QWidget(this);
    m_titleLayout = new QHBoxLayout(this);
    //左上角图标
//    m_logoBtn = new QLabel(this);
//    m_logoBtn->setFixedSize(LOGO_BTN);
//    m_logoBtn->setFixedSize(LOGO_BTN);//重置图标大小
//    m_logoBtn->setPixmap(QPixmap::fromImage(QImage(":/res/res/kyview_logo.png")));
//    m_logoBtn->setScaledContents(true);

    m_logoBtn = new QPushButton(this);
    m_logoBtn->setFixedSize(LOGO_BTN);//重置图标大小
    m_logoBtn->setIcon(QIcon::fromTheme("kylin-photo-viewer", QIcon(":/res/res/kyview_logo.png")));
    m_logoBtn->setIconSize(LOGO_BTN);
    m_logoBtn->setFocusPolicy(Qt::NoFocus);
    //左上角APP名字
    m_logolb = new QLabel(this);

    m_logolb->setText(tr("Pictures"));

    //中间图片名字
    g_imageName = new QLabel(this);
    g_imageName->hide();
    //窗口四联--菜单
    g_menu = new menuModule(this);
    g_menu->setFocusPolicy(Qt::NoFocus);
    //窗口四联--最小化按钮
    m_minibtn = new QPushButton(this);
    m_minibtn->setFixedSize(TITLE_BTN);
    m_minibtn->setToolTip(tr("minimize"));
    m_minibtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    m_minibtn->setIcon(QIcon::fromTheme("window-minimize-symbolic"));//主题库的最小化图标
    m_minibtn->setProperty("isWindowButton", 0x1);
    m_minibtn->setProperty("useIconHighlightEffect", 0x2);
    m_minibtn->setFlat(true);
    //窗口四联--最大化/还原按钮
    g_fullscreen = new QPushButton(this);
    g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的最小化图标
    g_fullscreen->setFixedSize(TITLE_BTN);
    g_fullscreen->setToolTip(tr("full screen"));
    g_fullscreen->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    g_fullscreen->setProperty("isWindowButton", 0x1);
    g_fullscreen->setProperty("useIconHighlightEffect", 0x2);
    g_fullscreen->setFlat(true);
    //窗口四联--关闭按钮
    m_closebtn = new QPushButton(this);
    m_closebtn->setToolTip(tr("close"));
    m_closebtn->setIcon(QIcon::fromTheme("window-close-symbolic"));//主题库的叉子图标
    m_closebtn->setFixedSize(TITLE_BTN);
    m_closebtn->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    m_closebtn->setProperty("isWindowButton", 0x2);
    m_closebtn->setProperty("useIconHighlightEffect", 0x8);
    m_closebtn->setFlat(true);
    m_logoBtn->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                             "QPushButton::hover{border:0px;border-radius:4px;background:transparent;}"
                             "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;}");
    this->setFixedHeight(BAR_HEIGHT);
    this->initConnect();
    this->initControlQss();
}
//顶栏图片名字
void TitleBar::showImageName(QString name)
{
    if (name == "") {
        return;
    }
    longText(g_imageName,name);
//    g_imageName->setText(name);
    g_imageName->show();

}
//名字过长显示...
void TitleBar::longText(QLabel *nameC, QString text)
{
    QFontMetrics fontWidth(nameC->font());//得到每个字符的宽度
    QString elideNote = fontWidth.elidedText(text, Qt::ElideRight, this->width()/2);//最大宽度wid像素
    nameC->setText(elideNote);//显示省略好的字符串
    //解决tooltips设置一次后一直显示的问题
    if (elideNote.contains("…",Qt::CaseInsensitive)) {
         nameC->setToolTip(text);//设置tooltips
    } else {
         nameC->setToolTip("");//设置tooltips
    }
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
    connect(m_closebtn, &QPushButton::clicked, Interaction::getInstance(), &Interaction::close);
    connect(Interaction::getInstance(),&Interaction::progremExit, KyView::mutual, &KyView::close);
    connect(g_menu,&menuModule::openSignal,this,&TitleBar::openSignal);
    connect(g_menu,&menuModule::aboutShow,this,&TitleBar::aboutShow);

}



