#include "kyview.h"
#include "xatom-helper.h"
MainWindow *MainWindow::mutual = nullptr;
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    int WIDTH = 1080;
    int HEIGHT = 720;
    this->resize(1080, 720);

    this ->setWindowIcon(QIcon::fromTheme("kylin-recorder", QIcon(":/svg/svg/recording_128.svg")));
    this ->setWindowTitle(tr("kylin-recorder"));
//    this->setMouseTracking(true);
    centerWidget = new QWidget(this);
    centerLayout = new QVBoxLayout(this);

    openImageWidget = new QWidget(this);
//    openImageLayout = new QHBoxLayout(this);

    toolbarWidget = new QWidget(this);
    toolbarLayout = new QHBoxLayout(this);
    //显示在活动屏幕中间新方法
    QScreen *screen = QGuiApplication::primaryScreen();
    this ->move((screen->geometry().width() - WIDTH) / 2,(screen->geometry().height() - HEIGHT) / 2);
    mutual = this;
    //
    titlebar = new TitleBar(this);
    titlebar->resize(this->width(),40);
    titlebar->move(0,0);

    openImage = new OpenImage(this);
    openImage->setFixedSize(128,128+50);
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));

    toolbar = new ToolBar();
    toolbar->setFixedSize(678,40);
    toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());
    qDebug()<<"11"<<toolbar->x()<<toolbar->y();
    toolbar->raise();
    toolbar->show();


    titlebar->show();

    showImage = new QLabel(this);
    showImage->resize(this->width(),this->height());
    showImage->hide();
    this->setMinimumSize(678,678);
    this->setstyle();
    this->initconnect();
    this->layout();
}
MainWindow::~MainWindow()
{
}

//初始化连接
void MainWindow::initconnect()
{
    connect(titlebar,SIGNAL(changeSize()),this, SLOT(changFullScreen()));
    connect(titlebar,SIGNAL(recovery()),this, SLOT(changOrigSize()));


}
//设置某些控件的QSS
void MainWindow::setstyle()
{

//    openImage->setStyleSheet("background-color:red;");
//    openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
//    addFile->setStyleSheet("background-image: url(:/res/res/addplus.png);");
    titlebar->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");
//    toolbar->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");

}
//全屏时改变控件大小
void MainWindow::changFullScreen()
{

    widgetSize = QSize(this->width(),this->height());
    p = this->pos();
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect screenRect =  screen->availableVirtualGeometry();
    this->resize(screenRect.width(),screenRect.height());
    titlebar->re_resize();
    removeForfull();

}
//恢复全屏之前的大小
void MainWindow::changOrigSize()
{
    this->move(p.rx(),p.ry());
    this->resize(widgetSize);
    titlebar->re_resize();
    removeForfull();

}
//全屏时改变控件位置
void MainWindow::removeForfull()
{
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));
    toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());
}
//检测鼠标位置--顶栏和工具栏的出现和隐藏
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    int x =this->mapFromGlobal(QCursor().pos()).x();
    int y =this->mapFromGlobal(QCursor().pos()).y();
    if (openImage->isHidden()) {
        if (y <= 40 || y >= this->height() - 40)
            {

                toolbar->show();
                titlebar->show();
            }else{

                toolbar->hide();
                titlebar->hide();
            }
    } else{
        toolbar->show();//暂时先show
        titlebar->show();
    }



}

void MainWindow::layout()
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
bool MainWindow::event(QEvent *event)
{
//    qDebug()<<event->type();
//    if(event->type() == QEvent::MouseMove){
//        qDebug()<<this->width();
//    }
//    return QObject::event(event);
}
