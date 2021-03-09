#include "interaction.h"

Interaction * Interaction::m_interaction(nullptr);

_Interaction::_Interaction()
{
    _canResize = new QTimer;
    _canResize->setSingleShot(true);
}

Interaction *Interaction::getInstance()
{
    if(m_interaction == nullptr)
    {
        m_interaction = new _Interaction;
    }
    return m_interaction;
}

void _Interaction::creatCore(const QStringList &list)
{
    if(_isCoreInit)return;//防止多次初始化核心
    //开辟线程
    Core *core = new Core();
    QThread *thread = new QThread();
    //绑定信号和槽
    _initConnect(core);
    //放到新线程中
    core->moveToThread(thread);
    thread->start();
    //初始化核心功能
    emit _loadCoreModel(list);
    disconnect(this,&_Interaction::_loadCoreModel,core,&Core::loadCoreModel);//只需初始化一次，所以解绑节省资源
    _isCoreInit = true;//防止多次初始化核心
}

void _Interaction::_initConnect(Core *core)
{
    connect(this,&_Interaction::_loadCoreModel,core,&Core::loadCoreModel);//加载核心模块

    //下面两个绑定顺序一定不要写反，查找文件再打开
    connect(this,&_Interaction::_openImage,core,&Core::findAllImageFromeDir,Qt::BlockingQueuedConnection);//阻塞调用取反回值
    connect(this,&_Interaction::_openImage,core,&Core::openImage);//阻塞调用取反回值

    connect(core,&Core::needOpenImage,this,&_Interaction::needOpenImage);//需要启动时加载图片
    connect(core,&Core::openFinish,this,&_Interaction::openFinish);//图片打开完成，将数据返回给UI层
    connect(core,&Core::albumFinish,this,&Interaction::albumFinish);//相册缩略图打开完成，获取数据
    connect(this,&_Interaction::_changeImage,core,&Core::changeImage);//切换图片
    connect(this,&_Interaction::_changeWidgetSize,core,&Core::changeWidgetSize);//切换图片_changeImageShowSize
    connect(this,&_Interaction::_changeImageShowSize,core,&Core::changeImageShowSize);//图片显示状态（放大缩小）
    connect(core,&Core::showNavigation,this,&_Interaction::showNavigation);//显示导航器
    connect(this,&_Interaction::_clickNavigation,core,&Core::clickNavigation);//导航器点击
}

void _Interaction::initUiFinish()
{
    if(_needStartWithOpenImagePath != ""){
        emit startWithOpenImage(_needStartWithOpenImagePath);
    }
    _initUiFinish = true;
}

QVariant _Interaction::openImage(const QString &path)
{
    return _openImage(path);
}

void _Interaction::changeImage(const int &type)
{
    emit _changeImage(type);
}

void _Interaction::nextImage()
{
    emit _changeImage(-1);
}

void _Interaction::backImage()
{
    emit _changeImage(-2);
}

void _Interaction::changeWidgetSize(const QSize &size)
{
    if(_canResize->isActive())
        return;
    _canResize->start(Variable::REFRESH_RATE);//刷新间隔
    emit _changeWidgetSize(size);
}

void _Interaction::watchBigImage()
{
    emit _changeImageShowSize(ImageShowStatus::Big);
}

void _Interaction::watchSmallImage()
{
    emit _changeImageShowSize(ImageShowStatus::Small);
}

void _Interaction::watchOriginalImage()
{
    emit _changeImageShowSize(ImageShowStatus::Origin);
}

void _Interaction::watchAutoImage()
{
    emit _changeImageShowSize(ImageShowStatus::Auto);
}

void _Interaction::clickNavigation(const QPoint &point)
{
    emit _clickNavigation(point);
}

void _Interaction::needOpenImage(const QString &path)
{
    if(_initUiFinish){
        //如果UI已经初始化完成
        emit startWithOpenImage(path);
        return;
    }
    //如果未初始化完成则进入任务队列
    _needStartWithOpenImagePath = path;
}

