#include "kyview.h"
#include "xatom-helper.h"
KyView *KyView::mutual = nullptr;
KyView::KyView(const QStringList &args)
{
    Interaction *interaction =Interaction::getInstance();
    interaction->creatCore(args);

    int WIDTH = 1080;
    int HEIGHT = 720;
    this->resize(1080, 720);

    this ->setWindowIcon(QIcon(":/res/res/kyview_logo.png"));
    this ->setWindowTitle(tr("Kylin Photo Viewer"));
    //毛玻璃
    this->setProperty("useSystemStyleBlur",true);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setMouseTracking(true);
    this->setAcceptDrops(true);

    mutual = this;

    QScreen *screen = QGuiApplication::primaryScreen();
    this ->move((screen->geometry().width() - WIDTH) / 2,(screen->geometry().height() - HEIGHT) / 2);

    titlebar = new TitleBar(this);
    titlebar->setAutoFillBackground(true);
    titlebar->setBackgroundRole(QPalette::Base);
    titlebar->move(0,0);

    openImage = new OpenImage(this);
    openImage->setFixedSize(128,128+50);
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));

    toolbar = new ToolBar(this);
    toolbar->move( int((this->width()-toolbar->width())/2),this->height() - toolbar->height() + 4);

    toolbar->show();
    titlebar->show();

    showImageWidget = new ShowImageWidget(this,WIDTH,HEIGHT);
    showImageWidget->setMouseTracking(true);
    showImageWidget->move(int((this->width() - showImageWidget->width())/2),int((this->height() - showImageWidget->height())/2));
    showImageWidget->hide();

    navigator = new Navigator(this);
    navigator->resize(200,133);
    navigator->move(this->width()-7-navigator->width(),this->height()-52-navigator->height());
    navigator->hide();

    information = new Information(this);
    information->resize(207,197 + 18);
    information->move(this->width()-information->width() +2,Variable::BAR_HEIGHT);
    information->installEventFilter(this);
    information->hide();

    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer_infor = new QTimer(this);
    timer_infor->setSingleShot(true);
    timer_navi = new QTimer(this);
    timer_navi->setSingleShot(true);
//    timer_leave = new QTimer(this);
    // 用户手册功能
    mDaemonIpcDbus = new DaemonDbus();

    this->setMinimumSize(678,678);
    this->_setstyle();
    this->_initconnect();
    this->_openState();
    this->_initGsetting();


}
KyView::~KyView()
{
}
//从顶栏打开
void KyView::menuopen()
{
    emit openSignal();
}

//初始化连接
void KyView::_initconnect()
{   //全屏恢复
    connect(titlebar,SIGNAL(recovery()),this, SLOT(changOrigSize()));
    //打开图片
    connect(openImage,&OpenImage::openImage,showImageWidget,&ShowImageWidget::_startWithOpenImage);
    connect(showImageWidget,&ShowImageWidget::perRate,toolbar,&ToolBar::changePerRate);
    connect(showImageWidget,&ShowImageWidget::ToshowImage,this,&KyView::_Toshowimage);
    //右上菜单--打开
    connect(this,&KyView::openSignal,openImage,&OpenImage::openimage);
    //定时器
    connect(timer ,&QTimer::timeout, this, &KyView::_delayHide);
    connect(timer_infor ,&QTimer::timeout, this, &KyView::_delayHide_infor);
    connect(timer_navi ,&QTimer::timeout, this, &KyView::_delayHide_navi);
    //展示或隐藏图片信息窗口
    connect(toolbar,&ToolBar::showInfor,this,&KyView::_showInforWid);
    //获取图片详细信息
    connect(showImageWidget,&ShowImageWidget::changeInfor,information,&Information::contentText);
    //顶栏图片名字
    connect(showImageWidget,&ShowImageWidget::titleName,titlebar,&TitleBar::showImageName);
    //清空相册时，展示的界面
    connect(showImageWidget,&ShowImageWidget::clearImage,this,&KyView::_clearImage);
    //导航器出现时，位置变化
    connect(navigator,&Navigator::naviChange,this,&KyView::_naviChange);
    //滚轮放大和缩小
    connect(showImageWidget,&ShowImageWidget::reduceChange,toolbar,&ToolBar::reduceImage);
    connect(showImageWidget,&ShowImageWidget::enlargeChange,toolbar,&ToolBar::enlargeImage);
    //监听信息栏鼠标位置，显示顶栏和底栏
    connect(information,&Information::twoBarShow,this,&KyView::_twoBarShow);



}
//打开首先检测是否需要展示工具栏
void KyView::_openState()
{

    if(!openImage->isHidden()){
        toolbar->hide();
        toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height() +4);

    }
}
//顶栏大小随主界面大小改变的响应函数
void KyView::_titlebarChange()
{
    if (titlebar->isHidden()){
        return;
    }else{
        titlebar->move(0,0);
        titlebar->resize(this->width(),Variable::BAR_HEIGHT);
        titlebar->titleWid->resize(this->width(),Variable::BAR_HEIGHT);
    }

}
//中间打开图片按钮大小随主界面大小改变的响应函数
void KyView::_openImageChange()
{
    if (openImage->isHidden()){
        return;
    }else{
        openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));
    }
}
//主界面展示的图片大小随主界面大小改变的响应函数
void KyView::_showImageChange()
{
//    if (showImageWidget->isHidden()){
//        return;
//    }else{
        showImageWidget->move(0,0);
        showImageWidget->resize(this->width(),this->height());
        showImageWidget->re_move(this->width(),this->height());
//    }
}
//工具栏大小随主界面大小改变的响应函数
void KyView::_toolbarChange()
{
    if (toolbar->isHidden()){
        return;
    }else{

        toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height() + 4);
    }
}

