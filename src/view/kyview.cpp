#include "kyview.h"
#include "xatom-helper.h"
#include "sizedate.h"
KyView *KyView::mutual = nullptr;
KyView::KyView(const QStringList &args)
{
    Interaction *m_interaction =Interaction::getInstance();
    m_interaction->creatCore(args);

    this->resize(SizeDate::DEFAULTWIDTH, SizeDate::DEFAULTHEIGHT);

    this ->setWindowIcon(QIcon(":/res/res/kyview_logo.png"));
    this ->setWindowTitle(tr("Kylin Photo Viewer"));
    //毛玻璃
    this->setProperty("useSystemStyleBlur",true);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setMouseTracking(true);
    //响应拖拽事件
    this->setAcceptDrops(true);

    mutual = this;

    QScreen *screen = QGuiApplication::primaryScreen();
    this ->move((screen->geometry().width() - SizeDate::DEFAULTWIDTH) / 2,(screen->geometry().height() - SizeDate::DEFAULTHEIGHT) / 2);

    //标题栏
    m_titlebar = new TitleBar(this);
    m_titlebar->setAutoFillBackground(true);
    m_titlebar->setBackgroundRole(QPalette::Base);
    m_titlebar->setWindowOpacity(0.7);
    m_titlebar->move(0,0);
    m_titlebar->show();

    //默认打开--打开图片按钮widget
    m_openImage = new OpenImage(this);
    m_openImage->setFixedSize(SizeDate::OPENIMAGESIZE);
    m_openImage->move(int((this->width()-m_openImage->width())/2),int((this->height()-m_openImage->height())/2));

    //工具栏
    m_toolbar = new ToolBar(this);
    m_toolbar->move( int((this->width()-m_toolbar->width())/2),this->height() - m_toolbar->height() + 4);
    m_toolbar->show();

    //图片展示界面
    m_showImageWidget = new ShowImageWidget(this,SizeDate::DEFAULTWIDTH,SizeDate::DEFAULTHEIGHT);
    m_showImageWidget->setMouseTracking(true);
    m_showImageWidget->move(int((this->width() - m_showImageWidget->width())/2),int((this->height() - m_showImageWidget->height())/2));
    m_showImageWidget->hide();

    //导航器
    m_navigator = new Navigator(this);
//    m_navigator->resize(SizeDate::NAVISIZE);
    m_navigator->move(this->width()-7-m_navigator->width(),this->height()-52-m_navigator->height());
    m_navigator->hide();

    //信息栏
    m_information = new Information(this);
    m_information->resize(SizeDate::INFORSIZE);
    m_information->move(this->width()-m_information->width() +2,SizeDate::BARHEIGHT);
    m_information->installEventFilter(this);
    m_information->hide();

    //定时器
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timerinfor = new QTimer(this);
    m_timerinfor->setSingleShot(true);
    m_timernavi = new QTimer(this);
    m_timernavi->setSingleShot(true);

    // 用户手册功能
    m_DaemonIpcDbus = new DaemonDbus();

    this->setMinimumSize(SizeDate::MINISIZE);
    this->initconnect();
    //要判断打开状态，是否隐藏主界面--打开按钮widget
    this->openState();
    this->initGsetting();

}
KyView::~KyView()
{
}


