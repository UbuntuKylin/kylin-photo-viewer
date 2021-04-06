#include "showimagewidget.h"
#include "kyview.h"
#include "sizedate.h"
ShowImageWidget::ShowImageWidget(QWidget *parent, int w, int h) : QWidget(parent)
{
    this->resize(w,h);
//    imageWid = new QWidget(this);
//    imageLayout = new QHBoxLayout(this);

    m_showImage = new QLabel(this);
    m_showImage->resize(this->width(),this->height());
    m_showImage->move(int((this->width() - m_showImage->width())/2),int((this->height() - m_showImage->height())/2));
    m_showImage->setMouseTracking(true);
    m_showImage->installEventFilter(this);
    m_showImage->setAlignment(Qt::AlignCenter);
    m_showImage->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_showImage->setContextMenuPolicy(Qt::CustomContextMenu);

//    imageLayout->addWidget(showImage);
//    imageWid->setLayout(imageLayout);
//    imageWid->resize(this->width(),this->height());
//    imageWid->move(int((this->width() - imageWid->width())/2),int((this->height() - imageWid->height())/2));
//    imageWid->setMouseTracking(true);


    m_copy = new QAction(tr("Copy"),this);
    m_setDeskPaper = new QAction(tr("Set Desktop Wallpaper"),this);
    m_setLockPaper = new QAction(tr("Set Lock Wallpaper"),this);
    m_print = new QAction(tr("Print"),this);
    m_deleteImage = new QAction(tr("Delete"),this);
    m_showInFile = new QAction(tr("Show in File"),this);
    m_imageMenu = new QMenu(this);
//    imageMenu->addAction(copy);
//    imageMenu->addAction(copy);
//    imageMenu->addAction(setDeskPaper);
//    imageMenu->addAction(setLockPaper);
//    imageMenu->addAction(print);
    m_imageMenu->addAction(m_deleteImage);
    m_imageMenu->addAction(m_showInFile);

    g_next = new QPushButton(this);
    g_next->resize(SizeDate::IMAGEBUTTON);
    g_next->setIcon(QIcon(":/res/res/right.png"));
    g_back = new QPushButton(this);
    g_back->resize(SizeDate::IMAGEBUTTON);
    g_back->setIcon(QIcon(":/res/res/left.png"));

    g_back->move(SizeDate::LEFTPOS,int((this->height() - g_back->height())/2));
    g_next->move(this->width() - SizeDate::LEFTPOS - g_next->width(),int((this->height() - g_next->height())/2));

    g_back->setIconSize(SizeDate::IMAGEICON);
    g_next->setIconSize(SizeDate::IMAGEICON);

    g_back->setFocusPolicy(Qt::NoFocus);
    g_next->setFocusPolicy(Qt::NoFocus);
    g_back->setStyleSheet("background-color:transparent;border-radius:4px;");
    g_next->setStyleSheet("background-color:transparent;border-radius:4px;");

    this->initConnect();
    initInteraction();//一定要放到构造函数末尾
}

void ShowImageWidget::initConnect()
{
    connect(g_next,&QPushButton::clicked,this,&ShowImageWidget::nextImage);
    connect(g_back,&QPushButton::clicked,this,&ShowImageWidget::backImage);
    //设置壁纸
    connect(m_showImage,&QLabel::customContextMenuRequested,[=](const QPoint &pos){
       Q_UNUSED(pos);
       m_imageMenu->exec(QCursor::pos());
    });
//    connect(copy, &QAction::triggered, this,&ShowImageWidget::copy);
    connect(m_setDeskPaper, &QAction::triggered, this,&ShowImageWidget::setDeskPaper);
//    connect(m_setLockPaper, &QAction::triggered, this,&ShowImageWidget::setLockPaper);
//    connect(m_print, &QAction::triggered, this,&ShowImageWidget::print);
    connect(m_deleteImage, &QAction::triggered, this,&ShowImageWidget::deleteImage);
    connect(m_showInFile, &QAction::triggered, this,&ShowImageWidget::showInFile);

}
//下一张
void ShowImageWidget::nextImage()
{
    m_canSet = true;
    m_interaction->nextImage();
}
//上一张
void ShowImageWidget::backImage()
{
    m_canSet = true;
    m_interaction->backImage();
}

