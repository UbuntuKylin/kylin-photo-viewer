#include "kyview.h"
#include "xatom-helper.h"
#include "sizedate.h"
KyView *KyView::mutual = nullptr;
KyView::KyView(const QStringList &args)

{
    Interaction::getInstance()->creatCore(args);

    this->resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    this ->setWindowTitle(tr("Pictures"));
    //毛玻璃
    this->setProperty("useSystemStyleBlur",true);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    //响应mouseMovEevent的无按压事件
    this->setMouseTracking(true);
    //响应拖拽事件
    this->setAcceptDrops(true);

    mutual = this;
    //应用居中
    QScreen *screen = QGuiApplication::primaryScreen();
    this ->move((screen->geometry().width() - DEFAULT_WIDTH) / 2,(screen->geometry().height() - DEFAULT_HEIGHT) / 2);

    //标题栏
    m_titlebar = new TitleBar(this);
    m_titlebar->move(0,0);
    m_titlebar->show();

    //默认打开--打开图片按钮widget
    m_openImage = new OpenImage(this);
    m_openImage->setFixedSize(OPEN_IMAGESIZE);
    m_openImage->move(int((this->width()-m_openImage->width())/2),int((this->height()-m_openImage->height())/2));

    //工具栏
    m_toolbar = new ToolBar(this);
    m_toolbar->move( int((this->width()-m_toolbar->width())/2),this->height() - m_toolbar->height() + 4);
    m_toolbar->show();

    //图片展示界面
    m_showImageWidget = new ShowImageWidget(this,DEFAULT_WIDTH,DEFAULT_HEIGHT);
    m_showImageWidget->setMouseTracking(true);
    m_showImageWidget->move(int((this->width() - m_showImageWidget->width())/2),int((this->height() - m_showImageWidget->height())/2));
    m_showImageWidget->hide();

    //导航器
    m_navigator = new Navigator(this);
//    m_navigator->resize(NAVI_SIZE);
    m_navigator->move(this->width()-7-m_navigator->width(),this->height()-52-m_navigator->height());
    m_navigator->hide();

    //信息栏
    m_information = new Information(this);
    if (m_local.system().name() == "en_US") {
        m_information->resize(INFOR_SIZE.width()+7,INFOR_SIZE.height());
    } else if (m_local.system().name() == "zh_CN") {
        m_information->resize(INFOR_SIZE);
    }
    m_information->move(this->width()-m_information->width() ,BAR_HEIGHT);
    this->installEventFilter(this);
    m_information->installEventFilter(this);
    m_titlebar->installEventFilter(this);
    m_information->hide();

    //相册
    m_sideBar = new SideBar(this);
//    m_sideBar->resize(SIDEBAR_SIZE);
//    m_sideBar->move(-6,(this->height()-m_sideBar->height())/2 + 20);
    m_sideBar->hide();

    //定时器
    m_timer = new QTimer(this);
//    m_timer->setSingleShot(true);
    m_timernavi = new QTimer(this);
//    m_timernavi->setSingleShot(true);
    m_timeNomove = new QTimer(this);
//    m_timeNomove->setSingleShot(true);


    // 用户手册功能
    m_DaemonIpcDbus = new DaemonDbus();

    this->setMinimumSize(MINI_SIZE);
    this->initconnect();
    //要判断打开状态，是否隐藏主界面--打开按钮widget
    this->openState();
    this->initGsetting();

    //初始化手势
    initGrabGesture();
}
KyView::~KyView()
{
}


