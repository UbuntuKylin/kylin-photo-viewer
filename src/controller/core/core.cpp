#include "core.h"

Core::Core()
{
    initCore();
}

void Core::initCore()
{
    m_file = new File;
    connect(m_file,&File::saveFinish,this,&Core::saveFinishSlot);

    m_matList = nullptr;

    qRegisterMetaType<ImageShowStatus::ChangeShowSizeType>("ImageShowStatus::ChangeShowSizeType");
    qRegisterMetaType<Processing::FlipWay>("Processing::FlipWay");

    m_clickBeforeStartPosition = QPoint(-1,-1);
    m_clickBeforePosition = QPoint(-1,-1);

    m_playMovieTimer = new QTimer(this);
    connect(m_playMovieTimer,&QTimer::timeout,this,&Core::playMovie);
}

QString Core::initDbus(const QStringList &arguments)
{
    m_dbus = new Dbus;

    //获取注册Dbus服务是否成功
    bool connectSeccess=m_dbus->getConnectSeccess();

    //如果注册Dbus服务成功，接收命令
    if (connectSeccess) {
        connect(m_dbus,&Dbus::processingCommand,this,&Core::processingCommand);
    }

    //没有参数不处理
    if (arguments.length() <2) {
        return "";
    }

    //帮助命令则打印
    if (arguments[1]=="-help" || arguments[1]=="--help" ) {
        for (QString &key : Variable::SUPPORT_CMD.keys()) {
            qInfo()<<key<<"   "<<Variable::SUPPORT_CMD.value(key);
        }
        exit(0);
        return "";
    }

    //如果是地址且文件存在则打开
    if (QFileInfo::exists(arguments[1])) {
        //不是支持的文件格式忽略
        QString format =arguments[1];
        format=format.split(".").last();
        if (!Variable::SUPPORT_FORMATS.contains(format)) {
            return "";
        }
        return arguments[1];
    }

    //如果是命令
    if (Variable::SUPPORT_CMD.keys().contains(arguments[1])) {
        if (connectSeccess) {//如果为首个实例不响应
            return "";
        }
        m_dbus->argumentsCommand(arguments);
        return "";
    }

    qDebug()<<"参数异常";
    return "";
}

void Core::processingCommand(const QStringList &cmd)
{
    qDebug()<<"响应外部命令"<<cmd;

    if (cmd[0] == "-next") {
        changeImage(NEXT_IMAGE);
        return;
    }
    if (cmd[0] == "-back") {
        changeImage(BACK_IMAGE);
        return;
    }
    if (cmd[0] == "-big") {
        changeImageShowSize(ImageShowStatus::BIG);
        return;
    }
    if (cmd[0] == "-small") {
        changeImageShowSize(ImageShowStatus::SMALL);
        return;
    }
    if (cmd[0] == "-origin") {
        changeImageShowSize(ImageShowStatus::ORIGIN);
        return;
    }
    if (cmd[0] == "-auto") {
        changeImageShowSize(ImageShowStatus::AUTO);
        return;
    }
    if (cmd[0] == "-rotate") {
        flipImage(Processing::FlipWay::clockwise);
        return;
    }
}

QVariant Core::openImage(QString fullPath)
{
    if (fullPath.isEmpty()) {
        return QVariant();
    }

    //如果正在播放动图，则停止
    if (m_playMovieTimer->isActive()) {
        m_playMovieTimer->stop();
    }

    MatAndFileinfo maf = File::loadImage(fullPath);
    if (!maf.mat.data) {
        //如果图片打开失败则回滚
        ChamgeImageType type = m_imageUrlList.nextOrBack(m_backpath,fullPath);
        changeImageType();
        emit deleteImageOnAlbum(m_nowType);
        //全部图片都被删除了
        if (m_nowType == 0) {
            showImage(QPixmap());
            return QVariant();
        }
        changeImage(type);
        return QVariant();
    }
    m_nowImage = Processing::converFormat(maf.mat);
    //记录状态
    changeMat(maf.mat);
    m_info = maf.info;
    if (m_matList!=nullptr) {
        for (Mat &tmpMat : *m_matList) {
            tmpMat.release();
        }
        delete m_matList;
    }
    m_matList = maf.matList;
    m_fps = maf.fps;
    m_nowpath = fullPath;
    creatImage();
    return QVariant();
}

void Core::showImage(const QPixmap &pix)
{
    ImageAndInfo package;
    package.proportion = m_proportion;
    package.info = m_info;
    package.image = pix;
    package.type = m_nowType;
    package.imageSize = m_imageSize;
    package.colorSpace = m_colorSpace;
    QVariant var;
    var.setValue<ImageAndInfo>(package);
    emit openFinish(var);
}