void ShowImageWidget::copy()
{
    qDebug()<<"复制";
}

void ShowImageWidget::setDeskPaper()
{
    m_interaction->setAsBackground();//设置为桌面壁纸
}

void ShowImageWidget::setLockPaper()
{
    qDebug()<<"设置为锁屏壁纸";
}

void ShowImageWidget::print()
{
    qDebug()<<"打印";
}
//删除
void ShowImageWidget::deleteImage()
{
    m_interaction->deleteImage();
}
//在文件夹中显示
void ShowImageWidget::showInFile()
{

    if (m_path == ""){
        return;
    }else{
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_path));
    }
}
//显示右键菜单栏
void ShowImageWidget::setMenuAction()
{
    //拿到可设置为壁纸的图片格式
    QStringList formatList;
    QString format;
    format = "";
    for(const QString &str:Variable::BACKGROUND_SUPPORT_FORMATS )
    {
        format = str;
        formatList.append(format);
    }
    //判断是否为可设置为壁纸的类型
    if(formatList.contains(m_paperFormat))
    {
      m_imageMenu->insertAction(m_deleteImage,m_setDeskPaper);
    }else{
        m_imageMenu->removeAction(m_setDeskPaper);
    }

}

void ShowImageWidget::initInteraction()
{
    m_interaction =Interaction::getInstance();
    //此处绑定信号和槽
    connect(m_interaction,&Interaction::startWithOpenImage,this,&ShowImageWidget::startWithOpenImage);//启动时打开图片
    connect(m_interaction,&Interaction::openFinish,this,&ShowImageWidget::openFinish);//图片打开完成，获取数据
    connect(m_interaction,&Interaction::albumFinish,this,&ShowImageWidget::albumFinish);//相册缩略图打开完成，获取数据
    m_interaction->initUiFinish();
}
void ShowImageWidget::startWithOpenImage(QString path)
{
    openImage(path);
}
//打开图片
void ShowImageWidget::openImage(QString path)
{
    QVariant var= m_interaction->openImage(path);
    //相册列表，需要得到图片的数量
    QList<int> list = var.value<QList<int>>();
    //小于2张，左右按钮隐藏
    if(list.length() < 2){
            //主界面需要知道是否只有一张图片来处理左右按钮显示。
            g_buttonState = false;
        }else{
            g_buttonState = true;
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
    m_path = info.absolutePath();//图片的路径
    m_copyImage = pixmap;//留着复制可能用
    m_paperFormat = info.suffix();

    //使用返回的信息进行设置界面
    this->m_showImage->setPixmap(pixmap);
    emit perRate(num);//发送给toolbar来更改缩放数字
    emit ToshowImage();//给主界面--展示图片
    emit changeInfor(info,imageSize,colorSpace);//给信息栏需要的信息
    emit titleName(info.fileName());//给顶栏图片的名字

    //设置壁纸--动图在传来时是一帧一帧，只判断并添加一次右键菜单选项
    if(m_canSet)
    {
        m_canSet = false;
        setMenuAction();

    }
}
//拉伸主界面时重新安排界面显示
void ShowImageWidget::re_move(int W, int H)
{
    //拉伸主界面时重新安排界面显示
    this->resize(W,H);
    this->m_showImage->resize(W,H);
    this->m_showImage->move(int((W - this->m_showImage->width())/2),int((H - this->m_showImage->height())/2));
    g_back->move(SizeDate::LEFTPOS,int((this->height() - g_back->height())/2));
    g_next->move(W - SizeDate::LEFTPOS - g_next->width(),int((H - g_next->height())/2));
}
//相册
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
    this->m_showImage->resize(KyView::mutual->width(),KyView::mutual->height());
    m_interaction->changeWidgetSize(this->m_showImage->size());
}
//滚轮放大缩小
bool ShowImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    //滚轮进行放大缩小图片
    if(obj == m_showImage)
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