//信号和槽
void KyView::initconnect()
{   
    //最大化和还原
    connect(m_titlebar,&TitleBar::recovery,this,&KyView::changOrigSize);
    //右上菜单--打开
    connect(m_titlebar,&TitleBar::openSignal,m_openImage,&OpenImage::openImagePath);
    //打开关于，两栏隐藏
    connect(m_titlebar,&TitleBar::aboutShow,this,&KyView::aboutShow);
    //更新信息栏
    connect(m_titlebar,&TitleBar::updateInformation,m_information,&Information::updateName);
    //判断为相册处切换图片，刷新动图右键内容
    connect(m_sideBar,&SideBar::changeImage,m_showImageWidget,&ShowImageWidget::albumChangeImage);
    //打开图片
    connect(m_openImage,&OpenImage::openImageSignal,m_showImageWidget,&ShowImageWidget::openImage);
    connect(m_openImage,&OpenImage::openEmptyFile,m_sideBar,&SideBar::openEmptyFile);
    //改变顶栏显示的图片名字
    connect(m_showImageWidget,&ShowImageWidget::perRate,m_toolbar,&ToolBar::changePerRate);
    //打开图片--主界面的界面显示处理
    connect(m_showImageWidget,&ShowImageWidget::toShowImage,this,&KyView::toShowImage);
    //获取图片详细信息
    connect(m_showImageWidget,&ShowImageWidget::changeInfor,m_information,&Information::contentText);
    //顶栏图片名字
    connect(m_showImageWidget,&ShowImageWidget::titleName,m_titlebar,&TitleBar::showImageName);
    //清空相册时，展示的界面
    connect(m_showImageWidget,&ShowImageWidget::clearImage,this,&KyView::clearImage);
    //滚轮放大和缩小
    connect(m_showImageWidget,&ShowImageWidget::reduceChange,m_toolbar,&ToolBar::reduceImage);
    connect(m_showImageWidget,&ShowImageWidget::enlargeChange,m_toolbar,&ToolBar::enlargeImage);
    //当图片大于2张及以上，默认展示相册---先保留
//    connect(m_showImageWidget,&ShowImageWidget::toShowSide,this,&KyView::defaultSidebar);
    //相册尺寸改变，位置改变
    connect(m_sideBar,&SideBar::sizeChange,this,&KyView::albumChange);
    //重命名
    connect(m_showImageWidget,&ShowImageWidget::toRename,m_titlebar,&TitleBar::needRename);
    //重命名，显示两栏
    connect(m_showImageWidget,&ShowImageWidget::isRename,this,&KyView::startRename);
    //展示或隐藏图片信息窗口
    connect(m_toolbar,&ToolBar::showInfor,this,&KyView::showInforWid);
    //导航器出现时，位置变化
    connect(m_navigator,&Navigator::naviChange,this,&KyView::naviChange);
    //显示相册
    connect(m_toolbar,&ToolBar::showSidebar,this,&KyView::showSidebar);
    //定时器
    connect(m_timer ,&QTimer::timeout, this, &KyView::delayHide);
    connect(m_timernavi ,&QTimer::timeout, this, &KyView::delayHide_navi);
    connect(m_timeNomove ,&QTimer::timeout, this, &KyView::delayHide_move);
    m_timeNomove->start(2500);
    //设置相册尺寸
    connect(m_showImageWidget,&ShowImageWidget::changeSideSize,m_sideBar,&SideBar::getSelect);
    connect(this,&KyView::albumState,m_showImageWidget,&ShowImageWidget::albumSlot);
    //删除时界面变化
    connect(m_toolbar,&ToolBar::isDelete,m_showImageWidget,&ShowImageWidget::isDelete);
//    connect(m_showImageWidget,&ShowImageWidget::toSelectHigh,m_sideBar,&SideBar::isDelete);
}
//打开首先检测是否需要展示工具栏
void KyView::openState()
{
    if (!m_openImage->isHidden()) {
        m_toolbar->hide();
        m_toolbar->move(int((this->width()-m_toolbar->width())/2),this->height() - m_toolbar->height() +4);
    }
}
//打开关于，两栏隐藏
void KyView::aboutShow()
{
    if (m_openImage->isHidden()) {
        if (!m_titlebar->isHidden()) {
            m_titlebar->hide();
            m_toolbar->hide();
        }
       if (!m_information->isHidden()) {
            m_information->move(this->width()-m_information->width() ,0);
       }
    }
}

void KyView::startRename()
{
    m_titlebar->show();
    m_toolbar->show();
}
//顶栏大小随主界面大小改变的响应函数
void KyView::titlebarChange()
{
    m_titlebar->move(0,0);
    m_titlebar->resize(this->width(),BAR_HEIGHT);
    m_titlebar->g_titleWid->resize(this->width(),BAR_HEIGHT);
}
//中间打开图片按钮大小随主界面大小改变的响应函数
void KyView::openImageChange()
{
    if (m_openImage->isHidden()) {
        return;
    }
    m_openImage->move(int((this->width()-m_openImage->width())/2),int((this->height()-m_openImage->height())/2));

}
//主界面展示的图片大小随主界面大小改变的响应函数
void KyView::showImageChange()
{
    m_showImageWidget->move(0,0);
    m_showImageWidget->resize(this->width(),this->height());
    m_showImageWidget->reMove(this->width(),this->height());
}
//工具栏大小随主界面大小改变的响应函数
void KyView::toolbarChange()
{
    if (m_toolbar->isHidden()) {
        return;
    }
    m_toolbar->move(int((this->width()-m_toolbar->width())/2),this->height() - m_toolbar->height() + 4);

}
//导航栏位置变化
void KyView::naviChange()
{
    if (m_navigator->isHidden()) {
        return;
    }
    m_navigator->move(this->width()-7-m_navigator->width(),this->height()-52-m_navigator->height());

}
//信息栏随顶栏位置变化而变化
void KyView::inforChange()
{
    if (m_information->isHidden()) {
        return;
    }
    if (m_titlebar->isHidden()) {
        m_information->move(this->width()-m_information->width(),0);
    } else {
        m_information->move(this->width()-m_information->width(), BAR_HEIGHT);
    }


}

