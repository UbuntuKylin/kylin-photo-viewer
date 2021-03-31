#include "showimagewidget.h"
#include "kyview.h"
ShowImageWidget::ShowImageWidget(QWidget *parent, int w, int h) : QWidget(parent)
{
    this->resize(w,h);
//    imageWid = new QWidget(this);
//    imageLayout = new QHBoxLayout(this);

    showImage = new QLabel(this);
    showImage->resize(this->width(),this->height());
    showImage->move(int((this->width() - showImage->width())/2),int((this->height() - showImage->height())/2));
    showImage->setMouseTracking(true);
    showImage->installEventFilter(this);
    showImage->setAlignment(Qt::AlignCenter);
    showImage->setContextMenuPolicy(Qt::ActionsContextMenu);
    showImage->setContextMenuPolicy(Qt::CustomContextMenu);

//    imageLayout->addWidget(showImage);
//    imageWid->setLayout(imageLayout);
//    imageWid->resize(this->width(),this->height());
//    imageWid->move(int((this->width() - imageWid->width())/2),int((this->height() - imageWid->height())/2));
//    imageWid->setMouseTracking(true);


    copy = new QAction(tr("Copy"),this);
    setDeskPaper = new QAction(tr("Set Desktop Wallpaper"),this);
    setLockPaper = new QAction(tr("Set Lock Wallpaper"),this);
    print = new QAction(tr("Print"),this);
    deleteImage = new QAction(tr("Delete"),this);
    showInFile = new QAction(tr("Show in File"),this);
    imageMenu = new QMenu(this);
//    imageMenu->addAction(copy);
//    imageMenu->addAction(copy);
//    imageMenu->addAction(setDeskPaper);
//    imageMenu->addAction(setLockPaper);
//    imageMenu->addAction(print);
    imageMenu->addAction(deleteImage);
    imageMenu->addAction(showInFile);

    next = new QPushButton(this);
    next->resize(56,56);
    next->setIcon(QIcon(":/res/res/right.png"));
    back = new QPushButton(this);
    back->resize(56,56);
    back->setIcon(QIcon(":/res/res/left.png"));

    back->move(43,int((this->height() - back->height())/2));
    next->move(this->width() - 43 - next->width(),int((this->height() - next->height())/2));

    iconSize = QSize(56,56);
    back->setIconSize(iconSize);
    next->setIconSize(iconSize);

    back->setFocusPolicy(Qt::NoFocus);
    next->setFocusPolicy(Qt::NoFocus);
    back->setStyleSheet("background-color:transparent;border-radius:4px;");
    next->setStyleSheet("background-color:transparent;border-radius:4px;");

    this->_initConnect();
    _initInteraction();//一定要放到构造函数末尾
}

void ShowImageWidget::_initConnect()
{
    connect(next,&QPushButton::clicked,this,&ShowImageWidget::_nextImage);
    connect(back,&QPushButton::clicked,this,&ShowImageWidget::_backImage);
    //设置壁纸
    connect(showImage,&QLabel::customContextMenuRequested,[=](const QPoint &pos){
       Q_UNUSED(pos);
       imageMenu->exec(QCursor::pos());
    });
//    connect(copy, &QAction::triggered, this,&ShowImageWidget::_copy);
    connect(setDeskPaper, &QAction::triggered, this,&ShowImageWidget::_setDeskPaper);
//    connect(setLockPaper, &QAction::triggered, this,&ShowImageWidget::_setLockPaper);
//    connect(print, &QAction::triggered, this,&ShowImageWidget::_print);
    connect(deleteImage, &QAction::triggered, this,&ShowImageWidget::_deleteImage);
    connect(showInFile, &QAction::triggered, this,&ShowImageWidget::_showInFile);

}
//下一张
void ShowImageWidget::_nextImage()
{
    canSet = true;
    interaction->nextImage();
}
//上一张
void ShowImageWidget::_backImage()
{
    canSet = true;
    interaction->backImage();
}

void ShowImageWidget::_copy()
{
//    qDebug()<<"复制";
}

void ShowImageWidget::_setDeskPaper()
{
    interaction->setAsBackground();//设置为桌面壁纸
}

void ShowImageWidget::_setLockPaper()
{
    qDebug()<<"设置为锁屏壁纸";
}

void ShowImageWidget::_print()
{
    qDebug()<<"打印";
}

void ShowImageWidget::_deleteImage()
{
    qDebug()<<"删除";
    interaction->deleteImage();
}