void KyView::_naviChange()
{
    if (navigator->isHidden()){
        return;
    }else{

        navigator->move(this->width()-7-navigator->width(),this->height()-52-navigator->height());
    }
}

void KyView::_inforChange()
{
    if (information->isHidden()){
        return;
    }else{
        if(titlebar->isHidden()){
//            timer_infor->start(2000);
            information->move(this->width()-information->width() +2,0);

        }else{
            information->move(this->width()-information->width() +2,Variable::BAR_HEIGHT);
        }

    }
}
//延时隐藏
void KyView::_delayHide()
{
    if(this->mapFromGlobal(QCursor::pos()).y() > Variable::BAR_HEIGHT  && this->mapFromGlobal(QCursor::pos()).y() <this->height()- Variable::BAR_HEIGHT)
    {
        titlebar->hide();
        toolbar->hide();

        _inforChange();
    }
}

void KyView::_delayHide_infor()
{
    information->move(this->width()-information->width() +2,0);
    timer_infor->stop();
}

void KyView::_delayHide_navi()
{
//    if(!navigator->isHidden())
//        navigator->hide();
//    if(hoverState){
        titlebar->hide();
        toolbar->hide();
//    }
    timer_navi->stop();
}

void KyView::_showInforWid()
{
    if(inforState == true){
        information->show();
        _inforChange();
        inforState = false;
    }else{
        information->hide();
        inforState = true;
    }
}

void KyView::_clearImage()
{
    openImage->show();
    showImageWidget->hide();
    toolbar->hide();
    if(titlebar->isHidden()){
        titlebar->show();
        titlebar->imageName->clear();
    }else{
        titlebar->imageName->clear();
        return;
    }
}

void KyView::_hoverChange(int y)
{
    if (y <= Variable::BAR_HEIGHT  || y >= this->height() - Variable::BAR_HEIGHT )
        {
            //判断定时器是否正在计时。如是，则停止
            if(timer->isActive())
                timer->stop();
            if(timer_navi->isActive())
                timer_navi->stop();

            toolbar->show();
            titlebar->show();

            //顶栏和标题栏位置的变化
            _toolbarChange();
            _titlebarChange();

            this->showImageWidget->lower();
            this->showImageWidget->next->hide();
            this->showImageWidget->back->hide();

            //判断具体在顶栏或工具栏的区域，将其raise
            if(y <= Variable::BAR_HEIGHT){
                titlebar->raise();
            }else if(y >= this->height() - Variable::BAR_HEIGHT){
                toolbar->raise();
            }
            //信息栏位置的变化
            _inforChange();
        }else{
            if(!timer->isActive())
                timer->start(2000);
            if(showImageWidget->buttonState == false){
                this->showImageWidget->next->hide();
                this->showImageWidget->back->hide();
            }else{
                this->showImageWidget->next->show();
                this->showImageWidget->back->show();
            }
            }
}

void KyView::_initGsetting()
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this, [=] (const QString &key){
            if (key == "styleName")
                _themeChange();

        });
    }

    if(QGSettings::isSchemaInstalled(FITCONTROLTRANS)){
        m_pGsettingControlTrans = new QGSettings(FITCONTROLTRANS);

    }
//    if(QGSettings::isSchemaInstalled(FITCONTROLTRANS))
//    {
//        m_pGsettingControlTrans = new QGSettings(FITCONTROLTRANS);
//        connect(m_pGsettingControlTrans,&QGSettings::changed, this, [=] (const QString &key){
//            if (key == "transparency")
//                _transChange();
//        });
//    }
    _themeChange();
//    _transChange();
    return;

}

void KyView::_themeChange()
{
    QString themeStyle = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == themeStyle || "ukui-black" == themeStyle)
    {
        information->setStyleSheet("background-color:rgba(0,0,0,0.66);border-radius:4px;");
        toolbar->tooleWid->setStyleSheet("background-color:rgba(0,0,0,0.66);border-radius:2px;");
    }else{
        toolbar->tooleWid->setStyleSheet("background-color:rgba(255,255,255,1);");
        information->setStyleSheet("background-color:rgba(255,255,255,0.66);border-radius:2px;");
    }
}

void KyView::_twoBarShow()
{
    if(titlebar->isHidden()){
        titlebar->show();
        toolbar->show();
    }
}

//void KyView::_transChange()
//{
//    double transRate = m_pGsettingControlTrans->get("transparency").toDouble();
//    this->setWindowOpacity(transRate);
//}