void KyView::albumChange()
{
    if (m_sideBar->isHidden()) {
        return;
    }
    m_sideBar->move(0,(this->height()-m_sideBar->height())/2 + 20);
    m_showImageWidget->g_back->move(m_sideBar->x() + m_sideBar->width() + 42 ,m_showImageWidget->g_next->y());

}
//延时隐藏
void KyView::delayHide()
{

    if (this->mapFromGlobal(QCursor::pos()).y() < BAR_HEIGHT  || this->mapFromGlobal(QCursor::pos()).y() >= this->height()- BAR_HEIGHT) {
        return;
    }
    if (!m_titlebar->g_myEdit->isHidden()) {
        m_titlebar->show();
        m_toolbar->show();
        return;
    }
    if (!m_titlebar->isHidden() && !m_titlebar->g_menu->m_menu->isHidden())
    {
        m_titlebar->show();
        m_toolbar->show();
    }else{
        m_titlebar->hide();
        m_toolbar->hide();
    }

    inforChange();
}

//鼠标离开界面时需要触发，届时会加上对导航器的处理
void KyView::delayHide_navi()
{
    if (!m_titlebar->g_myEdit->isHidden()) {
        m_titlebar->show();
        m_toolbar->show();
        return;
    }
    m_titlebar->hide();
    m_toolbar->hide();
    inforChange();
    m_timernavi->stop();
}
//鼠标静止不动，延时两秒两栏隐藏
void KyView::delayHide_move()
{
    if (!m_openImage->isHidden()) {
        return;
    }
    if (!m_titlebar->isHidden() && m_titlebar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
        return;
    }
    if (!m_toolbar->isHidden() && m_toolbar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
        return;
    }
    if (!m_titlebar->g_myEdit->isHidden()) {
        m_titlebar->show();
        m_toolbar->show();
        return;
    }
    //如果下拉菜单show，则标题栏必须show
    if (!m_titlebar->g_menu->m_menu->isHidden()) {
        return;
    }
    //解决刚一打开图片，不到两秒就隐藏的问题
    if (m_timeNomove->isActive()) {
        m_timeNomove->stop();
    }
    m_titlebar->hide();
    m_toolbar->hide();
    inforChange();
    m_timeNomove->stop();
}
//展示信息栏
void KyView::showInforWid()
{
    if (m_inforState == true) {
        m_information->show();
        m_inforState = false;
        inforChange();
    } else {
        m_information->hide();
        m_inforState = true;
    }
}
//列表为空-清空图片--回到默认的打开界面
void KyView::clearImage()
{
    //三个time->stop,解当图片全部删除时返回默认界面，两栏偶现消失的问题
    if (m_timeNomove->isActive()) {
        m_timeNomove->stop();
    }
    if (m_timer->isActive()) {
        m_timer->stop();
    }
    if (m_timernavi->isActive()) {
        m_timernavi->stop();
    }
    m_openImage->show();
    m_showImageWidget->hide();
    m_toolbar->hide();
    m_information->hide();   
    m_titlebar->show();
    if (!m_sideBar->isHidden()) {
        m_sideBar->hide();
    }
    m_titlebar->g_imageName->clear();

}
//处理鼠标悬浮两栏的界面展示
void KyView::hoverChange(int y)
{
    static int i = 0;

    if (i == 0) {
        m_timer->start(2500);
    }
    if (y <= BAR_HEIGHT  || y >= this->height() - BAR_HEIGHT) {
        //需要获取从两栏到中间的变化，且为防止频繁操作
        if (m_timestart == false) {
            if (m_timer->remainingTime() < 1000 && m_timer->isActive()) {
                m_timer->stop();
            }
            m_timestart = true;
        }
        m_toolbar->show();
        m_titlebar->show();

        //顶栏和标题栏位置的变化
        toolbarChange();
        titlebarChange();

        this->m_showImageWidget->lower();
        this->m_showImageWidget->g_next->hide();
        this->m_showImageWidget->g_back->hide();

        //判断具体在顶栏或工具栏的区域，将其raise
        if (y <= BAR_HEIGHT) {
            m_titlebar->raise();
        } else if (y >= this->height() - BAR_HEIGHT) {
            m_toolbar->raise();
        }
        //信息栏位置的变化
        inforChange();
        return;
    }

    if (m_timestart == true) {

        if ((m_timer->remainingTime() < 2000 && m_timer->isActive() ) || !(m_timer->isActive())) {
            m_timer->stop();
            m_timer->start(2500);
        }
        m_timestart = false;
    }
    //判断定时器是否正在计时。如是，则停止
    if (m_timernavi->isActive()) {
        m_timernavi->stop();
    }

    //判断列表中是否只有一张图，一张图片左右按钮不显示
    if (m_showImageWidget->g_buttonState == false) {
        this->m_showImageWidget->g_next->hide();
        this->m_showImageWidget->g_back->hide();
    } else {
        this->m_showImageWidget->g_next->show();
        this->m_showImageWidget->g_back->show();
    }

}
//读取主题配置文件
void KyView::initGsetting()
{
    if (QGSettings::isSchemaInstalled(FITTHEMEWINDOW)) {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this, [=] (const QString &key) {
            if (key == "styleName") {
                themeChange();
            }
        });
    }

    if (QGSettings::isSchemaInstalled(FITCONTROLTRANS)) {
        m_pGsettingControlTrans = new QGSettings(FITCONTROLTRANS);
        connect(m_pGsettingControlTrans,&QGSettings::changed, this, [=] (const QString &key){
            if (key == "transparency") {
                transChange();
            }
        });
    }
    themeChange();
    transChange();
    return;

}
//主题变化
void KyView::themeChange()
{
    QString themeStyle = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == themeStyle || "ukui-black" == themeStyle) {
        m_information->setStyleSheet("QWidget{background-color:rgba(0,0,0,0.72);border-top-left-radius:0px;border-top-right-radius：0px;border-bottom-left-radius:4px;border-bottom-right-radius:0px;}");
        m_titlebar->g_menu->setThemeDark();
        m_showImageWidget->g_next->setIcon(QIcon(":/res/res/1right.png"));
        m_showImageWidget->g_back->setIcon(QIcon(":/res/res/1left.png"));
        m_sideBar->setStyleSheet("QListView{border:1px ;border-top-left-radius:0px;border-top-right-radius:4px;border-bottom-left-radius:0px;border-bottom-right-radius:4px;outline:none;background:rgba(63, 69, 77, 1)}"
                                   "QListView::item{margin:0 2px 0 0;background:rgba(0, 0, 0, 0.4);border-radius:2px;}"
                                   "QListView::item:selected{border:2px solid rgba(13, 135, 255, 1);background:rgba(0, 0, 0, 0.4);border-radius:2px;}"
                                   "QListView::item:hover{background:rgba(0, 0, 0, 0.4);border-radius:2px;}");
    } else {
        m_information->setStyleSheet("QWidget{background-color:rgba(225, 240, 250, 0.72);border-top-left-radius:0px;border-top-right-radius：0px;border-bottom-left-radius:4px;border-bottom-right-radius:0px;}");
        m_sideBar->setStyleSheet("QListView{border:1px ;border-top-left-radius:0px;border-top-right-radius:4px;border-bottom-left-radius:0px;border-bottom-right-radius:4px;outline:none;background:rgba(227, 235, 239, 1)}"
                                   "QListView::item{margin:0 2px 0 0;background:rgba(255, 255, 255, 0.5);border-radius:2px;}"
                                   "QListView::item:selected{border:2px solid rgba(13, 135, 255, 0.86);background:rgba(255, 255, 255, 0.9);border-radius:2px;}"
                                   "QListView::item:hover{background:rgba(255, 255, 255, 0.9);border-radius:2px;}");
        m_titlebar->g_menu->setThemeLight();
        m_showImageWidget->g_next->setIcon(QIcon(":/res/res/right.png"));
        m_showImageWidget->g_back->setIcon(QIcon(":/res/res/left.png"));
    }
    Interaction::getInstance()->changeOpenIcon(themeStyle);
    m_toolbar->changeStyle();

}


