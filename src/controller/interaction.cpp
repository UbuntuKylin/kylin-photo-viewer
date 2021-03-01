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
    if(isCoreInit)return;//防止多次初始化核心
    //开辟线程
    Core *core = new Core();
    QThread *thread = new QThread();
    connect(this,&_Interaction::initCoreModel,core,&Core::initCoreModel);
    core->moveToThread(thread);
    thread->start();
    //初始化核心功能
    emit initCoreModel(list);
    disconnect(this,&_Interaction::initCoreModel,core,&Core::initCoreModel);//只初始化一次，所以解绑节省资源
    isCoreInit = true;//防止多次初始化核心
}
