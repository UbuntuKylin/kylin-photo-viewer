#include "showimagewidget.h"
#include "kyview.h"
#include "sizedate.h"
ShowImageWidget::ShowImageWidget(QWidget *parent, int w, int h) : QWidget(parent)
{
    this->resize(w,h);

    initInteraction();

    //中间展示图片部分
    m_showImage = new QLabel(this);
    m_showImage->resize(this->width(),this->height());
    m_showImage->move(int((this->width() - m_showImage->width())/2),int((this->height() - m_showImage->height())/2));
    m_showImage->setMouseTracking(true);
    m_showImage->installEventFilter(this);
    m_showImage->setAlignment(Qt::AlignCenter);
    m_showImage->setContextMenuPolicy(Qt::ActionsContextMenu);
    m_showImage->setContextMenuPolicy(Qt::CustomContextMenu);

    //菜单栏给功能选项：复制，设置为桌面壁纸，设置为锁屏壁纸，打印，删除，在文件夹中显示
    m_copy = new QAction(tr("Copy"),this);
    m_setDeskPaper = new QAction(tr("Set Desktop Wallpaper"),this);
    m_setLockPaper = new QAction(tr("Set Lock Wallpaper"),this);
    m_print = new QAction(tr("Print"),this);
    m_deleteImage = new QAction(tr("Delete"),this);
    m_showInFile = new QAction(tr("Show in File"),this);
    //右键菜单
    m_imageMenu = new QMenu(this);
//    imageMenu->addAction(copy);
    m_imageMenu->addAction(m_setDeskPaper);
//    imageMenu->addAction(setLockPaper);
//    imageMenu->addAction(print);
    m_imageMenu->addAction(m_deleteImage);
    m_imageMenu->addAction(m_showInFile);
    //上一张，下一张按钮
    g_next = new QPushButton(this);
    g_next->resize(IMAGE_BUTTON);

    g_back = new QPushButton(this);
    g_back->resize(IMAGE_BUTTON);


    g_back->move(LEFT_POS,int((this->height() - g_back->height())/2));
    g_next->move(this->width() - LEFT_POS - g_next->width(),int((this->height() - g_next->height())/2));

    g_back->setIconSize(IMAGE_ICON);
    g_next->setIconSize(IMAGE_ICON);

    g_back->setFocusPolicy(Qt::NoFocus);
    g_next->setFocusPolicy(Qt::NoFocus);
    g_back->setStyleSheet("background-color:transparent;border-radius:4px;");
    g_next->setStyleSheet("background-color:transparent;border-radius:4px;");
    //图片没处理完毕时，显示转圈圈图
    m_loadingMovie = new QMovie(":/res/res/loadgif.gif");

    this->initConnect();
}

void ShowImageWidget::initConnect()
{
    connect(g_next,&QPushButton::clicked,this,&ShowImageWidget::nextImage);
    connect(g_back,&QPushButton::clicked,this,&ShowImageWidget::backImage);
    //右键菜单
    connect(m_showImage,&QLabel::customContextMenuRequested,[=](const QPoint &pos){
       Q_UNUSED(pos);
       m_imageMenu->exec(QCursor::pos());
    });
    //点击菜单选项
//    connect(copy, &QAction::triggered, this,&ShowImageWidget::copy);
    connect(m_setDeskPaper, &QAction::triggered, this,&ShowImageWidget::setDeskPaper);
//    connect(m_setLockPaper, &QAction::triggered, this,&ShowImageWidget::setLockPaper);
//    connect(m_print, &QAction::triggered, this,&ShowImageWidget::print);
    connect(m_deleteImage, &QAction::triggered, this,&ShowImageWidget::deleteImage);
    connect(m_showInFile, &QAction::triggered, this,&ShowImageWidget::showInFile);

}

void ShowImageWidget::sideState(int num)
{
    //判断是不是点击删除按钮触发切换图片
    if (m_isDelete == false) {
       emit changeSideSize(num);
    } else {
        emit changeSideSize(num - 1);
    }
    //判断是否应该显示侧栏
    if (num >= 2) {
        if (m_isOpen != true) {
            return;
        }
        emit toShowSide();
    }
}
//下一张
void ShowImageWidget::nextImage()
{
    m_canSet = true;
    Interaction::getInstance()->nextImage();
}
//上一张
void ShowImageWidget::backImage()
{
    m_canSet = true;
    Interaction::getInstance()->backImage();
}
//备用，图片加载过慢时，ui的处理--暂缓
void ShowImageWidget::delayShow(bool isLoading)
{
    if (isLoading == false) {
        if (m_loadingMovie->state() != QMovie::NotRunning) {
            m_loadingMovie->stop();
        }
        return;
    } else {
        this->m_showImage->setMovie(m_loadingMovie);
        m_loadingMovie->start();
        emit toShowImage();//给主界面--展示图片
    }

}

void ShowImageWidget::albumSlot(bool isOpen)
{
    m_isOpen = isOpen;
}

void ShowImageWidget::isDelete(bool isDel)
{
    m_isDelete = isDel;
//    emit toSelectHigh(isDel);
}