void KyView::transChange()
{
  m_tran = m_pGsettingControlTrans->get("transparency").toDouble() * 255;
  this->update();

}

void KyView::avoidChange()
{
    //暂时用此方法解决鼠标在子上释放事件不被检测的bug，以后如有更好的办法再更改。
    //解决点击导航器，触发图片切换的问题。
    if (m_navigator != nullptr) {
        if (!m_navigator->isHidden() && m_navigator->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            m_panTriggered = false;
        } /*else { //此部分会导致打开相册后就将m_panTriggered设为true，导致->左右滑动事件点击即触发
            if (m_navigator->isHidden())
            m_panTriggered = true;
        }*/
    }
    //解决点击侧栏，触发图片切换的问题。
    if (m_sideBar != nullptr) {
        if (!m_sideBar->isHidden() && m_sideBar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            m_panTriggered = false;
        }
    }
    //解决点击左右按钮，触发图片切换的问题。
    if (m_showImageWidget != nullptr) {
        if (!m_showImageWidget->g_back->isHidden() && (m_showImageWidget->g_back->geometry().contains(this->mapFromGlobal(QCursor::pos())) ||m_showImageWidget->g_next->geometry().contains(this->mapFromGlobal(QCursor::pos())) )) {
            m_panTriggered = false;
        }
    }
    //解决点击顶栏，触发图片切换的问题。
    if (m_titlebar != nullptr) {
        if (!m_titlebar->isHidden() && m_titlebar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            m_panTriggered = false;
        }
    }
    //解决点击工具栏，触发图片切换的问题。
    if (m_toolbar != nullptr) {
        if (!m_toolbar->isHidden() && m_toolbar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            m_panTriggered = false;
        }
    }
}

