#include "showimagewidget.h"
#include "kyview.h"
ShowImageWidget::ShowImageWidget(QWidget *parent, int w, int h) : QWidget(parent)
{
    this->resize(w,h);
    imageWid = new QWidget(this);
    imageLayout = new QHBoxLayout(this);

    showImage = new QLabel(this);
    showImage->resize(this->width(),this->height());
//    showImage->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    showImage->move(int((this->width() - showImage->width())/2),int((this->height() - showImage->height())/2));
    showImage->setMouseTracking(true);
    showImage->setAlignment(Qt::AlignCenter);

    showImage->setContextMenuPolicy(Qt::ActionsContextMenu);
    showImage->setContextMenuPolicy(Qt::CustomContextMenu);

//    imageLayout->addWidget(showImage);
//    imageWid->setLayout(imageLayout);
//    imageWid->resize(this->width(),this->height());
//    imageWid->move(int((this->width() - imageWid->width())/2),int((this->height() - imageWid->height())/2));
//    imageWid->setMouseTracking(true);


    action_wallpaper = new QAction(tr("set wallpaper"),this);
    imageMenu = new QMenu(this);
    imageMenu->addAction(action_wallpaper);

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
    this->_initConnect();
    _initInteraction();//一定要放到构造函数末尾
}

void ShowImageWidget::_initConnect()
{
    connect(next,&QPushButton::clicked,this,&ShowImageWidget::_nextImage);
    connect(back,&QPushButton::clicked,this,&ShowImageWidget::_backImage);
    //设置壁纸
    connect(showImage,&QLabel::customContextMenuRequested,[=](const QPoint &pos)
    {
       imageMenu->exec(QCursor::pos());
    });
    connect(action_wallpaper, &QAction::triggered, this,&ShowImageWidget::_setWallpaper);
}
//下一张
void ShowImageWidget::_nextImage()
{
    interaction->nextImage();
}
//下一张
void ShowImageWidget::_backImage()
{
    interaction->backImage();
}

void ShowImageWidget::_setWallpaper()
{
    qDebug()<<"设置壁纸";
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

    //创建相册列表
//    qDebug()<<var.value<QList<int>>();
}

void ShowImageWidget::openFinish(QVariant var)
{

    ImageAndInfo package =var.value<ImageAndInfo>();
    QFileInfo info = package.info;//详情信息
    QPixmap pixmap = package.image;//图片
    int type = package.type;//在队列中的标签
    int proportion = package.proportion;//比例
    QString num;
    num = QString("%1").arg(proportion) + "%";
    this->showImage->setPixmap(pixmap);
    emit perRate(num);
    emit ToshowImage();

//    qDebug()<<info<<type<<proportion;
}

void ShowImageWidget::re_move(int W, int H)
{

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
//    ShowImageWidget::resizeEvent(event);
    this->showImage->resize(KyView::mutual->width(),KyView::mutual->height());
    interaction->changeWidgetSize(this->showImage->size());
}

