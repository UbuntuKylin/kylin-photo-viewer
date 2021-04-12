#include "coreinteraction.h"

Interaction * Interaction::m_interaction(nullptr);


Interaction *Interaction::getInstance()
{
    if (m_interaction == nullptr) {
        m_interaction = new CoreInteraction;
    }
    return m_interaction;
}


CoreInteraction::CoreInteraction()
{
    m_canResize = new QTimer(this);
    m_canResize->setSingleShot(true);
}

void CoreInteraction::creatCore(const QStringList &list)
{
    if (m_isCoreInit) {
        return;//防止多次初始化核心
    }
    //开辟线程
    m_core = new Core();
    m_needStartWithOpenImagePath = m_core->initDbus(list);
    QThread *thread = new QThread();
    //绑定信号和槽
    initConnect();
    //放到新线程中
    m_core->moveToThread(thread);
    thread->start();
    m_isCoreInit = true;//防止多次初始化核心
}

void CoreInteraction::initConnect()
{
    connect(this,&CoreInteraction::coreOpenImage,m_core,&Core::findAllImageFromDir);//生成缩略图列表
    connect(this,&CoreInteraction::coreGetAlbumModel,m_core,&Core::getAlbumModel,Qt::BlockingQueuedConnection);//获取相册model指针
    connect(m_core,&Core::openFinish,this,&CoreInteraction::openFinish);//图片打开完成，将数据返回给UI层
    connect(this,&CoreInteraction::coreChangeImage,m_core,&Core::changeImage);//切换图片
    connect(this,&CoreInteraction::coreChangeWidgetSize,m_core,&Core::changeWidgetSize);//改变窗口大小
    connect(this,&CoreInteraction::coreChangeImageShowSize,m_core,&Core::changeImageShowSize);//图片显示状态（放大缩小）
    connect(m_core,&Core::showNavigation,this,&CoreInteraction::showNavigation);//显示导航器
    connect(this,&CoreInteraction::coreClickNavigation,m_core,&Core::clickNavigation);//导航器点击
    connect(this,&CoreInteraction::coreFlip,m_core,&Core::flipImage);//翻转
    connect(this,&CoreInteraction::coreDeleteImage,m_core,&Core::deleteImage);//删除图片
    connect(this,&CoreInteraction::coreSetAsBackground,m_core,&Core::setAsBackground);//设置为背景图
    connect(this,&CoreInteraction::coreClose,m_core,&Core::close);//关闭事件
    connect(m_core,&Core::coreProgremExit,this,&CoreInteraction::progremExit);//发送信号让主界面结束进程
    connect(this,&CoreInteraction::changeImageFromClick,m_core,&Core::changeImageFromClick);//点击相册事件
}

bool CoreInteraction::coreOperateTooOften()
{
    if (m_canResize->isActive()) {
        return true;
    }
    m_canResize->start(Variable::REFRESH_RATE);//刷新间隔
    return false;
}

void CoreInteraction::initUiFinish()
{
    emit startWithOpenImage(m_needStartWithOpenImagePath);
}

void CoreInteraction::openImage(const QString &path)
{
    emit coreOpenImage(path);
}

void CoreInteraction::changeImage(const int &type)
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImage(type);
}

void CoreInteraction::nextImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImage(Enums::NEXT_IMAGE);
}

void CoreInteraction::backImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImage(Enums::BACK_IMAGE);
}

void CoreInteraction::changeWidgetSize(const QSize &size)
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeWidgetSize(size);
}

void CoreInteraction::watchBigImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImageShowSize(ImageShowStatus::BIG);
}

void CoreInteraction::watchSmallImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImageShowSize(ImageShowStatus::SMALL);
}

void CoreInteraction::watchOriginalImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImageShowSize(ImageShowStatus::ORIGIN);
}

void CoreInteraction::watchAutoImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreChangeImageShowSize(ImageShowStatus::AUTO);
}

void CoreInteraction::clickNavigation(const QPoint &point)
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreClickNavigation(point);
}

void CoreInteraction::rotate(const bool &clockwise)
{
    if (coreOperateTooOften()) {
        return;
    }
    if (clockwise) {
        emit coreFlip(Processing::clockwise);
    } else {
        emit coreFlip(Processing::counterclockwise);
    }
}

void CoreInteraction::flipH()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreFlip(Processing::horizontal);
}

void CoreInteraction::flipV()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreFlip(Processing::vertical);
}

void CoreInteraction::deleteImage()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreDeleteImage();
}

void CoreInteraction::setAsBackground()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreSetAsBackground();
}

void CoreInteraction::close()
{
    if (coreOperateTooOften()) {
        return;
    }
    emit coreClose();
}

QStandardItemModel *CoreInteraction::getAlbumModel()
{
    return coreGetAlbumModel();
}