//最大化和还原
void KyView::changOrigSize()
{

    if (this->isMaximized()) {
        this->showNormal();
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        m_titlebar->g_fullscreen->setToolTip(tr("full srceen"));
    } else {
        this->showMaximized();
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        m_titlebar->g_fullscreen->setToolTip(tr("recovery"));
    }
}

//显示图片--主界面的界面显示
void KyView::toShowImage()
{
    this->m_openImage->hide();
    this->m_showImageWidget->show();
    if (!m_titlebar->isHidden()) {
        this->m_showImageWidget->lower();
    }
    inforChange();
    if (m_timeNomove->isActive()) {
        m_timeNomove->stop();
        return;
    }
    m_timeNomove->start(2500);
}
//显示相册
void KyView::showSidebar()
{

    if (m_albumState == true) {
        m_sideBar->show();
        emit albumState(true);
        albumChange();
        m_albumState = false;
    } else {
        m_sideBar->hide();
        m_showImageWidget->g_back->move( LEFT_POS,m_showImageWidget->g_next->y());
        emit albumState(false);
        m_albumState = true;
    }

}
//先保留--默认打开相册
//void KyView::defaultSidebar()
//{
//    m_sideBar->show();
//    emit albumState(true);
//    albumChange();
//    m_albumState = false;
//}
//检测鼠标位置--顶栏和工具栏的出现和隐藏
void KyView::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标移入特定区域显示/隐藏控件
    if (m_openImage->isHidden()) {
        int y =this->mapFromGlobal(QCursor().pos()).y();
        hoverChange(y);
    } else {
        m_toolbar->hide();
        m_titlebar->show();
    }
    //窗口拖拽
    if (m_titlebar == nullptr) {
        return;
    }
    if (m_titlebar->isHidden() || !m_titlebar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
        return;
    }
    if (!m_mousePress) {
        return;
    }
    double device = QGuiApplication::primaryScreen()->devicePixelRatio();
    if (device <= 0) {
        device = 1;
    }
    m_mousePointFromWindow = event->pos();

    QPoint pos = this->mapToGlobal(event->pos());

    XEvent xEvent;
    memset(&xEvent, 0, sizeof(XEvent));

    Display *display = QX11Info::display();
    xEvent.xclient.type = ClientMessage;
    xEvent.xclient.message_type = XInternAtom(display, "_NET_WM_MOVERESIZE", False);
    xEvent.xclient.display = display;
    xEvent.xclient.window = (XID)(this ->winId());
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[0] = pos.x() * device;
    xEvent.xclient.data.l[1] = pos.y() * device;
    xEvent.xclient.data.l[2] = 8;
    xEvent.xclient.data.l[3] = Button1;
    xEvent.xclient.data.l[4] = 1;

    XUngrabPointer(display, CurrentTime);
    XSendEvent(display,
               QX11Info::appRootWindow(QX11Info::appScreen()),
               False,
               SubstructureNotifyMask | SubstructureRedirectMask,
               &xEvent);
    XFlush(display);
    event->accept();
}
void KyView::mousePressEvent(QMouseEvent * event)
{
    //只能是鼠标左键移动和改变大小
    if (event->button() == Qt::LeftButton) {
        m_mousePress = true;
    }
}

void KyView::mouseReleaseEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    m_mousePress = false;
}

