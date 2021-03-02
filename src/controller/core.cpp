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
    initFile();
}

void Core::initDbus(QStringList arguments)
{
    dbus = new Dbus;

    //获取注册Dbus服务是否成功
    bool connectSeccess=dbus->getConnectSeccess();

    //如果注册Dbus服务成功，接收命令
    if(connectSeccess)
        connect(dbus,&Dbus::processingCommand,this,&Core::processingCommand);

    //没有参数不处理
    if(arguments.length() <2)
        return;

    //帮助命令则打印
    if(arguments[1]=="-help" || arguments[1]=="--help" ){
        for(QString &key : Variable::SUPPORT_CMD.keys()){
            qInfo()<<key<<"   "<<Variable::SUPPORT_CMD.value(key);
        }
        exit(0);
        return;
    }

    //如果是地址且文件存在则打开
    if(QFileInfo::exists(arguments[1])){
        //不是支持的文件格式忽略
        QString format =arguments[1];
        format=format.split(".").last();
        if(!Variable::SUPPORT_FORMATS.contains(format))
            return;
        processingCommand(arguments);
        return;
    }

    //如果是命令
    if(Variable::SUPPORT_CMD.keys().contains(arguments[1])){
        if(connectSeccess)//如果为首个实例不响应
            return;
        dbus->argumentsCommand(arguments);
        return;
    }

    qDebug()<<"参数异常";
}

void Core::initFile()
{
    file = new File;
}

void Core::processingCommand(QStringList cmd)
{
    qDebug()<<"此处处理命令"<<cmd;
}
