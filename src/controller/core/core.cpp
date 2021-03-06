#include "core.h"

Core::Core()
{
    _initCore();
}

void Core::_initCore()
{
    _maxType = 0;
    _nowType = 0;
    _backType = 0;
}

void Core::loadCoreModel(QStringList arguments)
{
    _initDbus(arguments);
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

void Core::_processingCommand(QStringList cmd)
{
    qDebug()<<"响应外部命令"<<cmd;

    if(cmd[0] == "-next"){
        changeImage(-1);
        return;
    }
    if(cmd[0] == "-back"){
        changeImage(-2);
        return;
    }
}

Mat ImageShowStatus::_changeImage(Mat mat)
{
    if(_backMat.data)
        _backMat.release();
    _backMat = _nowMat;
    _nowMat = mat;
    return _nowMat;
}

void ImageShowStatus::_changeImageType(int num)
{
    if(num == 0)//回滚
    {
        _nowType=_backType;
        return;
    }
    _backType = _nowType;
    _nowType = num;
}

QVariant Core::openImage(QString fullPath)
{
    MatAndFileinfo maf = File::loadImage(fullPath);
    if(!maf.mat.data)
    {
        //如果图片打开失败则回滚
        _changeImageType();
        return QVariant();
    }
    //格式转换，记录状态
    Mat mat = _changeImage(maf.mat);
    _nowImage = Processing::converFormat(mat);
    _info = maf.info;
    _showImage();
    return QVariant();
}

void Core::_showImage()
{
    if(_nowImage.isNull())
        return;
    QPixmap pix = Processing::resizePix(_nowImage,_size);
    ImageAndInfo package;
    package.proportion = 100 * pix.width() / _nowImage.width();
    package.info = _info;
    package.image = pix;
    package.type = _nowType;
    QVariant var;
    var.setValue<ImageAndInfo>(package);
    emit openFinish(var);
}

void Core::changeImage(const int &mat)
{
    //如果图片队列小于2，不处理
    if(_imageUrlMap.size()<2)
        return;

    QList<int> list = _imageUrlMap.keys();
    if(mat == -1){
        if(_nowType == list.last()){
            //最后一张切下一张时，回到队列开头
            _changeImageType(list.first());
            openImage(_imageUrlMap.first());
            return;
        }
        int key =list[list.indexOf(_nowType)+1];
        _changeImageType(key);
        openImage(_imageUrlMap.value(key));
        return;
    }
    if(mat == -2){
        if(_nowType == list.first()){
            //第一张切上一张时，回到队列结尾
            _changeImageType(list.last());
            openImage(_imageUrlMap.last());
            return;
        }
        int key =list[list.indexOf(_nowType)-1];
        _changeImageType(key);
        openImage(_imageUrlMap.value(key));
        return;
    }
    //如果队列中无此关键值，不处理
    if(list.indexOf(mat)<0)
        return;
    _changeImageType(mat);
    openImage(_imageUrlMap.value(mat));
}

void Core::changeWidgetSize(const QSize &size)
{
    _size = size;
    _showImage();
}

QVariant Core::findAllImageFromeDir(QString fullPath)
{
    QFileInfo info(fullPath);
    QString path = info.absolutePath();//转成绝对路径
    QString filepath = info.absoluteFilePath();//转成绝对路径
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
        if(tmpFullPath == filepath)
        {
            _backType = _nowType;
            _nowType = _maxType;
        }
    }
    //新路径中的所有文件靠前排序
    _imageUrlMap.swap(tmpImageUrlMap);
    _imageUrlMap.unite(tmpImageUrlMap);
    QVariant var;
    var.setValue<QList<int>>(_imageUrlMap.keys());
    _loadAlbum();
    return var;
}

void Core::_loadAlbum()
{
    for(int &type : _imageUrlMap.keys()){
        AlbumThumbnail* thread= new AlbumThumbnail(type,_imageUrlMap.value(type));
        connect(thread,&AlbumThumbnail::finished,thread,&AlbumThumbnail::deleteLater);
        connect(thread,&AlbumThumbnail::albumFinish,this,&Core::albumFinish);
        thread->start();
    }
}