//拖拽主窗口--各控件需要改变位置或尺寸
void KyView::resizeEvent(QResizeEvent *event)
{

    titlebarChange();
    openImageChange();
    showImageChange();
    toolbarChange();
    naviChange();
    inforChange();
    albumChange();

}
//鼠标离开事件
void KyView::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_titleState = false;
    //点出顶栏的下拉菜单时，不隐藏
    if (!m_titlebar->isHidden() && !m_titlebar->g_menu->m_menu->isHidden()) {
        return;
    }
    if (m_openImage->isHidden()) {
        if (!m_timernavi->isActive()) {
            m_timernavi->start(2500);
        }
        m_showImageWidget->g_next->hide();
        m_showImageWidget->g_back->hide();
    }

    inforChange();
//    if(m_navigator->isHidden()){
//        return;
//    }else{
//        timer_navi->start(2000);
    //    }
}
//鼠标进入事件
void KyView::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_titleState = true;
    if (m_timernavi->isActive()) {
        m_timernavi->stop();
    }
    if (m_sideBar->isHidden()) {
        return;
    }
    if (m_sideBar->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
        m_showImageWidget->g_back->show();
        m_showImageWidget->g_next->show();
    }
    if ((m_timer->remainingTime() < 2000 && m_timer->isActive()) ||
         !(m_timer->isActive())) {
        m_timer->stop();
        m_timer->start(2500);
    }
}

void KyView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    QPainterPath rectPath;
    rectPath.addRoundedRect(this->rect(), 0, 0); // 左上右下
//    double tran=m_pGsettingControlTrans->get("transparency").toDouble()*255;
    QStyleOption opt;
    opt.init(this);

    QColor mainColor;

    if (QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base)) {
        mainColor = QColor(242, 242, 242,m_tran);
    } else {
        mainColor = QColor(20, 20, 20,m_tran);
    }

    p.fillPath(rectPath,QBrush(mainColor));
}
// F1快捷键打开用户手册
void KyView::keyPressEvent(QKeyEvent *event)
{

    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!m_DaemonIpcDbus->daemonIsNotRunning()) {
            //F1快捷键打开用户手册，如kylin-photo-viewer
            //如果是小工具类，下面的showGuide参数要填写"tools/kylin-photo-viewer"
            m_DaemonIpcDbus->showGuide(USER_GUIDE);
        }
    }
    //上一张，下一张，delete按键响应
    if (!m_openImage->isHidden()) {
        return;
    } else {
        if (event->key() == Qt::Key_Delete) {
            m_toolbar->delImage();
        }
        if (event->key() == Qt::Key_Left) {
            m_showImageWidget->m_canSet = true;
            m_showImageWidget->backImage();
        }
        if (event->key() == Qt::Key_Right) {
            m_showImageWidget->m_canSet = true;
            m_showImageWidget->nextImage();
        }

    }
}
//添加左键双击事件--最大化和还原
void KyView::mouseDoubleClickEvent(QMouseEvent *event)
{
    //判断左键双击
    if (event->button() != Qt::LeftButton) {
        return;
    }
    //在标题栏的图片名字处双击不响应全屏或恢复
    if (m_titlebar->g_imageName->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
        return;
    }
//    if (event->button() == Qt::LeftButton) {
    if (this->isMaximized()) {
        this->showNormal();
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        m_titlebar->g_fullscreen->setToolTip(tr("full srceen"));
    } else {
        this->showMaximized();
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        m_titlebar->g_fullscreen->setToolTip(tr("recovery"));

        }
//    }
}
//检测鼠标在信息栏上方进入，顶栏和工具栏要展示
bool KyView::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_information) {

        //如果信息栏show并且，鼠标在信息栏上
        if (!m_information->isHidden() && m_information->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            int infor_y = m_information->mapFromGlobal(QCursor().pos()).y();
            //判断鼠标在信息栏的位置
            if (infor_y <= BAR_HEIGHT) {
                if (m_timernavi->isActive()) {
                    m_timernavi->stop();
                }
                if ((m_timer->remainingTime() < 2000 && m_timer->isActive()) ||
                     !(m_timer->isActive())) {
                    m_timer->stop();
                    m_timer->start(2500);
                }
                m_titlebar->show();
                m_toolbar->show();
                inforChange();
            }

        }
    }

    //最大化和还原时按钮图标设置
    if (this == obj) {
        if (this->windowState() == Qt::WindowMaximized) {
            m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
            m_titlebar->g_fullscreen->setToolTip(tr("recovery"));
        } else {
            m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
            m_titlebar->g_fullscreen->setToolTip(tr("full srceen"));

    }
    }
    return QObject::eventFilter(obj,event);
}

