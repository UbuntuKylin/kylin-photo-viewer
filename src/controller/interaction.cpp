#include "interaction.h"

Interaction * Interaction::m_interaction(nullptr);

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
}

void _Interaction::initUiFinish()
{
    if(_needStartWithOpenImagePath != ""){
        emit startWithOpenImage(_needStartWithOpenImagePath);
    }
    _initUiFinish = true;
}

QList<int> _Interaction::openImage(QString path)
{
    return _openImage(path);
}

void _Interaction::needOpenImage(QString path)
{
    if(_initUiFinish){
        //如果UI已经初始化完成
        emit startWithOpenImage(path);
        return;
    }
    //如果未初始化完成则进入任务队列
    _needStartWithOpenImagePath = path;
}

