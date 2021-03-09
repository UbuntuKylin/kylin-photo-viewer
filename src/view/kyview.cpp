#include "kyview.h"
#include "xatom-helper.h"
KyView *KyView::mutual = nullptr;
KyView::KyView(QWidget *parent) : QWidget(parent)
{
    int WIDTH = 1080;
    int HEIGHT = 720;
    this->resize(1080, 720);

//    this ->setWindowIcon(QIcon::fromTheme("kylin-recorder", QIcon(":/svg/svg/recording_128.svg")));
    this ->setWindowIcon(QIcon(":/res/res/kyview_logo.png"));
    this ->setWindowTitle(tr("Kylin Photo Viewer"));
    this->setMouseTracking(true);
//    centerWidget = new QWidget(this);
//    centerLayout = new QVBoxLayout(this);

//    openImageWidget = new QWidget(this);
////    openImageLayout = new QHBoxLayout(this);

//    toolbarWidget = new QWidget(this);
//    toolbarLayout = new QHBoxLayout(this);
    //显示在活动屏幕中间方法
    QScreen *screen = QGuiApplication::primaryScreen();
    this ->move((screen->geometry().width() - WIDTH) / 2,(screen->geometry().height() - HEIGHT) / 2);
    mutual = this;
    //
    titlebar = new TitleBar(this);
//    titlebar->resize(this->width(),Variable::BAR_HEIGHT);
    titlebar->move(0,0);

    openImage = new OpenImage(this);
    openImage->setFixedSize(128,128+50);
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));

    toolbar = new ToolBar(this);
//    toolbar->resize(678,40);
    toolbar->move( int((this->width()-toolbar->width())/2),this->height() - toolbar->height());

    toolbar->show();
    titlebar->show();
    qDebug()<<"sdsdsds"<<this->width()<<this->height();
    showImageWidget = new ShowImageWidget(this,WIDTH,HEIGHT);
//    showImageWidget->resize(WIDTH,HEIGHT);
    showImageWidget->setMouseTracking(true);
    showImageWidget->move(int((this->width() - showImageWidget->width())/2),int((this->height() - showImageWidget->height())/2));
    showImageWidget->hide();

    timer = new QTimer(this);

    this->setMinimumSize(678,678);
    this->_setstyle();
    this->_initconnect();
    this->_layout();
    this->_openState();


}
KyView::~KyView()
{
}
//从顶栏打开
void KyView::menuopen(QString path)
{
    emit openSignal(path);
}

//初始化连接
void KyView::_initconnect()
{   //全屏恢复
    connect(titlebar,SIGNAL(changeSize()),this, SLOT(changFullScreen()));
    connect(titlebar,SIGNAL(recovery()),this, SLOT(changOrigSize()));
    //打开图片
    connect(openImage,&OpenImage::openImage,showImageWidget,&ShowImageWidget::_startWithOpenImage);
    connect(showImageWidget,&ShowImageWidget::perRate,toolbar,&ToolBar::changePerRate);
    connect(showImageWidget,&ShowImageWidget::ToshowImage,this,&KyView::_Toshowimage);
    //右上菜单--打开
    connect(this,&KyView::openSignal,showImageWidget,&ShowImageWidget::_startWithOpenImage);
    //定时器
    connect(timer ,&QTimer::timeout, this, &KyView::_delayHide);


}
//打开首先检测是否需要展示工具栏
void KyView::_openState()
{

    if(!openImage->isHidden()){
        toolbar->hide();
        toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());
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

        toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());

    }
}
//延时隐藏
void KyView::_delayHide()
{
    titlebar->hide();
    toolbar->hide();
}

//设置某些控件的QSS
void KyView::_setstyle()
{

//    openImage->setStyleSheet("background-color:red;");
//    openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
//    addFile->setStyleSheet("background-image: url(:/res/res/addplus.png);");
    titlebar->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");
    toolbar->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");

}
//全屏
void KyView::changFullScreen()
{

    this->showFullScreen();

}
//恢复全屏之前的大小
void KyView::changOrigSize()
{

    this->showNormal();

}

//显示图片
void KyView::_Toshowimage()
{
    this->titlebar->hide();
    this->openImage->hide();
    this->showImageWidget->show();

}
//检测鼠标位置--顶栏和工具栏的出现和隐藏
void KyView::mouseMoveEvent(QMouseEvent *event)
{

    int y =this->mapFromGlobal(QCursor().pos()).y();
    if (openImage->isHidden()) {
        if (y <= Variable::BAR_HEIGHT || y >= this->height() - Variable::BAR_HEIGHT)
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
            }else{
//                timer->start(2000); //开启延时隐藏
//                qDebug()<<"aa"<<titlebar->m_menu->m_menu->windowState();
                toolbar->hide();
                titlebar->hide();
                this->showImageWidget->next->show();
                this->showImageWidget->back->show();

            }
    } else{
        toolbar->hide();
        titlebar->show();
    }



}


void KyView::_layout()
{
//    openImageLayout->addStretch();
//    openImageLayout->addWidget(openImage);
//    openImageLayout->addStretch();
//    openImageWid->setLayout(openImageLayout);
//    openImageWid->resize(this->width(),this->height());
//    openImageWid->move(0,0);

//    showImageLayout->addStretch();
//    showImageLayout->addWidget(showImageWidget);
//    showImageLayout->addStretch();
//    showImageWid->setLayout(showImageLayout);
//    showImageWid->resize(this->width(),this->height());
//    showImageWid->move(0,0);

}

void KyView::resizeEvent(QResizeEvent *event){

    _titlebarChange();
    _openImageChange();
    _showImageChange();
    _toolbarChange();

}

void KyView::leaveEvent(QEvent *event)
{
    if(openImage->isHidden()){

        titlebar->hide();
        toolbar->hide();
        showImageWidget->next->hide();
        showImageWidget->back->hide();
    }

}