void Core::creatImage(const int &proportion)
{
    if (m_nowImage.isNull()) {
        return;
    }

    int defaultProportion  = 100 * m_size.width() / m_nowImage.width();
    if (m_nowImage.height() * defaultProportion / 100 > m_size.height()) {
        defaultProportion = 100 * m_size.height() / m_nowImage.height();
    }

    //自适应窗口大小显示
    if (proportion <= 0) {
        m_proportion  = defaultProportion;
        m_tmpSize = m_nowImage.size() * m_proportion / 100;
        navigation(); //关闭导航器
        showImageOrMovie();
        return;
    }

    m_proportion = proportion;
    m_tmpSize = m_nowImage.size() * m_proportion / 100;
    //如果显示比例大于默认比例
    if (m_proportion > defaultProportion) {
        navigation(QPoint(0,0));
        return;
    }

    //如果显示比例小于或等于默认比例
    navigation(); //关闭导航器

    showImageOrMovie();
}

void Core::showImageOrMovie()
{
    //动画类格式循环播放
    if (m_matList!=nullptr) {
        if (m_matList->length()>2) {
            playMovie();//立即播放第一帧
            m_playMovieTimer->start(m_fps);
            return;
        }
    }
    //非动画类格式
    QPixmap pix = Processing::resizePix(m_nowImage,m_tmpSize);
    showImage(pix);
}

void Core::playMovie()
{
    if (m_matListIndex >= m_matList->length()) {
        m_matListIndex=0;
    }

    Mat mat =m_matList->at(m_matListIndex);
    QPixmap nowIndexPix = Processing::converFormat(mat);
    m_matListIndex++;

    QPixmap pix = Processing::resizePix(nowIndexPix,m_tmpSize);
    if (m_isNavigationShow) {
        QPixmap result = pix.copy(m_startShowPoint.x(),m_startShowPoint.y(),m_size.width(),m_size.height());
        showImage(result);
        return;
    }
    showImage(pix);
}

void Core::navigation(const QPoint &point)
{
    m_clickBeforeStartPosition = QPoint(-1,-1);
    if ( point.x()<0 || point.y()<0 ) {//关闭导航器
        m_clickBeforePosition = QPoint(-1,-1);
        m_isNavigationShow = false;
        emit showNavigation(QPixmap());
        return;
    }
    m_isNavigationShow = true;
    creatNavigation();
    //记录上次放大位置
    clickNavigation();
}

void Core::clickNavigation(const QPoint &point)
{
    bool hasArg = true;
    //无参输入则使用上次的位置
    if (point==QPoint(-1,-1)) {
        hasArg=false;
    }

    //有参则记录，无参使用上一次的点
    if (hasArg) {
        m_clickBeforePosition = point;
    }

    //计算点击区域——鼠标要在高亮区域中央，且要减去导航栏窗口与图片边缘的距离
    QPoint startPoint(m_clickBeforePosition.x() - m_hightlightSize.width() / 2 - m_spaceWidth,m_clickBeforePosition.y() - m_hightlightSize.height() / 2 - m_spaceHeight);
    int right = m_navigationImage.width() - m_hightlightSize.width();//右侧边缘
    int bottom = m_navigationImage.height() - m_hightlightSize.height();//下侧边缘

    //过滤无效区域
    if (startPoint.x()<0) {
        startPoint.setX(0);
    }
    if (startPoint.y()<0) {
        startPoint.setY(0);
    }
    if (startPoint.x()>right) {
        startPoint.setX(right);
    }
    if (startPoint.y()>bottom) {
        startPoint.setY(bottom);
    }

    //有参情况下，和上次点击的有效区域一致则不处理
    if (startPoint == m_clickBeforeStartPosition && hasArg) {
        return;
    }
    m_clickBeforeStartPosition = startPoint;

    //处理导航器图片
    QImage image = Processing::pictureDeepen(m_navigationImage,m_hightlightSize,startPoint);

    //发送到导航器
    emit showNavigation(QPixmap::fromImage(image));

    //处理待显示区域
    m_startShowPoint = startPoint * m_showPix.width() / m_navigationImage.width();

    //如果是动图，则交给动图显示事件去处理，避免闪屏
    if (m_playMovieTimer->isActive()) {
        return;
    }
    QPixmap result = m_showPix.copy(m_startShowPoint.x(),m_startShowPoint.y(),m_size.width(),m_size.height());
    showImage(result);
}

void Core::saveFinishSlot()
{
    m_isProcessingFinish=true;
    //emit processingFinish(true);
}