void ShowImageWidget::_showInFile()
{

    if (path == "")
        return;
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
//显示右键菜单栏
void ShowImageWidget::m_setMenuAction()
{   //拿到可设置为壁纸的图片格式

    QStringList formatList;
    QString format;
    format = "";
    for(const QString &str:Variable::BACKGROUND_SUPPORT_FORMATS )
    {
        format = str;
        formatList.append(format);
    }
    //判断是否为可设置为壁纸的类型
    if(formatList.contains(paperFormat))
    {
      imageMenu->insertAction(deleteImage,setDeskPaper);
    }else{
        imageMenu->removeAction(setDeskPaper);
    }

}

void ShowImageWidget::_processFinish(bool isFinish)
{
    if(isFinish)
        qDebug()<<"finish"<<isFinish;
    else
        qDebug()<<"not finish"<<isFinish;
}

void ShowImageWidget::_initInteraction()
{
    interaction =Interaction::getInstance();
    //此处绑定信号和槽
    connect(interaction,&Interaction::startWithOpenImage,this,&ShowImageWidget::_startWithOpenImage);//启动时打开图片
    connect(interaction,&Interaction::openFinish,this,&ShowImageWidget::openFinish);//图片打开完成，获取数据
    connect(interaction,&Interaction::albumFinish,this,&ShowImageWidget::albumFinish);//相册缩略图打开完成，获取数据
    interaction->initUiFinish();
}
void ShowImageWidget::_startWithOpenImage(QString path)
{
    _openImage(path);
}

void ShowImageWidget::_openImage(QString path)
{
    QVariant var= interaction->openImage(path);
    //相册列表，需要得到图片的数量
    QList<int> list = var.value<QList<int>>();
    //小于2张，左右按钮隐藏
    if(list.length() < 2){
            //主界面需要知道是否只有一张图片来处理hover态显示。
            buttonState = false;
        }else{
            buttonState = true;
        }
}
//拿到图片信息，进行处理
void ShowImageWidget::openFinish(QVariant var)
{

    ImageAndInfo package =var.value<ImageAndInfo>();
    int type = package.type;//在队列中的标签
    //判断有几张图片，分别进行处理：删除到0，显示打开界面；只有一张：不显示左右按钮。
    if(type == 0){
        emit clearImage();
        return;
    }
    //拿到返回信息
    QFileInfo info = package.info;//详情信息
    QPixmap pixmap = package.image;//图片
    int proportion = package.proportion;//比例
    QString imageSize = package.imageSize;
    QString colorSpace = package.colorSpace;
    QString num;
    num = QString("%1").arg(proportion) + "%";
    path = info.absolutePath();//图片的路径
    copyImage = pixmap;//留着复制可能用
    paperFormat = info.suffix();

    //使用返回的信息进行设置界面
    this->showImage->setPixmap(pixmap);
    emit perRate(num);//发送给toolbar来更改缩放数字
    emit ToshowImage();//给主界面--展示图片
    emit changeInfor(info,imageSize,colorSpace);//给信息栏需要的信息
    emit titleName(info.fileName());//给顶栏图片的名字
    //设置壁纸--动图在传来时是一帧一帧，防止多次添加右键菜单动作
    if(canSet)
    {
        canSet = false;
        m_setMenuAction();

    }


}

void ShowImageWidget::re_move(int W, int H)
{
    //拉伸主界面时重新安排界面显示
    this->resize(W,H);
    this->showImage->resize(W,H);
    this->showImage->move(int((W - this->showImage->width())/2),int((H - this->showImage->height())/2));
    back->move(43,int((this->height() - back->height())/2));
    next->move(W - 43 - next->width(),int((H - next->height())/2));
}

void ShowImageWidget::albumFinish(QVariant var)
{
    ImageAndInfo package =var.value<ImageAndInfo>();
    QFileInfo info = package.info;//详情信息
    QPixmap pixmap = package.image;//图片
    int type = package.type;//在队列中的标签
//    qDebug()<<pixmap<<info<<type;
}
void ShowImageWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    this->showImage->resize(KyView::mutual->width(),KyView::mutual->height());
    interaction->changeWidgetSize(this->showImage->size());
}
bool ShowImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    //滚轮进行放大缩小图片
    if(obj == showImage)
    {
        if(event->type()==QEvent::Wheel)
        {
            QWheelEvent *wheelEvent=static_cast<QWheelEvent *>(event);
            if(wheelEvent->delta()>0)
            {
                emit enlargeChange();
            }
            else
            {
                emit reduceChange();
            }
        }
    }

    return QObject::eventFilter(obj,event);
}


