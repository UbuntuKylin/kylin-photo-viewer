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
    _proportion = 0;
    _isNavigationShow = false;
    qRegisterMetaType<ImageShowStatus::ChangeShowSizeType>("ImageShowStatus::ChangeShowSizeType");
    qRegisterMetaType<Processing::FlipWay>("Processing::FlipWay");
}

void Core::loadCoreModel(QStringList arguments)
{
    _initDbus(arguments);
}

void Core::_initDbus(const QStringList &arguments)
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

void Core::_processingCommand(const QStringList &cmd)
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
    if(cmd[0] == "-big"){
        changeImageShowSize(ImageShowStatus::Big);
        return;
    }
    if(cmd[0] == "-small"){
        changeImageShowSize(ImageShowStatus::Small);
        return;
    }
    if(cmd[0] == "-origin"){
        changeImageShowSize(ImageShowStatus::Origin);
        return;
    }
    if(cmd[0] == "-auto"){
        changeImageShowSize(ImageShowStatus::Auto);
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
    _creatImage();
    return QVariant();
}

void Core::_showImage(const QPixmap &pix)
{
    ImageAndInfo package;
    package.proportion = _proportion;
    package.info = _info;
    package.image = pix;
    package.type = _nowType;
    QVariant var;
    var.setValue<ImageAndInfo>(package);
    emit openFinish(var);
}

void Core::_creatImage(const int &proportion)
{
    if(_nowImage.isNull())
        return;

    int defaultProportion  = 100 * _size.width() / _nowImage.width();

    //自适应窗口大小显示
    if(proportion <= 0){
        QPixmap pix = Processing::resizePix(_nowImage,_size);
        _proportion  = defaultProportion;
        _navigation(); //关闭导航器
        _showImage(pix);
        return;
    }

    _proportion = proportion;

    //如果显示比例大于默认比例
    if(_proportion > defaultProportion){
        _navigation(QPoint(0,0));
        return;
    }

    //如果显示比例小于或等于默认比例
    _navigation(); //关闭导航器
    QSize tmpSize = _nowImage.size() * _proportion / 100;;

    QPixmap pix = Processing::resizePix(_nowImage,tmpSize);
    _showImage(pix);
}

void Core::_navigation(const QPoint &point)
{
    if(point.isNull()){//关闭导航器
        _isNavigationShow = false;
        emit showNavigation(QPixmap());
    }
    clickNavigation(point);
}

void Core::_pictureDeepen(QImage &image , const QSize &hightlightSize ,const QPoint &point)
{
    int key = Variable::PICTURE_DEEPEN_KEY;
    int left = point.x();
    int right = point.x()+hightlightSize.width();
    int top = point.y();
    int bottom = point.y()+hightlightSize.height();

    for(int j = 0 ; j < image.height() ; ++j){
        for(int i = 0 ; i < image.width() ; ++i){
            if(i>left && i<right && j>top && j<bottom)continue;//高亮区域不处理
            QColor color(image.pixel(i, j));
            color.setRed(color.red()-key);
            color.setGreen(color.green()-key);
            color.setBlue(color.blue()-key);
            image.setPixel(i, j, color.rgb());
        }
    }
}

void Core::clickNavigation(const QPoint &point)
{
    //导航栏背景
    QSize navigationSize = Variable::NAVIGATION_SIZE;
    QImage navigation = Processing::resizePix(_nowImage,navigationSize).toImage();

    //高亮区域大小
    QSize hightlightSize;
    hightlightSize.setWidth(navigationSize.width() * _size.width() / _nowImage.width());
    hightlightSize.setWidth(navigationSize.height() * _size.height() / _nowImage.height());

    //计算点击区域
    QSize halfHightlightSize = hightlightSize / 2;
    QPoint startPoint(point.x()-halfHightlightSize.width(),point.y()-halfHightlightSize.height());
    int right = navigation.width()-halfHightlightSize.width();//右侧边缘
    int bottom = navigation.width()-halfHightlightSize.width();//下侧边缘

    //过滤无效区域
    if(startPoint.x()<0)startPoint.setX(0);
    if(startPoint.y()<0)startPoint.setY(0);
    if(startPoint.x()>right)startPoint.setX(right);
    if(startPoint.y()>bottom)startPoint.setY(bottom);

    //和上次点击的有效区域一致则不处理
    if(startPoint == _clickBeforePosition)
        return;
    _clickBeforePosition = startPoint;

    //处理导航器图片
    _pictureDeepen(navigation,hightlightSize,startPoint);

    //发送到导航器
    emit showNavigation(QPixmap::fromImage(navigation));

    //处理待显示区域
    QSize pixSize = _nowImage.size() * _proportion / 100;
    QPixmap pix = Processing::resizePix(_nowImage,pixSize);
    QPoint start = startPoint * _nowImage.width() / navigation.width();
    QPixmap result = pix.copy(start.x(),start.y(),_size.width(),_size.height());
    _showImage(result);
}

void Core::flipImage(const Processing::FlipWay &way)
{
    Mat mat = Processing::processingImage(Processing::flip,_nowMat,QVariant(way));
    if(!mat.data)
        return;
    mat = _changeImage(mat);
    _nowImage = Processing::converFormat(mat);
    _creatImage();
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
    _creatImage();
}

void Core::changeImageShowSize(ImageShowStatus::ChangeShowSizeType type)
{
    int resizeKey = Variable::RESIZE_KEY;
    int tmpProportion = 0;
    switch (type) {

    ///----------------------------放大图片----------------------------
    case ImageShowStatus::Big:
        if(_proportion == Variable::RESIZE_KEY_MAX){//等于临界值
            return;
        }
        if(_proportion + resizeKey > Variable::RESIZE_KEY_MAX){//将要超出临界值
            tmpProportion = Variable::RESIZE_KEY_MAX;
            _creatImage(tmpProportion);
            return;
        }
        if(_proportion % resizeKey != 0){ //不能整除
            tmpProportion = (_proportion/resizeKey + 1)*resizeKey;
            _creatImage(tmpProportion);
            return;
        }
        tmpProportion = _proportion + resizeKey;
        _creatImage(tmpProportion);
        return;

        ///----------------------------缩小图片----------------------------
    case ImageShowStatus::Small:
        if(_proportion == Variable::RESIZE_KEY_MIN){//等于临界值
            return;
        }
        if(_proportion - resizeKey < Variable::RESIZE_KEY_MIN){//将要超出临界值
            tmpProportion = Variable::RESIZE_KEY_MIN;
            _creatImage(tmpProportion);
            return;
        }
        if(_proportion % resizeKey != 0){ //不能整除
            tmpProportion = (_proportion/resizeKey)*resizeKey;
            _creatImage(tmpProportion);
            return;
        }
        tmpProportion = _proportion - resizeKey;
        _creatImage(tmpProportion);
        return;

        ///----------------------------查看原图----------------------------
    case ImageShowStatus::Origin:
        _creatImage(100);
        return;

        ///----------------------------查看自适应图----------------------------
    case ImageShowStatus::Auto:
        _creatImage();
        return;

    default:
        qDebug()<<"代码错误，请检查";
        break;
    }
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

