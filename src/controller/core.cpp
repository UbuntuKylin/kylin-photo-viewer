#include "core.h"
#include <QDebug>
Core::Core()
{
    _initCore();
}

void Core::_initCore()
{

}

void Core::loadCoreModel(QStringList arguments)
{
    _initDbus(arguments);
    _initFile();
}

void Core::_initDbus(QStringList arguments)
{
    _dbus = new Dbus;

    //获取注册Dbus服务是否成功
    bool connectSeccess=_dbus->getConnectSeccess();

    //如果注册Dbus服务成功，接收命令
    if(connectSeccess)
        connect(_dbus,&Dbus::processingCommand,this,&Core::_processingCommand);

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
        emit needOpenImage(arguments[1]);
        return;
    }

    //如果是命令
    if(Variable::SUPPORT_CMD.keys().contains(arguments[1])){
        if(connectSeccess)//如果为首个实例不响应
            return;
        _dbus->argumentsCommand(arguments);
        return;
    }

    qDebug()<<"参数异常";
}

void Core::_initFile()
{
    _file = new File;
}

void Core::_processingCommand(QStringList cmd)
{
    qDebug()<<"此处处理命令"<<cmd;
}

QList<int> Core::openImage(QString fullPath)
{
    Q_UNUSED(fullPath);
    QVariant oldVar = _file->loadImage(fullPath);
    //QVariant newVar;


    //emit openFinish();
    return QList<int>();
}

QList<int> Core::findAllImageFromeDir(QString fullPath)
{
    QString path = QFileInfo(fullPath).absolutePath();//提取绝对路径
    QDir dir(path);//实例化目录对象
    QStringList nameFilters;//格式过滤
    for(const QString &format : Variable::SUPPORT_FORMATS)
        nameFilters<<"*."+format;//构造格式过滤列表
    QStringList images = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);//获取所有支持的图片
    //将所有图片打上唯一标签并存入队列
    QMap<int,QString> tmpImageUrlMap;
    for(QString &filename : images){
        QString tmpFullPath = path+"/"+filename;
        _maxType++;
        tmpImageUrlMap.insert(_maxType,tmpFullPath);
        //记录需要显示的图片
        if(tmpFullPath == fullPath)
            _showingNowType = _maxType;
    }
    //新路径中的所有文件靠前排序
    _imageUrlMap.swap(tmpImageUrlMap);
    _imageUrlMap.unite(tmpImageUrlMap);
    return _imageUrlMap.keys();
}
