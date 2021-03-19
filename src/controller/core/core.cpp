#include "core.h"

Core::Core()
{
    _initCore();
}

void Core::_initCore()
{
    _matList = nullptr;

    qRegisterMetaType<ImageShowStatus::ChangeShowSizeType>("ImageShowStatus::ChangeShowSizeType");
    qRegisterMetaType<Processing::FlipWay>("Processing::FlipWay");

    _clickBeforeStartPosition = QPoint(-1,-1);
    _clickBeforePosition = QPoint(-1,-1);

    _playMovieTimer = new QTimer(this);
    connect(_playMovieTimer,&QTimer::timeout,this,&Core::_playMovie);
}

QString Core::initDbus(const QStringList &arguments)
{
    _dbus = new Dbus;

    //获取注册Dbus服务是否成功
    bool connectSeccess=_dbus->getConnectSeccess();

    //如果注册Dbus服务成功，接收命令
    if(connectSeccess)
        connect(_dbus,&Dbus::processingCommand,this,&Core::_processingCommand);

    //没有参数不处理
    if(arguments.length() <2)
        return "";

    //帮助命令则打印
    if(arguments[1]=="-help" || arguments[1]=="--help" ){
        for(QString &key : Variable::SUPPORT_CMD.keys()){
            qInfo()<<key<<"   "<<Variable::SUPPORT_CMD.value(key);
        }
        exit(0);
        return "";
    }

    //如果是地址且文件存在则打开
    if(QFileInfo::exists(arguments[1])){
        //不是支持的文件格式忽略
        QString format =arguments[1];
        format=format.split(".").last();
        if(!Variable::SUPPORT_FORMATS.contains(format))
            return "";
        return arguments[1];
    }

    //如果是命令
    if(Variable::SUPPORT_CMD.keys().contains(arguments[1])){
        if(connectSeccess)//如果为首个实例不响应
            return "";
        _dbus->argumentsCommand(arguments);
        return "";
    }

    qDebug()<<"参数异常";
    return "";
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
    if(cmd[0] == "-rotate"){
        flipImage(Processing::FlipWay::clockwise);
        return;
    }
}

Mat ImageShowStatus::_changeImage(Mat mat)
{
    if(_backMat.data)
        _backMat.release();
    _backMat = _nowMat;
    _nowMat = mat;
    _imageSize = QString::number(_nowImage.width())+"x"+QString::number(_nowImage.height());
    switch (_nowMat.type()) {
    case CV_8UC4:
        _colorSpace = "RGBA";
        break;
    case CV_8UC3:
        _colorSpace = "RGB";
        break;
    case CV_8UC1:
        _colorSpace = "GRAY";
        break;
    }
    //gif走的是apng流程，所以这里要单独判断
    if(QFileInfo(_nowpath).suffix().toLower() == "gif")
        _colorSpace = "RGB";
    return _nowMat;
}

void ImageShowStatus::_changeImageType(int num)
{
    _matListIndex = 0;
    if(num == 0){//回滚
        _imageUrlMap.remove(_nowType);
        _nowType=_backType;
        _nowpath = _backpath;
        return;
    }
    _backType = _nowType;
    _nowType = num;
    _backpath = _nowpath;
    _nowpath = _imageUrlMap.value(num);
}

QVariant Core::openImage(QString fullPath)
{
    MatAndFileinfo maf = File::loadImage(fullPath);
    if(!maf.mat.data){
        //如果图片打开失败则回滚
        int nextOrBack = 0;//判断向前还是向后切换
        if(_imageUrlMap.values().indexOf(fullPath) > _imageUrlMap.keys().indexOf(_backType))
            nextOrBack = -1;//下一张
        else
            nextOrBack = -2;//上一张
        _changeImageType();
        changeImage(nextOrBack);
        return QVariant();
    }
    //记录状态
    Mat mat = _changeImage(maf.mat);
    _nowImage = Processing::converFormat(mat);
    _info = maf.info;
    if(_matList!=nullptr){
        for(Mat &tmpMat : *_matList){
            tmpMat.release();
        }
        delete _matList;
    }
    _matList = maf.matList;
    _fps = maf.fps;
    _nowpath = fullPath;
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
    package.imageSize = _imageSize;
    package.colorSpace = _colorSpace;
    QVariant var;
    var.setValue<ImageAndInfo>(package);
    emit openFinish(var);
}