//设置某些控件的QSS
void KyView::_setstyle()
{

    toolbar->setStyleSheet("border-radius:4px;");

    information->setStyleSheet("border-radius:4px;");

}

//全屏和恢复
void KyView::changOrigSize()
{

    if (this->isMaximized()){
        this->showNormal();
        titlebar->fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        titlebar->fullscreen->setToolTip(tr("full srceen"));

    }else{
        this->showMaximized();
        titlebar->fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        titlebar->fullscreen->setToolTip(tr("recovery"));

    }

}

//显示图片
void KyView::_Toshowimage()
{
    this->openImage->hide();
    this->showImageWidget->show();
    if(!titlebar->isHidden())
        this->showImageWidget->lower();
    if(information->isHidden()){
        return;
    }else{
        if(titlebar->isHidden())
        {
            information->move(this->width()-information->width()+2,0);
        }else{
            information->move(this->width()-information->width()+2,Variable::BAR_HEIGHT);
        }
    }

}
//检测鼠标位置--顶栏和工具栏的出现和隐藏
void KyView::mouseMoveEvent(QMouseEvent *event)
{
    int y =this->mapFromGlobal(QCursor().pos()).y();
    if (openImage->isHidden()) {

        _hoverChange(y);
    } else{
        toolbar->hide();
        titlebar->show();
    }
}

void KyView::resizeEvent(QResizeEvent *event){

    _titlebarChange();
    _openImageChange();
    _showImageChange();
    _toolbarChange();
    _naviChange();
    _inforChange();

    if (this->isMaximized()){
        titlebar->fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        titlebar->fullscreen->setToolTip(tr("recovery"));
    }else{
        titlebar->fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        titlebar->fullscreen->setToolTip(tr("full srceen"));
    }

}

void KyView::leaveEvent(QEvent *event)
{

    if(!titlebar->isHidden() && !titlebar->m_menu->m_menu->isHidden()){
        return;
    }
    if(openImage->isHidden()){
        if(!timer_navi->isActive())
            timer_navi->start(2000);
        if(!timer_infor->isActive())
            timer_infor->start(2000);
        showImageWidget->next->hide();
        showImageWidget->back->hide();
    }

    _inforChange();
//    if(navigator->isHidden()){
//        return;
//    }else{
//        timer_navi->start(2000);
    //    }
}

void KyView::enterEvent(QEvent *event)
{
    if(timer_navi->isActive())
        timer_navi->stop();
    if(timer_infor->isActive())
        timer_infor->stop();

}

void KyView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    QPainterPath rectPath;
    rectPath.addRoundedRect(this->rect(), 0, 0); // 左上右下
//    double tran=m_pGsettingControlTrans->get("transparency").toDouble()*255;
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    QColor mainColor;
    if(QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base))
    {
        mainColor = QColor(255, 255, 255,120);
    }else{
        mainColor = QColor(26, 26, 26,200);
    }

    p.fillPath(rectPath,QBrush(mainColor));
}

void KyView::keyPressEvent(QKeyEvent *event)
{
    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!mDaemonIpcDbus->daemonIsNotRunning()){
            //F1快捷键打开用户手册，如kylin-photo-viewer
            //如果是小工具类，下面的showGuide参数要填写"tools/kylin-photo-viewer"
            mDaemonIpcDbus->showGuide("tools/indicator-china-weather");
        }
    }
}
//添加左键双击事件
void KyView::mouseDoubleClickEvent(QMouseEvent *event)
{
    //判断左键双击
    if(event->button() == Qt::LeftButton)
    {
        if (this->isMaximized()){
            this->showNormal();
            titlebar->fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
            titlebar->fullscreen->setToolTip(tr("full srceen"));

        }else{
            this->showMaximized();
            titlebar->fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
            titlebar->fullscreen->setToolTip(tr("recovery"));

        }
    }
}
//检测鼠标在信息栏上方进入，顶栏和工具栏要展示
bool KyView::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == information){
        if(!information->isHidden()){
            if(information->geometry().contains(this->mapFromGlobal(QCursor::pos())))
               {
                    int infor_y = information->mapFromGlobal(QCursor().pos()).y();
                    if (infor_y <= Variable::BAR_HEIGHT)
                    {
                        if(timer_navi->isActive())
                            timer_navi->stop();
                        if(timer_infor->isActive())
                            timer_infor->stop();
                        if(timer->isActive())
                            timer->stop();
                        titlebar->show();
                        toolbar->show();
                        _inforChange();
                    }
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

//拖拽图片
void KyView::dragEnterEvent(QDragEnterEvent *event)
{
    QStringList formatList;
    QString format;
    format = "";
    for(const QString &str:Variable::SUPPORT_FORMATS )
    {
        format = str;
        formatList.append(format);
    }
    QString str = event->mimeData()->urls()[0].fileName();
    if(formatList.contains( QFileInfo(str).suffix()))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();//否则不接受鼠标事件
    }

}

void KyView::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QString path = urls.first().toLocalFile();
    if(path != "")
        emit openImage->openImage(path);
    else
        return;

}
