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
    m_canResize = new QTimer;
    m_canResize->setSingleShot(true);
}

void CoreInteraction::creatCore(const QStringList &list)
{
    if (m_isCoreInit) {
        return;//防止多次初始化核心
    }
    //开辟线程
    Core *core = new Core();
    m_needStartWithOpenImagePath = core->initDbus(list);
    QThread *thread = new QThread();
    //绑定信号和槽
    initConnect(core);
    //放到新线程中
    core->moveToThread(thread);
    thread->start();
    m_isCoreInit = true;//防止多次初始化核心
}

void CoreInteraction::initConnect(Core *core)
{
    //下面两个绑定顺序一定不要写反，查找文件再打开
    connect(this,&CoreInteraction::coreOpenImage,core,&Core::findAllImageFromDir);//生成缩略图列表
    //connect(this,&CoreInteraction::coreOpenImage,core,&Core::openImage);//打开图片

    connect(this,&CoreInteraction::coreGetAlbumModel,core,&Core::getAlbumModel,Qt::BlockingQueuedConnection);//获取相册model指针
    connect(core,&Core::openFinish,this,&CoreInteraction::openFinish);//图片打开完成，将数据返回给UI层
    connect(this,&CoreInteraction::coreChangeImage,core,&Core::changeImage);//切换图片
    connect(this,&CoreInteraction::coreChangeWidgetSize,core,&Core::changeWidgetSize);//改变窗口大小
    connect(this,&CoreInteraction::coreChangeImageShowSize,core,&Core::changeImageShowSize);//图片显示状态（放大缩小）
    connect(core,&Core::showNavigation,this,&CoreInteraction::showNavigation);//显示导航器
    connect(this,&CoreInteraction::coreClickNavigation,core,&Core::clickNavigation);//导航器点击
    connect(this,&CoreInteraction::coreFlip,core,&Core::flipImage);//翻转
    connect(this,&CoreInteraction::coreDeleteImage,core,&Core::deleteImage);//删除图片
    connect(this,&CoreInteraction::coreSetAsBackground,core,&Core::setAsBackground);//设置为背景图
    connect(this,&CoreInteraction::coreClose,core,&Core::close);//结束进程
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
    if (m_needStartWithOpenImagePath!="") {
        emit startWithOpenImage(m_needStartWithOpenImagePath);
        m_needStartWithOpenImagePath = "";
    }
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
