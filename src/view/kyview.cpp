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

    //显示在活动屏幕中间新方法
    QScreen *screen = QGuiApplication::primaryScreen();
    this ->move((screen->geometry().width() - WIDTH) / 2,(screen->geometry().height() - HEIGHT) / 2);
    mutual = this;

    openImage = new OpenImage(this);
    openImage->setFixedSize(128,128+50);
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));


    titlebar = new TitleBar(this);
    titlebar->resize(WIDTH,40);
    titlebar->move(0,0);

    toolbar = new ToolBar();
    toolbar->setFixedSize(678,40);
    toolbar->move(int((this->width()-toolbar->width())/2),this->height() - toolbar->height());

    toolbar->show();
    titlebar->show();



    this->setStyle();
    this->initconnect();
}
MainWindow::~MainWindow()
{
}
void MainWindow::initconnect()
{
    connect(titlebar,SIGNAL(changeSize()),this, SLOT(changSize()));
    connect(toolbar,SIGNAL(showTwobar()),this,SLOT(showTwobar()));
    connect(toolbar,SIGNAL(hideTwobar()),this,SLOT(hideTwobar()));

}

void MainWindow::setStyle()
{

//    openImage->setStyleSheet("background-color:red;");
//    openInCenter->setStyleSheet("background-color:rgba(255, 255, 255, 1);border-radius:64;");
//    addFile->setStyleSheet("background-image: url(:/res/res/addplus.png);");
    titlebar->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");
//    toolbar->setStyleSheet("background-color:rgba(255, 255, 255, 0.7);");

}
void MainWindow::changSize()
{

    titlebar->titleWid->setFixedWidth(this->width());
    removeForfull();

}
void MainWindow::removeForfull()
{
    openImage->move(int((this->width()-openImage->width())/2),int((this->height()-openImage->height())/2));
}
void MainWindow::showTwobar(){

    qDebug() << "enter";
    toolbar->show();
    titlebar->show();
}
void MainWindow::hideTwobar(){
    qDebug() << "leave";
    toolbar->hide();
    titlebar->hide();
}