//信号和槽
void KyView::initconnect()
{   //全屏恢复
    connect(m_titlebar,&TitleBar::recovery,this,&KyView::changOrigSize);
    //右上菜单--打开
    connect(m_titlebar,&TitleBar::openSignal,m_openImage,&OpenImage::openimage);
    //打开关于，两栏隐藏
    connect(m_titlebar,&TitleBar::aboutShow,this,&KyView::aboutShow);

    //打开图片
    connect(m_openImage,&OpenImage::openImage,m_showImageWidget,&ShowImageWidget::openImage);

    //改变顶栏显示的图片名字
    connect(m_showImageWidget,&ShowImageWidget::perRate,m_toolbar,&ToolBar::changePerRate);
    //打开图片--主界面的界面显示处理
    connect(m_showImageWidget,&ShowImageWidget::ToshowImage,this,&KyView::Toshowimage);
    //获取图片详细信息
    connect(m_showImageWidget,&ShowImageWidget::changeInfor,m_information,&Information::contentText);
    //顶栏图片名字
    connect(m_showImageWidget,&ShowImageWidget::titleName,m_titlebar,&TitleBar::showImageName);
    //清空相册时，展示的界面
    connect(m_showImageWidget,&ShowImageWidget::clearImage,this,&KyView::clearImage);
    //滚轮放大和缩小
    connect(m_showImageWidget,&ShowImageWidget::reduceChange,m_toolbar,&ToolBar::reduceImage);
    connect(m_showImageWidget,&ShowImageWidget::enlargeChange,m_toolbar,&ToolBar::enlargeImage);

    //展示或隐藏图片信息窗口
    connect(m_toolbar,&ToolBar::showInfor,this,&KyView::showInforWid);
    //导航器出现时，位置变化
    connect(m_navigator,&Navigator::naviChange,this,&KyView::naviChange);

    //定时器
    connect(m_timer ,&QTimer::timeout, this, &KyView::delayHide);
    connect(m_timerinfor ,&QTimer::timeout, this, &KyView::delayHide_infor);
    connect(m_timernavi ,&QTimer::timeout, this, &KyView::delayHide_navi);
}
//打开首先检测是否需要展示工具栏
void KyView::openState()
{
    if(!m_openImage->isHidden()){
        m_toolbar->hide();
        m_toolbar->move(int((this->width()-m_toolbar->width())/2),this->height() - m_toolbar->height() +4);
    }
}
//打卡关于，两栏隐藏
void KyView::aboutShow()
{
    if(m_openImage->isHidden())
    {
        m_titlebar->hide();
        m_toolbar->hide();
        m_information->move(this->width()-m_information->width() +2,0);
    }
}
//顶栏大小随主界面大小改变的响应函数
void KyView::titlebarChange()
{
    if (m_titlebar->isHidden())
    {
        return;
    }else{
        m_titlebar->move(0,0);
        m_titlebar->resize(this->width(),SizeDate::BARHEIGHT);
        m_titlebar->g_titleWid->resize(this->width(),SizeDate::BARHEIGHT);
    }

}
//中间打开图片按钮大小随主界面大小改变的响应函数
void KyView::openImageChange()
{
    if (m_openImage->isHidden())
    {
        return;
    }else{
        m_openImage->move(int((this->width()-m_openImage->width())/2),int((this->height()-m_openImage->height())/2));
    }
}
//主界面展示的图片大小随主界面大小改变的响应函数
void KyView::showImageChange()
{
//    if (m_showImageWidget->isHidden()){
//        return;
//    }else{
        m_showImageWidget->move(0,0);
        m_showImageWidget->resize(this->width(),this->height());
        m_showImageWidget->re_move(this->width(),this->height());
//    }
}
//工具栏大小随主界面大小改变的响应函数
void KyView::toolbarChange()
{
    if (m_toolbar->isHidden()){
        return;
    }else{

        m_toolbar->move(int((this->width()-m_toolbar->width())/2),this->height() - m_toolbar->height() + 4);
    }
}
//导航栏位置变化
void KyView::naviChange()
{
    if (m_navigator->isHidden()){
        return;
    }else{

        m_navigator->move(this->width()-7-m_navigator->width(),this->height()-52-m_navigator->height());
    }
}
//信息栏随顶栏位置变化而变化
void KyView::inforChange()
{
    if (m_information->isHidden()){
        return;
    }else{
        if(m_titlebar->isHidden()){
//            timer_infor->start(2000);
            m_information->move(this->width()-m_information->width() +2,0);

        }else{
            m_information->move(this->width()-m_information->width() +2,SizeDate::BARHEIGHT);
        }

    }
}
//延时隐藏
void KyView::delayHide()
{
    if(this->mapFromGlobal(QCursor::pos()).y() > SizeDate::BARHEIGHT  && this->mapFromGlobal(QCursor::pos()).y() <this->height()- SizeDate::BARHEIGHT)
    {
        m_titlebar->hide();
        m_toolbar->hide();
        inforChange();
    }
}