void Core::_creatImage(const int &proportion)
{
    if(_nowImage.isNull())
        return;

    unsigned int defaultProportion  = 100 * _size.width() / _nowImage.width();
    if(_nowImage.height() * defaultProportion / 100 > _size.height())
        defaultProportion = 100 * _size.height() / _nowImage.height();

    //自适应窗口大小显示
    if(proportion <= 0){
        _proportion  = defaultProportion;
        _tmpSize = _nowImage.size() * _proportion / 100;
        _navigation(); //关闭导航器
        _showImageOrMovie();
        return;
    }

    _proportion = proportion;
    _tmpSize = _nowImage.size() * _proportion / 100;
    //如果显示比例大于默认比例
    if(_proportion > defaultProportion){
        _navigation(QPoint(0,0));
        return;
    }

    //如果显示比例小于或等于默认比例
    _navigation(); //关闭导航器

    _showImageOrMovie();
}

void Core::_showImageOrMovie()
{
    //动画类格式循环播放
    if(_matList!=nullptr)
        if(_matList->length()>2){
            _playMovie();//立即播放第一帧
            _playMovieTimer->start(_fps);
            return;
        }
    //非动画类格式
    QPixmap pix = Processing::resizePix(_nowImage,_tmpSize);
    _showImage(pix);
}

void Core::_playMovie()
{
    if(_matListIndex >= _matList->length())
        _matListIndex=0;

    Mat mat =_matList->at(_matListIndex);
    QPixmap nowIndexPix = Processing::converFormat(mat);
    _matListIndex++;

    QPixmap pix = Processing::resizePix(nowIndexPix,_tmpSize);
    if(_isNavigationShow){
        QPixmap result = pix.copy(_startShowPoint.x(),_startShowPoint.y(),_size.width(),_size.height());
        _showImage(result);
        return;
    }
    _showImage(pix);
}

void Core::_navigation(const QPoint &point)
{
    _clickBeforeStartPosition = QPoint(-1,-1);
    if( point.x()<0 || point.y()<0 ){//关闭导航器
        _clickBeforePosition = QPoint(-1,-1);
        _isNavigationShow = false;
        emit showNavigation(QPixmap());
        return;
    }
    _isNavigationShow = true;
    _creatNavigation();
    //记录上次放大位置
    clickNavigation();
}

void NavigationStatus::_creatNavigation()
{
    //导航栏背景
    QSize navigationSize = Variable::NAVIGATION_SIZE;
    _navigationImage = Processing::resizePix(_nowImage,navigationSize).toImage();

    //记录空白区域
    _spaceWidth = (navigationSize.width()-_navigationImage.width())/2;
    _spaceHeight = (navigationSize.height()-_navigationImage.height())/2;

    //待显示图
    QSize pixSize = _nowImage.size() * _proportion / 100;
    _showPix = Processing::resizePix(_nowImage,pixSize);

    //高亮区域大小
    _hightlightSize.setWidth(_navigationImage.width() * _size.width() /  _showPix.width());
    _hightlightSize.setHeight(_navigationImage.height() * _size.height() /  _showPix.height());
    if(_hightlightSize.width()>_navigationImage.width())
        _hightlightSize.setWidth(_navigationImage.width());
    if(_hightlightSize.height()>_navigationImage.height())
        _hightlightSize.setHeight(_navigationImage.height());
}

void Core::clickNavigation(const QPoint &point)
{
    bool hasArg = true;
    //无参输入则使用上次的位置
    if(point==QPoint(-1,-1))
        hasArg=false;

    //有参则记录，无参使用上一次的点
    if(hasArg)
        _clickBeforePosition = point;

    //计算点击区域——鼠标要在高亮区域中央，且要减去导航栏窗口与图片边缘的距离
    QPoint startPoint(_clickBeforePosition.x() - _hightlightSize.width() / 2 - _spaceWidth,_clickBeforePosition.y() - _hightlightSize.height() / 2 - _spaceHeight);
    int right = _navigationImage.width() - _hightlightSize.width();//右侧边缘
    int bottom = _navigationImage.height() - _hightlightSize.height();//下侧边缘

    //过滤无效区域
    if(startPoint.x()<0)startPoint.setX(0);
    if(startPoint.y()<0)startPoint.setY(0);
    if(startPoint.x()>right)startPoint.setX(right);
    if(startPoint.y()>bottom)startPoint.setY(bottom);

    //有参情况下，和上次点击的有效区域一致则不处理
    if(startPoint == _clickBeforeStartPosition && hasArg)
        return;
    _clickBeforeStartPosition = startPoint;

    //处理导航器图片
    QImage image = Processing::_pictureDeepen(_navigationImage,_hightlightSize,startPoint);

    //发送到导航器
    emit showNavigation(QPixmap::fromImage(image));

    //处理待显示区域
    _startShowPoint = startPoint * _showPix.width() / _navigationImage.width();

    //如果是动图，则交给动图显示事件去处理，避免闪屏
    if(_playMovieTimer->isActive())
        return;
    QPixmap result = _showPix.copy(_startShowPoint.x(),_startShowPoint.y(),_size.width(),_size.height());
    _showImage(result);
}

