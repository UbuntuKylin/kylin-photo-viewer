#include "core.h"
#include <QDebug>
Core::Core()
{
    initCore();
}

void Core::initCore()
{

}

void Core::initCoreModel(QStringList arguments)
{
    initDbus(arguments);
}

void Core::initDbus(QStringList arguments)
{
    dbus = new Dbus;
    //没有参数不处理
    if(arguments.length() <2)
        return;

    //如果是地址则打开
    if(QFileInfo::exists(arguments[1]))
    {
        //emit startWithOpen(arguments[1]);
        return;
    }

    //如果是命令
    if(Variable::SUPPORT_CMD.keys().contains(arguments[1]))
        dbus->argumentsCommand(arguments[1]);

    qDebug()<<"参数异常";
}