void Core::flipImage(const Processing::FlipWay &way)
{
    m_processed = true;

    //如果是动图，则批量处理
    if (m_playMovieTimer->isActive()) {
        for (int i=0;i<m_matList->length();i++) {
            Mat mat = Processing::processingImage(Processing::flip,m_matList->at(i),QVariant(way));
            m_matList->replace(i,mat);
        }
        //刷新导航栏
        m_nowImage = Processing::converFormat(m_matList->first());
        creatImage(m_proportion);
        return;
    }
    Mat mat = Processing::processingImage(Processing::flip,m_nowMat,QVariant(way));
    if (!mat.data) {
        return;
    }
    mat = changeMat(mat);
    m_nowImage = Processing::converFormat(mat);
    creatImage();
}

void Core::deleteImage()
{
    File::deleteImage(m_nowpath);

    //切换到下一张
    changeImage(NEXT_IMAGE);

    //从队列中去除
    m_imageUrlList.remove(m_backType);
    emit deleteImageOnAlbum(m_backType);

    //删除后队列中无图片，返回状态
    if(m_imageUrlList.isEmpty()){
        m_imageUrlList.clear();
        m_maxType = 0;//重置计数
        m_nowType = 0;//显示添加图片按钮
        navigation();//关闭导航器
        showImage(QPixmap());//发送状态
    }
}

void Core::setAsBackground()
{
    //设置为背景图
    if (QGSettings::isSchemaInstalled(SET_BACKGROUND_PICTURE_GSETTINGS_PATH)) {
        QGSettings *background = new QGSettings(SET_BACKGROUND_PICTURE_GSETTINGS_PATH);
        QStringList keyList = background->keys();
        if (keyList.contains(SET_BACKGROUND_PICTURE_GSETTINGS_NAME)) {
            background->set(SET_BACKGROUND_PICTURE_GSETTINGS_NAME,m_nowpath);
        }
        background->deleteLater();
    }
}

void Core::close()
{
    //如果正在播放动图，则停止
    if (m_playMovieTimer->isActive()) {
        m_playMovieTimer->stop();
        //保存动图
        m_file->saveImage(m_matList,m_fps,m_nowpath);
    } else {
        //保存图片
        m_file->saveImage(m_nowMat,m_nowpath);
    }
}

void Core::changeImage(const int &type)
{
    //如果图片队列小于2，不处理
    if (m_imageUrlList.length()<1) {
        m_backType = m_nowType;
        m_backpath = m_nowpath;
        return;
    }

//    //有未处理完成的指令则不处理
//    if (!m_isProcessingFinish) {
//        return;
//    }
//    //更改界面按钮状态
//    m_isProcessingFinish=false;

    //如果正在播放动图，则停止
    if (m_playMovieTimer->isActive()) {
        m_playMovieTimer->stop();
        //保存动图
        if (m_processed) {
            m_file->saveImage(m_matList,m_fps,m_nowpath);
        }
    } else {
        //保存图片
        if (m_processed) {
            m_file->saveImage(m_nowMat,m_nowpath);
        }
    }

    m_processed=false;//重置是否操作过的状态

    if (type == NEXT_IMAGE) {
        int key = m_imageUrlList.nextKey(m_nowType);
        changeImageType(key);
        openImage(m_nowpath);
        return;
    }
    if (type == BACK_IMAGE) {
        int key = m_imageUrlList.backKey(m_nowType);
        changeImageType(key);
        openImage(m_nowpath);
        return;
    }

    //如果队列中无此关键值，不处理
    if (m_imageUrlList.keys().indexOf(type)<0) {
        return;
    }

    changeImageType(type);
    openImage(m_nowpath);
}

void Core::changeWidgetSize(const QSize &size)
{
    m_size = size;
    creatImage(m_proportion);
}

void Core::changeImageShowSize(ImageShowStatus::ChangeShowSizeType type)
{
    int resizeKey = Variable::RESIZE_KEY;
    int tmpProportion = 0;
    switch (type) {

    ///----------------------------放大图片----------------------------
    case ImageShowStatus::BIG:
        if (m_proportion == Variable::RESIZE_KEY_MAX) {//等于临界值
            return;
        }
        if (m_proportion + resizeKey > Variable::RESIZE_KEY_MAX) {//将要超出临界值
            tmpProportion = Variable::RESIZE_KEY_MAX;
            creatImage(tmpProportion);
            return;
        }
        if (m_proportion % resizeKey != 0) { //不能整除
            tmpProportion = (m_proportion/resizeKey + 1)*resizeKey;
            creatImage(tmpProportion);
            return;
        }
        tmpProportion = m_proportion + resizeKey;
        creatImage(tmpProportion);
        return;

        ///----------------------------缩小图片----------------------------
    case ImageShowStatus::SMALL:
        if (m_proportion == Variable::RESIZE_KEY_MIN) {//等于临界值
            return;
        }
        if (m_proportion - resizeKey < Variable::RESIZE_KEY_MIN) {//将要超出临界值
            tmpProportion = Variable::RESIZE_KEY_MIN;
            creatImage(tmpProportion);
            return;
        }
        if (m_proportion % resizeKey != 0) { //不能整除
            tmpProportion = (m_proportion/resizeKey)*resizeKey;
            creatImage(tmpProportion);
            return;
        }
        tmpProportion = m_proportion - resizeKey;
        creatImage(tmpProportion);
        return;

        ///----------------------------查看原图----------------------------
    case ImageShowStatus::ORIGIN:
        creatImage(100);
        return;

        ///----------------------------查看自适应图----------------------------
    case ImageShowStatus::AUTO:
        creatImage();
        return;

    default:
        qDebug()<<"代码错误，请检查";
        break;
    }
}