void KyView::delayHide_infor()
{
    m_information->move(this->width()-m_information->width() +2,0);
    m_timerinfor->stop();
}
//鼠标离开界面时需要触发，届时会加上对导航器的处理
void KyView::delayHide_navi()
{

    m_titlebar->hide();
    m_toolbar->hide();
    m_timernavi->stop();
}
//展示信息栏
void KyView::showInforWid()
{
    if(m_inforState == true){
        m_information->show();
        m_inforState = false;
        inforChange();
    }else{
        m_information->hide();
        m_inforState = true;
    }
}
//列表为空-清空图片--回到默认的打开界面
void KyView::clearImage()
{
    m_openImage->show();
    m_showImageWidget->hide();
    m_toolbar->hide();
    m_information->hide();
    if(m_titlebar->isHidden()){
        m_titlebar->show();
        m_titlebar->g_imageName->clear();
    }else{
        m_titlebar->g_imageName->clear();
        return;
    }
}
//处理鼠标悬浮两栏的界面展示
void KyView::hoverChange(int y)
{
    if (y <= SizeDate::BARHEIGHT  || y >= this->height() - SizeDate::BARHEIGHT )
        {
            //判断定时器是否正在计时。如是，则停止
            if(m_timer->isActive())
            {
                m_timer->stop();
            }
            if(m_timernavi->isActive())
            {
                m_timernavi->stop();
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
            if(y <= SizeDate::BARHEIGHT)
            {
                m_titlebar->raise();
            }else if(y >= this->height() - SizeDate::BARHEIGHT){
                m_toolbar->raise();
            }
            //信息栏位置的变化
            inforChange();
        }else{
            if(!m_timer->isActive())
            {
                m_timer->start(2000);
            }
            if(m_showImageWidget->g_buttonState == false)
            {
                this->m_showImageWidget->g_next->hide();
                this->m_showImageWidget->g_back->hide();
            }else{
                this->m_showImageWidget->g_next->show();
                this->m_showImageWidget->g_back->show();
            }
            }
}
//读取主题配置文件
void KyView::initGsetting()
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this, [=] (const QString &key){
            if (key == "styleName")
            {
                themeChange();
            }

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
//                transChange();
//        });
//    }
    themeChange();
//    transChange();
    return;

}

void KyView::themeChange()
{
    QString themeStyle = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == themeStyle || "ukui-black" == themeStyle)
    {
        m_information->setStyleSheet("background-color:rgba(0,0,0,0.66);border-radius:4px;");
        m_titlebar->g_menu->setThemeDark();
    }else{
        m_information->setStyleSheet("background-color:rgba(255,255,255,0.66);border-radius:4px;");
        m_titlebar->g_menu->setThemeLight();
    }
     m_toolbar->changeStyle();

}


//void KyView::transChange()
//{
//    double transRate = m_pGsettingControlTrans->get("transparency").toDouble();
//    this->setWindowOpacity(transRate);
//}

//最大化和还原
void KyView::changOrigSize()
{

    if (this->isMaximized()){
        this->showNormal();
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        m_titlebar->g_fullscreen->setToolTip(tr("full srceen"));
    }else{
        this->showMaximized();
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        m_titlebar->g_fullscreen->setToolTip(tr("recovery"));
    }
}