//复制
void ShowImageWidget::copy()
{
    qDebug()<<"复制";
}
//设置为桌面壁纸
void ShowImageWidget::setDeskPaper()
{
    Interaction::getInstance()->setAsBackground();
}
//设置为锁屏壁纸
void ShowImageWidget::setLockPaper()
{
    qDebug()<<"设置为锁屏壁纸";
}
//打印
void ShowImageWidget::print()
{
    qDebug()<<"打印";
}
//删除
void ShowImageWidget::deleteImage()
{
    emit isDelete(true);
    Interaction::getInstance()->deleteImage();
}
//在文件夹中显示
void ShowImageWidget::showInFile()
{
    Interaction::getInstance()->openImageInfile();
//    if (m_path == "") {
//        return;
//    }
//    QDesktopServices::openUrl(QUrl::fromLocalFile(m_path));

}
//显示右键菜单栏
void ShowImageWidget::setMenuAction()
{
    //拿到可设置为壁纸的图片格式
    QStringList formatList;
    QString format;
    format = "";
    for (const QString &str:Variable::BACKGROUND_SUPPORT_FORMATS) {
        format = str;
        formatList.append(format);
    }
    //判断是否为可设置为壁纸的类型
    if (formatList.contains(m_paperFormat)) {
        m_imageMenu->insertAction(m_deleteImage,m_setDeskPaper);
    } else {
        m_imageMenu->removeAction(m_setDeskPaper);
    }

}

void ShowImageWidget::initInteraction()
{
    connect(Interaction::getInstance(),&Interaction::startWithOpenImage,this,&ShowImageWidget::startWithOpenImage);//启动时打开图片
    connect(Interaction::getInstance(),&Interaction::openFinish,this,&ShowImageWidget::openFinish);//图片打开完成，获取数据
    connect(Interaction::getInstance(),&Interaction::delayShow,this,&ShowImageWidget::delayShow);
}
//双击或带参数打开
void ShowImageWidget::startWithOpenImage(QString path)
{
    if (path != Variable::API_TYPE) {
        KyView::mutual->show();
    }
    if (path == "") {
        return;
    }
    //双击和右键打开图片，将路径写入
    QFileInfo info(path);
    Variable::setSettings("imagePath",info.absolutePath());
    openImage(path);
}
//打开图片
void ShowImageWidget::openImage(QString path)
{
//    delayShow();
    Interaction::getInstance()->openImage(path);
}
//拿到图片信息，进行处理
void ShowImageWidget::openFinish(QVariant var)
{

    ImageAndInfo package =var.value<ImageAndInfo>();
    QPixmap pixmap = package.image;//图片

    //拿到返回信息
    QFileInfo info = package.info;//详情信息
    int proportion = package.proportion;//比例
    QString imageSize = package.imageSize;
    QString colorSpace = package.colorSpace;
    QString num;
    int number = package.imageNumber;//在队列中的标签
    //判断有几张图片，分别进行处理：删除到0，显示打开界面；只有一张：不显示左右按钮。
    if (number == 0) {
        emit clearImage();
        //点击删除按钮删除全部文件时，此标志位应该重设为默认状态，防止之后继续打开图片造成相册大小有误差
        m_isDelete = false;
        return;
    }
    if (number == 1) {
        g_buttonState = false;
    } else {
        //删除时逻辑导致item总是比实际多一张，暂时先在前端进行判断来解决相关问题
        if (number == 2 && m_isDelete == true) {
            g_buttonState = false;
        } else {
            g_buttonState = true;
        }

    }

    num = QString("%1").arg(proportion) + "%";
    m_path = info.absolutePath();//图片的路径
    m_paperFormat = info.suffix();
    m_typeNum = number;
    //使用返回的信息进行设置界面
    emit toShowImage();//给主界面--展示图片
    emit perRate(num);//发送给toolbar来更改缩放数字
    emit changeInfor(info,imageSize,colorSpace);//给信息栏需要的信息
    emit titleName(info.fileName());//给顶栏图片的名字

    if (pixmap.isNull()) {
        this->m_showImage->setMovie(m_loadingMovie);
        m_loadingMovie->start();
        return;
    } else {
        if (m_loadingMovie->state() != QMovie::NotRunning) {
            m_loadingMovie->stop();
        }
    }
    this->m_showImage->setPixmap(pixmap);
    //区分动图和静态图
    if (m_paperFormat == "gif" || m_paperFormat == "apng") {
        //设置壁纸--动图在传来时是一帧一帧，只判断并添加一次右键菜单选项
        if (m_canSet)  {
            m_canSet = false;
            //设置右键菜单
            setMenuAction();
        }
    } else {
        setMenuAction();
    }
    sideState(m_typeNum);
    m_isDelete = false;
//    emit toSelectHigh(false);

}
//拉伸主界面时重新安排界面显示
void ShowImageWidget::reMove(int W, int H)
{
    //拉伸主界面时重新安排界面显示
    this->resize(W,H);
    this->m_showImage->resize(W,H);
    this->m_showImage->move(int((W - this->m_showImage->width())/2),int((H - this->m_showImage->height())/2));
    g_back->move(LEFT_POS,int((this->height() - g_back->height())/2));
    g_next->move(W - LEFT_POS - g_next->width(),int((H - g_next->height())/2));
}

void ShowImageWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    this->m_showImage->resize(KyView::mutual->width(),KyView::mutual->height());
    Interaction::getInstance()->changeWidgetSize(this->m_showImage->size());
}
//滚轮放大缩小
bool ShowImageWidget::eventFilter(QObject *obj, QEvent *event)
{
    //滚轮进行放大缩小图片
    if (obj == m_showImage) {
        if (event->type()==QEvent::Wheel) {
            QWheelEvent *wheelEvent=static_cast<QWheelEvent *>(event);
            if (wheelEvent->delta()>0) {
                emit enlargeChange();
            } else {
                emit reduceChange();
            }
        }
    }

    return QObject::eventFilter(obj,event);
}