//拖文件进界面
void KyView::dragEnterEvent(QDragEnterEvent *event)
{
    //获得所有可被查看的图片格式
    QStringList formatList;
    QString format;
    format = "";
    for (const QString &str:Variable::SUPPORT_FORMATS) {
        format = str;
        formatList.append(format);
    }
    //检查当前拖入的否为url，暂时只支持以url形式拖入
    if (!event->mimeData()->hasUrls()) {
        return;
    }
    //极端情况下，拖拽文件时list有可能为空，不做异常处理时，会闪退。
    QList<QUrl> list = event->mimeData()->urls();
    if (list.isEmpty()) {
        return;
    }
    QUrl url = list.first();
    if (url.isEmpty()) {
        return;
    }
    QString str = url.fileName();
    if (str.isEmpty()) {
        return;
    }

    //判断图片是否支持被查看
    if (formatList.contains( QFileInfo(str).suffix().toLower())) {
        event->acceptProposedAction();
    } else {
        event->ignore();//否则不接受鼠标事件
    }

}
//放下文件之后响应
void KyView::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    //判断是否为空
    if (urls.isEmpty()) {
        return;
    }
    //拿路径，发信号打开图片
    QString path = urls.first().toLocalFile();
    if (path == "") {
        return;
    }
    QFileInfo info(path);
    Variable::setSettings("imagePath",info.absolutePath());
    emit m_showImageWidget->openImage(path);

}

void KyView::initGrabGesture()
{

    //手势需要关闭主题事件
    this->setProperty("useStyleWindowManager",false);
    // 注册手势
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->grabGesture(Qt::PinchGesture);//捏手势。
    this->grabGesture(Qt::TapAndHoldGesture);//长按手势
    QTapAndHoldGesture::setTimeout(1);//按下n毫秒判定为长按
}
bool KyView::event(QEvent *event)
{
    //判断列表中是否只有一张图，一张图片左右按钮不显示--解决静止鼠标，按钮仍然显示的问题
    if (m_openImage != nullptr && m_openImage->isHidden()) {
        if (m_showImageWidget->g_buttonState == false) {
            this->m_showImageWidget->g_next->hide();
            this->m_showImageWidget->g_back->hide();
        }
    }
    //解决点击下拉菜单出现，鼠标移出窗体，点击窗体外部，两秒后两栏未隐藏的问题。
    if (m_titleState == false) {
        if (m_titlebar->g_menu->m_menu->isHidden() && !m_titlebar->isHidden()) {
            if (!m_timeNomove->isActive()) {
                m_timeNomove->start(2500);
            }
        }
    }
    avoidChange();
    //手势处理
    if (event->type() ==  QEvent::GestureOverride ||event->type() ==  QEvent::Gesture) {
        return gestureEvent(event);
    }

    //使用X11接管窗口移动事件后，鼠标离开事件不触发，此处作为备用方案
    if (m_mousePress) {
        if (event->type() == QEvent::Move) {
            //本次不响应切换手势
            m_panTriggered = false;
            x11EventEnd();
        }
    }
    if (m_panTriggered) {
        if (event->type() == QEvent::MouseButtonRelease) {
            m_panTriggered = false;
            //如果本次点击出发了旋转或缩放，不处理
            if (m_touchPoint.rx() == -1) {
                return QWidget::event(event);
            }
            QMouseEvent *eve = static_cast<QMouseEvent *>(event);
            int distance = eve->globalPos().rx() - m_touchPoint.rx();
            //移动距离超过此值才判定为滑动
            if (distance > 100) {
                m_showImageWidget->m_canSet = true;
                Interaction::getInstance()->backImage();
            } else if (distance < -100) {
                m_showImageWidget->m_canSet = true;
                Interaction::getInstance()->nextImage();
            }
        }
        return QWidget::event(event);
    }

    return QWidget::event(event);
}

void KyView::x11EventEnd()
{
    double device = QGuiApplication::primaryScreen()->devicePixelRatio();
    if (device <= 0) {
        device = 1;
    }
    QPoint globalPoints = QCursor::pos();

    XEvent xEvent;
    memset(&xEvent, 0, sizeof(XEvent));
    Display *display = QX11Info::display();
    xEvent.type = ButtonRelease;
    xEvent.xbutton.button = Button1;
    xEvent.xbutton.window = this->effectiveWinId();
    if (globalPoints.y() < 5) {
        xEvent.xbutton.x = globalPoints.x() * device;
        xEvent.xbutton.y = globalPoints.y() * device;
    } else {
        xEvent.xbutton.x = this->mapFromGlobal(globalPoints).x() * device;
        xEvent.xbutton.y = this->mapFromGlobal(globalPoints).y() * device;
    }
    xEvent.xbutton.x_root = globalPoints.x()* device;
    xEvent.xbutton.y_root = globalPoints.y()* device;
    xEvent.xbutton.display = display;

    XSendEvent(display,this->effectiveWinId(),False,ButtonReleaseMask,&xEvent);
    XFlush(display);

}