//显示图片--主界面的界面显示
void KyView::Toshowimage()
{
    this->m_openImage->hide();
    this->m_showImageWidget->show();
    if(!m_titlebar->isHidden()){
        this->m_showImageWidget->lower();
    }
    inforChange();
}
//检测鼠标位置--顶栏和工具栏的出现和隐藏
void KyView::mouseMoveEvent(QMouseEvent *event)
{
    int y =this->mapFromGlobal(QCursor().pos()).y();
    if (m_openImage->isHidden()) {

        hoverChange(y);
    } else{
        m_toolbar->hide();
        m_titlebar->show();
    }
}
//拖拽主窗口--各控件需要改变位置或尺寸
void KyView::resizeEvent(QResizeEvent *event){

    titlebarChange();
    openImageChange();
    showImageChange();
    toolbarChange();
    naviChange();
    inforChange();

    if (this->isMaximized()){
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
        m_titlebar->g_fullscreen->setToolTip(tr("recovery"));
    }else{
        m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
        m_titlebar->g_fullscreen->setToolTip(tr("full srceen"));
    }

}
//鼠标离开事件
void KyView::leaveEvent(QEvent *event)
{

    if(!m_titlebar->isHidden() && !m_titlebar->g_menu->m_menu->isHidden()){
        return;
    }
    if(m_openImage->isHidden()){
        if(!m_timernavi->isActive())
        {
            m_timernavi->start(2000);
        }
        if(!m_timerinfor->isActive())
        {
            m_timerinfor->start(2000);
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
    if(m_timernavi->isActive())
    {
        m_timernavi->stop();
    }
    if(m_timerinfor->isActive())
    {
        m_timerinfor->stop();
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
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    painter.setBrush(opt.palette.color(QPalette::Base));

    QColor mainColor;
    if(QColor(255,255,255) == opt.palette.color(QPalette::Base) || QColor(248,248,248) == opt.palette.color(QPalette::Base))
    {
        mainColor = QColor(254, 254, 254,155);
    }else{
        mainColor = QColor(26, 26, 26,200);
    }

    p.fillPath(rectPath,QBrush(mainColor));
}
// F1快捷键打开用户手册
void KyView::keyPressEvent(QKeyEvent *event)
{

    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!m_DaemonIpcDbus->daemonIsNotRunning())
        {
            //F1快捷键打开用户手册，如kylin-photo-viewer
            //如果是小工具类，下面的showGuide参数要填写"tools/kylin-photo-viewer"
            m_DaemonIpcDbus->showGuide(SizeDate::USERGUIDE);
        }
    }
    //上一张，下一张，delete按键响应
    if(!m_openImage->isHidden())
    {
        return;
    }else{
        if(event->key() == Qt::Key_Delete)
        {
            m_toolbar->delImage();
        }
        if(event->key() == Qt::Key_Left)
        {
            m_showImageWidget->backImage();
        }
        if(event->key() == Qt::Key_Right)
        {
            m_showImageWidget->nextImage();
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
            m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-maximize-symbolic"));//主题库的全屏图标
            m_titlebar->g_fullscreen->setToolTip(tr("full srceen"));

        }else{
            this->showMaximized();
            m_titlebar->g_fullscreen->setIcon(QIcon::fromTheme("window-restore-symbolic"));//主题库的恢复图标
            m_titlebar->g_fullscreen->setToolTip(tr("recovery"));

        }
    }
}
//检测鼠标在信息栏上方进入，顶栏和工具栏要展示
bool KyView::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_information){
        if(!m_information->isHidden()){
            if(m_information->geometry().contains(this->mapFromGlobal(QCursor::pos())))
               {
                    int infor_y = m_information->mapFromGlobal(QCursor().pos()).y();
                    if (infor_y <= SizeDate::BARHEIGHT)
                    {
                        if(m_timernavi->isActive()){
                            m_timernavi->stop();
                        }
                        if(m_timerinfor->isActive()){
                            m_timerinfor->stop();
                        }
                        if(m_timer->isActive()){
                            m_timer->stop();
                        }
                        m_titlebar->show();
                        m_toolbar->show();
                        inforChange();
                    }
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

//拖文件进界面
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
//放下文件之后响应
void KyView::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
    {
        return;
    }

    QString path = urls.first().toLocalFile();
    if(path != "")
    {
        emit m_openImage->openImage(path);
    }
    else{
        return;
    }

}
