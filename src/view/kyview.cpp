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

    const QByteArray transparency_id(FITCONTROLTRANS);
    if(QGSettings::isSchemaInstalled(transparency_id)){
        m_pGsettingControlTrans = new QGSettings(transparency_id);

    }

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
    information->setMouseTracking(true);
    information->hide();

    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer_infor = new QTimer(this);
    timer_infor->setSingleShot(true);
    timer_navi = new QTimer(this);
    timer_navi->setSingleShot(true);
//    timer_leave = new QTimer(this);

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
    //右键菜单删除当前图片
//    connect(showImageWidget,&ShowImageWidget::deleteCurrImage,toolbar,&ToolBar::delImage);
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
    if(this->mapFromGlobal(QCursor::pos()).y() > 40  && this->mapFromGlobal(QCursor::pos()).y() <this->height()- 40)
    {
        titlebar->hide();
        toolbar->hide();
        if(information->isHidden()){
            return;
        }else{
           information->move(this->width()-information->width() +2,0);
    }
    }
}

void KyView::_delayHide_infor()
{
    information->move(this->width()-information->width() +2,0);
    timer_infor->stop();
}

void KyView::_delayHide_navi()
{
    if(!navigator->isHidden())
        navigator->hide();
    titlebar->hide();
    toolbar->hide();
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
            toolbar->show();
            titlebar->show();

            _toolbarChange();
            _titlebarChange();
            this->showImageWidget->lower();
            this->showImageWidget->next->hide();
            this->showImageWidget->back->hide();
            if(y <= Variable::BAR_HEIGHT){
                titlebar->raise();
            }else if(y >= this->height() - Variable::BAR_HEIGHT){
                toolbar->raise();
            }
            if(information->isHidden()){
                return;
            }else{
               information->move(this->width()-information->width()+2,Variable::BAR_HEIGHT);
            }
        }else{
            if(!timer->isActive())
                timer->start(2000);
//            toolbar->hide();
//            titlebar->hide();
//            QTime _Timer = QTime::currentTime().addMSecs(2000);
//            while( QTime::currentTime() < _Timer );
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
    if(information->isHidden()){
        return;
    }else{
       information->move(this->width()-information->width()+2,0);
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

        timer_navi->start(2000);
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

void KyView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    QPainterPath rectPath;
    rectPath.addRoundedRect(this->rect(), 0, 0); // 左上右下
    double tran=m_pGsettingControlTrans->get("transparency").toDouble()*255;
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    QColor mainColor;
    if(QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base))
    {
        mainColor = QColor(255, 255, 255,200);
    }else{
        mainColor = QColor(26, 26, 26,200);
    }

    p.fillPath(rectPath,QBrush(mainColor));
}

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
    if(formatList.contains( event->mimeData()->urls()[0].fileName().right(3)))
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