// 手势识别
bool KyView::gestureEvent(QEvent *event)
{
    QGestureEvent *gestureEvent = static_cast<QGestureEvent *>(event);
    if (QGesture *gesture = gestureEvent->gesture(Qt::PinchGesture)) {
        pinchTriggered(static_cast<QPinchGesture *>(gesture));
        return true;
    }
    if (QGesture *gesture = gestureEvent->gesture(Qt::TapAndHoldGesture)) {
        tapAndHoldGesture(static_cast<QTapAndHoldGesture *>(gesture));
        return true;
    }
    return false;
}

void KyView::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags(); // 获取已改变的手势属性

    if (changeFlags & QPinchGesture::RotationAngleChanged) { // 手势覆盖角度改变
        if (!m_isResize) {
            const qreal lastAngle = gesture->lastRotationAngle();
            if (touchRotate(lastAngle)) {
                m_isRotate = true;
                m_touchPoint.setX(-1);//特殊标记
            }
        }
    }

    if (changeFlags & QPinchGesture::ScaleFactorChanged) { // 两个输入点的距离改变
        if (!m_isRotate) {
            qreal scaleFactor = gesture->totalScaleFactor();
            qreal tmpNum = scaleFactor - m_lastDistance;
            //避免操作太频繁，移动幅度超过某值才做处理
            if (tmpNum > m_minDistance) {
                Interaction::getInstance()->watchBigImage();
                m_lastDistance = scaleFactor;
            } else if (tmpNum < m_minDistance * -1) {
                Interaction::getInstance()->watchSmallImage();
                m_lastDistance = scaleFactor;
            }
            if (m_lastDistance != 1) {
                m_isResize=true;
                m_touchPoint.setX(-1);//特殊标记
                m_minDistance = 0.1;//判断为缩放后提高灵敏度
            }
        }
    }

    if (gesture->state() == Qt::GestureFinished) { // 手势结束
        m_rotateLevel = 0;
        m_isRotate = false;
        m_isResize = false;
        m_lastDistance = 1;
        m_minDistance = 0.5;
    }

}

bool KyView::touchRotate(const qreal &lastAngle)
{
    bool result = false;
    // 最后的手势覆盖角度介于-45度和45度之间
    if (lastAngle > -45 && lastAngle < 45 && lastAngle != 0) {
        if (m_rotateLevel == -1) {
            //顺时针转到此区域
            Interaction::getInstance()->rotate();
            result = true;
        } else if (m_rotateLevel == 1) {
            //逆时针转到此区域
            Interaction::getInstance()->rotate(false);
            result = true;
        }
        //记录区域标签
        m_rotateLevel = 0;
        return result;
    }

    if (lastAngle > 45 && lastAngle < 135) {
        if (m_rotateLevel == 0) {
            Interaction::getInstance()->rotate();
            result = true;
        } else if (m_rotateLevel == 2) {
            Interaction::getInstance()->rotate(false);
            result = true;
        }
        m_rotateLevel = 1;
        return result;
    }
    if (lastAngle > 135 && lastAngle < 180) {
        if (m_rotateLevel == 1) {
            Interaction::getInstance()->rotate();
            result = true;
        }
        m_rotateLevel = 2;
        return result;
    }

    if (lastAngle < -45 && lastAngle > -135) {
        if (m_rotateLevel == 0) {
            Interaction::getInstance()->rotate(false);
            result = true;
        } else if (m_rotateLevel == -2) {
            Interaction::getInstance()->rotate();
            result = true;
        }
        m_rotateLevel = -1;
        return result;
    }
    if (lastAngle < -90 && lastAngle > -180) {
        if (m_rotateLevel == -1) {
            Interaction::getInstance()->rotate(false);
            result = true;
        }
        m_rotateLevel = -2;
        return result;
    }
    return result;
}

void KyView::tapAndHoldGesture(QTapAndHoldGesture *gesture)
{
    if (m_panTriggered) {
        return;
    }
    m_touchPoint = gesture->position();
    if (!m_openImage->isHidden()) {
        return;
    }
    m_panTriggered = true;

}
