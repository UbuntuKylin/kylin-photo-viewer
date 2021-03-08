#include "kyview.h"
#include "xatom-helper.h"
KyView *KyView::mutual = nullptr;
KyView::KyView(QWidget *parent) : QWidget(parent)
{
    int WIDTH = 1080;
    int HEIGHT = 720;
    this->resize(1080, 720);

    this ->setWindowIcon(QIcon::fromTheme("kylin-recorder", QIcon(":/svg/svg/recording_128.svg")));
    this ->setWindowTitle(tr("kylin-recorder"));
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
    titlebar->resize(this->width(),Variable::BAR_HEIGHT);
    titlebar->move(0,0);

    openImage = new OpenImage(this);
    openImage->setFixedSize(128,128+50);
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));

    toolbar = new ToolBar(this);
//    toolbar->resize(678,40);
    toolbar->move( int((this->width()-toolbar->width())/2),this->height() - toolbar->height());

    toolbar->show();
    titlebar->show();

    showImageWidget = new ShowImageWidget(this,this->width(),this->height());
    showImageWidget->resize(this->width(),this->height());
    showImageWidget->setMouseTracking(true);
    showImageWidget->move(int((this->width() - showImageWidget->width())/2),int((this->height() - showImageWidget->height())/2));
    showImageWidget->hide();

    this->setMinimumSize(678,678);
    this->_setstyle();
    this->_initconnect();
    this->layout();
    this->_openState();


}
KyView::~KyView()
{
}

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


}

void KyView::_openState()
{

    if(!openImage->isHidden()){
        toolbar->hide();
        toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());
    }
}

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

void KyView::_openImageChange()
{
    if (openImage->isHidden()){
        return;
    }else{
        openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));
    }
}

void KyView::_showImageChange()
{
    if (showImageWidget->isHidden()){
        return;
    }else{
        showImageWidget->move(0,0);
        showImageWidget->resize(this->width(),this->height());
        showImageWidget->re_move(showImageWidget->width(),showImageWidget->height());

    }
}

void KyView::_toolbarChange()
{
    if (toolbar->isHidden()){
        return;
    }else{

        toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());

    }
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
//全屏时改变控件大小
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


void KyView::layout()
{
//    openImageLayout->addStretch();
//    openImageLayout->addWidget(openImage);
//    openImageLayout->addStretch();
//    openImageWidget->setLayout(openImageLayout);

//    toolbarLayout->addStretch();
//    toolbarLayout->addWidget(toolbar);
//    toolbarLayout->addStretch();
//    toolbarWidget->setLayout(toolbarLayout);


//    centerLayout->addWidget(titlebar);
//    openImageLayout->addStretch();
//    centerLayout->addWidget(openImage);
//    openImageLayout->addStretch();
//    centerLayout->addWidget(toolbar);
//    centerWidget->setLayout(centerLayout);
//    centerWidget->resize(1080,720);
//    centerWidget->move(0,0);

}

void KyView::resizeEvent(QResizeEvent *event){

    _titlebarChange();
    _openImageChange();
    _showImageChange();
    _toolbarChange();


}