void Core::flipImage(const Processing::FlipWay &way)
{
    //如果是动图，则批量处理
    if(_playMovieTimer->isActive()){
        for(int i=0;i<_matList->length();i++){
            Mat mat = Processing::processingImage(Processing::flip,_matList->at(i),QVariant(way));
            _matList->replace(i,mat);
        }
        //刷新导航栏
        _nowImage = Processing::converFormat(_matList->first());
        _navigationImage = Processing::resizePix(_nowImage,Variable::NAVIGATION_SIZE).toImage();
        clickNavigation();
        return;
    }
    Mat mat = Processing::processingImage(Processing::flip,_nowMat,QVariant(way));
    if(!mat.data)
        return;
    File::saveImage(mat,_nowpath);
    mat = _changeImage(mat);
    _nowImage = Processing::converFormat(mat);
    _creatImage();
}

void Core::deleteImage()
{
    File::deleteImage(_nowpath);

    //切换到下一张
    changeImage(-1);

    //从队列中去除
    _imageUrlMap.remove(_backType);

    //删除后队列中无图片，返回状态
    if(_imageUrlMap.isEmpty()){
        _imageUrlMap.clear();
        _maxType = 0;//重置计数
        _nowType = 0;//显示添加图片按钮
        _navigation();//关闭导航器
        _showImage(QPixmap());//发送状态
    }
}

void Core::setAsBackground()
{
    //设置为背景图
    if(QGSettings::isSchemaInstalled(SET_BACKGROUND_PICTURE_GSETTINGS_PATH)){
        QGSettings *background = new QGSettings(SET_BACKGROUND_PICTURE_GSETTINGS_PATH);
        QStringList keyList = background->keys();
        if (keyList.contains(SET_BACKGROUND_PICTURE_GSETTINGS_NAME))
            background->set(SET_BACKGROUND_PICTURE_GSETTINGS_NAME,_nowpath);
        background->deleteLater();
    }
}

void Core::changeImage(const int &type)
{
    //如果图片队列小于2，不处理
    if(_imageUrlMap.size()<2){
        _backType = _nowType;
        _backpath = _nowpath;
        return;
    }

    //如果正在播放动图，则停止
    if(_playMovieTimer->isActive())
        _playMovieTimer->stop();

    QList<unsigned int> list = _imageUrlMap.keys();
    if(type == -1){
        if(_nowType == list.last()){
            //最后一张切下一张时，回到队列开头
            _changeImageType(list.first());
            openImage(_imageUrlMap.first());
            return;
        }
        int key =list[list.indexOf(_nowType)+1];
        _changeImageType(key);
        openImage(_nowpath);
        return;
    }
    if(type == -2){
        if(_nowType == list.first()){
            //第一张切上一张时，回到队列结尾
            _changeImageType(list.last());
            openImage(_imageUrlMap.last());
            return;
        }
        int key =list[list.indexOf(_nowType)-1];
        _changeImageType(key);
        openImage(_nowpath);
        return;
    }
    //如果队列中无此关键值，不处理
    if(list.indexOf(type)<0)
        return;
    _changeImageType(type);
    openImage(_nowpath);
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
    QMap<unsigned int,QString> tmpImageUrlMap;
    for(QString &filename : images){
        QString tmpFullPath = path+"/"+filename;
        _maxType++;
        tmpImageUrlMap.insert(_maxType,tmpFullPath);
        //记录需要显示的图片
        if(tmpFullPath == filepath){
            _backType = _nowType;
            _backpath = _nowpath;
            _nowType = _maxType;
        }
    }
    //新路径中的所有文件靠前排序
    _imageUrlMap.swap(tmpImageUrlMap);
    _imageUrlMap.unite(tmpImageUrlMap);
    QVariant var;
    var.setValue<QList<unsigned int>>(_imageUrlMap.keys());
    _loadAlbum();
    return var;
}

void Core::_loadAlbum()
{
    for(unsigned int &type : _imageUrlMap.keys()){
        AlbumThumbnail* thread= new AlbumThumbnail(type,_imageUrlMap.value(type));
        connect(thread,&AlbumThumbnail::finished,thread,&AlbumThumbnail::deleteLater);
        connect(thread,&AlbumThumbnail::albumFinish,this,&Core::albumFinish);
        thread->start();
    }
}