void Core::changeImageType(int num)
{
    m_matListIndex = 0;
    if (num == 0) {//回滚
        m_imageUrlList.remove(m_nowType);
        m_nowType=m_backType;
        m_nowpath = m_backpath;
        if (m_imageUrlList.isEmpty()) {
            m_nowType=0;
            m_nowpath = "";
        }
        return;
    }
    m_backType = m_nowType;
    m_nowType = num;
    m_backpath = m_nowpath;
    m_nowpath = m_imageUrlList.getPath(num);
}

Mat Core::changeMat(Mat mat)
{
    if (m_backMat.data) {
        m_backMat.release();
    }
    m_backMat = m_nowMat;
    m_nowMat = mat;
    m_imageSize = QString::number(m_nowImage.width())+"x"+QString::number(m_nowImage.height());
    switch (m_nowMat.type()) {
    case CV_8UC4:
        m_colorSpace = "RGBA";
        break;
    case CV_8UC3:
        m_colorSpace = "RGB";
        break;
    case CV_8UC1:
        m_colorSpace = "GRAY";
        break;
    }
    //gif走的是apng流程，所以这里要单独判断
    if (QFileInfo(m_nowpath).suffix().toLower() == "gif") {
        m_colorSpace = "RGB";
    }
    return m_nowMat;
}

void Core::creatNavigation()
{
    //导航栏背景
    QSize navigationSize = Variable::NAVIGATION_SIZE;
    m_navigationImage = Processing::resizePix(m_nowImage,navigationSize).toImage();

    //记录空白区域
    m_spaceWidth = (navigationSize.width()-m_navigationImage.width())/2;
    m_spaceHeight = (navigationSize.height()-m_navigationImage.height())/2;

    //待显示图
    QSize pixSize = m_nowImage.size() * m_proportion / 100;
    m_showPix = Processing::resizePix(m_nowImage,pixSize);

    //高亮区域大小
    m_hightlightSize.setWidth(m_navigationImage.width() * m_size.width() /  m_showPix.width());
    m_hightlightSize.setHeight(m_navigationImage.height() * m_size.height() /  m_showPix.height());
    if (m_hightlightSize.width()>m_navigationImage.width()) {
        m_hightlightSize.setWidth(m_navigationImage.width());
    }
    if (m_hightlightSize.height()>m_navigationImage.height()) {
        m_hightlightSize.setHeight(m_navigationImage.height());
    }
}

QVariant Core::findAllImageFromDir(QString fullPath)
{
    QFileInfo info(fullPath);
    QString path = info.absolutePath();//转成绝对路径
    QString filepath = info.absoluteFilePath();//转成绝对路径
    QDir dir(path);//实例化目录对象
    QStringList nameFilters;//格式过滤
    for (const QString &format : Variable::SUPPORT_FORMATS) {
        nameFilters<<"*."+format;//构造格式过滤列表
    }
    QStringList images = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);//获取所有支持的图片
    //将所有图片打上唯一标签并存入队列
    ImageUrlAndTypeList tmpImageUrlList;
    for (QString &filename : images) {
        QString tmpFullPath = path+"/"+filename;
        m_maxType++;
        tmpImageUrlList.append(m_maxType,tmpFullPath);
        //记录需要显示的图片
        if (tmpFullPath == filepath) {
            m_backType = m_nowType;
            m_backpath = m_nowpath;
            m_nowType = m_maxType;
        }
    }
    //新路径中的所有文件靠前排序
    m_imageUrlList.append(tmpImageUrlList);
    QVariant var;
    var.setValue<QList<int>>(m_imageUrlList.keys());
    loadAlbum();
    return var;
}

void Core::loadAlbum()
{
    for (int &k : m_imageUrlList.keys()) {
        AlbumThumbnail* thread= new AlbumThumbnail(k,m_imageUrlList.getPath(k));
        connect(thread,&AlbumThumbnail::finished,thread,&AlbumThumbnail::deleteLater);
        connect(thread,&AlbumThumbnail::albumFinish,this,&Core::albumFinish);
        thread->start();
    }
}

