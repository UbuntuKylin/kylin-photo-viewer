#include "core.h"

Core::Core()
{
    initCore();
}

void Core::initCore()
{
    m_file = new File;
    connect(m_file,&File::saveMovieFinish,this,&Core::saveMovieFinish);

    m_matList = nullptr;

    m_albumModel = new QStandardItemModel;

    qRegisterMetaType<ImageShowStatus::ChangeShowSizeType>("ImageShowStatus::ChangeShowSizeType");
    qRegisterMetaType<Processing::FlipWay>("Processing::FlipWay");

    m_clickBeforeStartPosition = QPoint(-1,-1);
    m_clickBeforePosition = QPoint(-1,-1);

    m_playMovieTimer = new QTimer(this);
    connect(m_playMovieTimer,&QTimer::timeout,this,&Core::playMovie);

    m_canProcess = new QTimer(this);
    m_canProcess->setSingleShot(true);
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
    if (arguments[1]=="-help" || arguments[1]=="--help" || arguments[1]=="-h") {
        for (QString &key : Variable::SUPPORT_CMD.keys()) {
            QString str = key + "   " + Variable::SUPPORT_CMD.value(key);
            QStringList list = str.split("\n");
            for (QString &tmpStr : list) {
                qInfo()<<tmpStr;
            }
        }
        exit(0);
        return "";
    }

    //如果是地址且文件存在则打开
    QString path = arguments[1];
    if (path.startsWith("file:///")) {
        path.replace("file:///","/");
    }
    if (QFileInfo::exists(path)) {
        //不是支持的文件格式忽略
        QString format =path;
        format=format.split(".").last();
        if (!Variable::SUPPORT_FORMATS.contains(format)) {
            return "";
        }
        return path;
    }

    if (arguments[1] == "-api") {
        return processingApi(arguments);
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
    if (coreOperateTooOften()) {
        return;
    }
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

QString Core::processingApi(const QStringList &cmd)
{
    if (cmd.length() < 4) {
        return "";
    }

    if (cmd[2] == "-flip") {
        if (cmd.length() < 5) {
            qInfo()<<"参数不合规，使用-h查看帮助";
            return "";
        }
        m_apiCmd.append(cmd[2]);
        if (!QFileInfo::exists(cmd[3])) {
            qInfo()<<"文件不存在";
            return "";
        }
        QString format =cmd[3];
        format=format.split(".").last();
        if (!Variable::SUPPORT_FORMATS.contains(format)) {
            qInfo()<<"不支持的格式";
            return "";
        }
        m_apiCmd.append(cmd[3]);
        QString flipCmd = cmd[4];
        int hNum = flipCmd.count('h');
        hNum = hNum % 2;//保留有效操作
        int vNum = flipCmd.count('v');
        vNum = vNum % 2;
        int cNum = flipCmd.count('c');
        cNum = cNum % 4;
        QString flipCmdResult;
        for (int i = 0;i < hNum;i++) {
            flipCmdResult+="h";
        }
        for (int i = 0;i < vNum;i++) {
            flipCmdResult+="v";
        }
        for (int i = 0;i < cNum;i++) {
            flipCmdResult+="c";
        }
        m_apiCmd.append(flipCmdResult);

        if (cmd.length() > 5) {
            QString saveWay = cmd[5];
            if (saveWay.indexOf('r') > 0) {
                m_apiReplaceFile = true;
            }
        }

        m_isApi = true;
        return Variable::API_TYPE;
    }
    return "";
}

void Core::setHighLight(const QString &path)
{
    for (int i = 0;i<m_albumModel->rowCount();i++) {
        MyStandardItem * item =dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
        if (item->getPath() == path) {
            emit changeAlbumHighLight(item->index());
            return;
        }
    }
}

void Core::openImage(QString fullPath)
{
    //1.文件路径为空；2.从外部删除文件夹下的所有图片时，传空图片列表，不展示图片，回到开始界面
    if (fullPath.isEmpty()) {
        showImage(QPixmap());
        return;
    }


    //如果正在播放动图，则停止
    if (m_playMovieTimer->isActive()) {
        m_playMovieTimer->stop();
    }
//    emit delayShow(true);
    MatAndFileinfo maf = File::loadImage(fullPath);
//    emit delayShow(false);
    if (!maf.mat.data) {
        //如果图片打开失败则回滚
        ChamgeImageType type = nextOrBack(m_backpath,fullPath);
        changeImageType();
        deleteAlbumItem(fullPath);
        //全部图片都被删除了
        if (m_albumModel->rowCount() == 0) {
            showImage(QPixmap());
            return;
        }
        changeImage(type);
        return;
    }
    setHighLight(fullPath);//设置相册选中
    m_nowImage = Processing::converFormat(maf.mat);
    //记录状态
    changeMat(maf.mat);
    m_info = maf.info;
    if (m_matList!=nullptr) {
        m_matList->clear();
    }
    m_matList = maf.matList;
    m_fps = maf.fps;
    m_nowpath = fullPath;
    //如果正在保存，则显示等待
    if (m_file->isSaving(fullPath)) {
        //如果此图正在保存
        m_thisImageIsSaving = true;
        showImage(QPixmap());
        return;
    }
    creatImage();
    return;
}

void Core::showImage(const QPixmap &pix)
{
    ImageAndInfo package;
    package.proportion = m_proportion;
    package.imageNumber = m_albumModel->rowCount();
    package.info = m_info;
    package.image = pix;
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
        if (defaultProportion > 1000) {
            m_proportion = 1000;
        } else {
            m_proportion  = defaultProportion;
        }
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
        if (m_matList->length()>1) {
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

    if (m_isNavigationShow) {

        QPixmap result = Processing::localAmplification(nowIndexPix,m_tmpSize,m_startShowPoint,m_size);
        showImage(result);
        return;
    }
    QPixmap pix = Processing::resizePix(nowIndexPix,m_tmpSize);
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
    m_startShowPoint = startPoint * m_tmpSize.width() / m_navigationImage.width();

    //如果是动图，则交给动图显示事件去处理，避免闪屏
    if (m_playMovieTimer->isActive()) {
        return;
    }
    QPixmap result = Processing::localAmplification(m_nowImage,m_tmpSize,m_startShowPoint,m_size);
    showImage(result);
}

void Core::saveMovieFinish(const QString &path)
{
    //如果收到关闭信号，保存完再退出
    if (m_shouldClose) {
        //双层判断，避免多次确认状态，提升效率
        if (m_file->allSaveFinish()) {
            progremExit();
        }
        return;
    }

    //如果当前播放的不是这张图
    if (m_nowpath != path) {
        return;
    }
    m_thisImageIsSaving = false;
    openImage(path);
}

void Core::flipImage(const Processing::FlipWay &way)
{
    //如果此图正在保存
    if (m_thisImageIsSaving) {
        return;
    }

    m_processed = true;

    //如果是动图，则批量处理
    if (m_playMovieTimer->isActive()) {
        for (int i=0;i<m_matList->length();i++) {
            Mat mat = Processing::processingImage(Processing::flip,m_matList->at(i),QVariant(way));
            m_matList->replace(i,mat);
        }
        //刷新导航栏
        m_nowImage = Processing::converFormat(m_matList->first());
        creatImage();
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
    //如果此图正在保存
    if (m_thisImageIsSaving) {
        return;
    }

    m_processed=false;//重置是否操作过的状态,都已经删除了，就没必要存了

    //切换到下一张
    changeImage(NEXT_IMAGE);

    //从队列中去除
    deleteAlbumItem(m_backpath);
    //删除文件
    m_file->deleteImage(m_backpath);

    emit setHighLight(m_nowpath);

    //删除后队列中无图片，返回状态
    if(m_albumModel->rowCount() == 0){
        navigation();//关闭导航器
        showImage(QPixmap());//发送状态
        return;
    }
}

void Core::setAsBackground()
{
    //设置为背景图
    if (QGSettings::isSchemaInstalled(SET_BACKGROUND_PICTURE_GSETTINGS_PATH)) {
        QGSettings *background = new QGSettings(SET_BACKGROUND_PICTURE_GSETTINGS_PATH);
        QStringList keyList = background->keys();
        if (keyList.contains(SET_BACKGROUND_PICTURE_GSETTINGS_NAME)) {
            QString nowPath =  background->get(SET_BACKGROUND_PICTURE_GSETTINGS_NAME).toString();
            if (nowPath == m_nowpath) {
                background->set(SET_BACKGROUND_PICTURE_GSETTINGS_NAME,QString(""));
            }
            background->set(SET_BACKGROUND_PICTURE_GSETTINGS_NAME,m_nowpath);
        }
        background->deleteLater();
    }
}

void Core::openInfile()
{
    QProcess process;
    process.start("peony --show-items " + m_nowpath);
    process.waitForFinished();
    process.waitForReadyRead();
    process.close();
}

void Core::close()
{
    //如果已经触发过关闭事件不响应
    if (m_shouldClose) {
        return;
    }

    //如果没有操作且有正在保存的动图直接退出
    if (!m_processed) {
        if (!m_file->allSaveFinish()) {
            if (m_playMovieTimer->isActive()) {
                m_playMovieTimer->stop();
            }
            m_shouldClose = true;
            showImage(QPixmap());
            return;
        }
        progremExit();
        return;
    }
    //如果不是作为外部API调用则默认覆盖保存
    if (!m_isApi) {
        m_apiReplaceFile = true;
    }
    //如果正在播放动图，则停止
    if (m_playMovieTimer->isActive()) {
        m_playMovieTimer->stop();
        showImage(QPixmap());
        //保存动图
        m_file->saveImage(m_matList,m_fps,m_nowpath,m_apiReplaceFile);
        m_shouldClose = true;
    } else {
        QString suffix = QFileInfo(m_nowpath).suffix().toLower();
        //保存图片
        m_file->saveImage(m_nowMat,m_nowpath,m_apiReplaceFile);
        if (suffix == "apng" || suffix == "png" || suffix == "gif") {
            m_shouldClose = true;
            return;
        }
        progremExit();
    }
}

QStandardItemModel *Core::getAlbumModel()
{
    return m_albumModel;
}

void Core::changeImage(const int &type)
{
    //如果错误不处理
    if (type == ERROR_IMAGE) {
        return;
    }

    //如果图片队列小于1，不处理。小于2时，相册文件清空时，会一直显示当前图片
    if (m_albumModel->rowCount()<1) {
        m_backpath = m_nowpath;
        return;
    }

    m_thisImageIsSaving = false;

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
        QString key = nextImagePath(m_nowpath);
        changeImageType(key);
        openImage(m_nowpath);
        return;
    }
    if (type == BACK_IMAGE) {
        QString key = backImagePath(m_nowpath);
        changeImageType(key);
        openImage(m_nowpath);
        return;
    }
}

void Core::changeImageFromClick(QModelIndex modelIndex)
{
    MyStandardItem *item = dynamic_cast<MyStandardItem *>(m_albumModel->itemFromIndex(modelIndex));
    if (item->getPath() == m_nowpath) {
        return;
    }
    openImage(item->getPath());
}

void Core::changeWidgetSize(const QSize &size)
{
    m_size = size;
    creatImage();
}

void Core::changeImageShowSize(ImageShowStatus::ChangeShowSizeType type)
{
    //如果此图正在保存
    if (m_thisImageIsSaving) {
        return;
    }

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

void Core::changeImageType(QString path)
{
    m_matListIndex = 0;
    if (path == "") {//回滚
        deleteAlbumItem(m_nowpath);
        if (m_albumModel->rowCount() == 0) {
            m_backpath = m_nowpath;
            m_nowpath = "";
            return;
        }
        m_nowpath = m_backpath;
        return;
    }
    m_backpath = m_nowpath;
    m_nowpath = path;
}

Mat Core::changeMat(Mat mat)
{
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
    m_tmpSize = m_nowImage.size() * m_proportion / 100;

    //高亮区域大小
    m_hightlightSize.setWidth(m_navigationImage.width() * m_size.width() /  m_tmpSize.width());
    m_hightlightSize.setHeight(m_navigationImage.height() * m_size.height() /  m_tmpSize.height());
    if (m_hightlightSize.width()>m_navigationImage.width()) {
        m_hightlightSize.setWidth(m_navigationImage.width());
    }
    if (m_hightlightSize.height()>m_navigationImage.height()) {
        m_hightlightSize.setHeight(m_navigationImage.height());
    }
}

void Core::deleteAlbumItem(const QString &path)
{
    if (path == "") {
        return;
    }
    for (int i = 0;i<m_albumModel->rowCount();i++) {
        MyStandardItem * item = dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
        if (item->getPath() == path) {
            m_albumModel->removeRow(i);
            return;
        }
    }
}

Enums::ChamgeImageType Core::nextOrBack(const QString &oldPath, const QString &newPath)
{
    for (int i = 0;i<m_albumModel->rowCount();i++) {
        MyStandardItem * item = dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
        if (item->getPath() == oldPath) {
            return NEXT_IMAGE;
        }
        if (item->getPath() == newPath) {
            return BACK_IMAGE;
        }
    }
    qDebug()<<"判断切换状态错误";
    return ERROR_IMAGE;
}

QString Core::nextImagePath(const QString &oldPath)
{
    //相册中没有文件时，不进行处理
    if (m_albumModel->rowCount()<1) {
        return "";
    }
    MyStandardItem * item =nullptr;
    //最后一张切下一张时，回到队列开头
    item = dynamic_cast<MyStandardItem *>(m_albumModel->item(m_albumModel->rowCount()-1));
    if (item->getPath() == oldPath) {
        item = dynamic_cast<MyStandardItem *>(m_albumModel->item(0));
        return item->getPath();
    }

    bool tmp = false;
    for (int i = 0;i<m_albumModel->rowCount();i++) {
        item = dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
        if (tmp) {
            return item->getPath();
        }
        if (item->getPath() == oldPath) {
            tmp = true;
        }
    }
    return "";
}

QString Core::backImagePath(const QString &oldPath)
{
    //相册中没有文件时，不进行处理
    if (m_albumModel->rowCount()<1) {
        return "";
    }
    MyStandardItem * item =nullptr;
    //第一张切上一张时，回到队列结尾
    item = dynamic_cast<MyStandardItem *>(m_albumModel->item(0));
    if (item->getPath() == oldPath) {
        item = dynamic_cast<MyStandardItem *>(m_albumModel->item(m_albumModel->rowCount()-1));
        return item->getPath();
    }

    QString tmp = "";
    for (int i = 0;i<m_albumModel->rowCount();i++) {
        item = dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
        QString tmp2 = item->getPath();
        if (tmp2 == oldPath) {
            return tmp;
        }
        tmp = tmp2;
    }
    return "";
}

void Core::progremExit()
{
    this->deleteLater();
    emit coreProgremExit();
}

void Core::albumLoadFinish(QVariant var)
{
    ImageAndInfo package = var.value<ImageAndInfo>();
    QString fileName = package.info.absoluteFilePath();
    for (int i = 0;i<m_albumModel->rowCount();i++) {
        MyStandardItem * item = dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
        if (item->getPath() == fileName) {
            if (package.image.isNull()) {
                m_albumModel->removeRow(i);
                return;
            }
            m_albumModel->item(i)->setIcon(package.image);
            return;
        }
    }
}

bool Core::apiFunction()
{
    if (!m_isApi) {
       return false;
    }
    QString fullPath = m_apiCmd[1];
    openImage(fullPath);
    //翻转
    if (m_apiCmd[0]=="-flip") {
        QString cmd = m_apiCmd[2];
        for (QChar &ch : cmd) {
            if (ch == 'h') {
                flipImage(Processing::FlipWay::horizontal);
            } else if (ch == 'v') {
                flipImage(Processing::FlipWay::vertical);
            } else if (ch == 'c') {
                flipImage(Processing::FlipWay::clockwise);
            }
        }
        close();
    }
    return true;
}
//判断是不是一个文件路径
bool Core::isSamePath(QString path)
{

    QString pathDir = QFileInfo(path).absolutePath();
    int num = m_albumModel->rowCount();
    if (num > 0) {
        for (int i = 0 ; i < num;i++) {
            MyStandardItem * item = dynamic_cast<MyStandardItem *>(m_albumModel->item(i));
            if (item->getPath().mid(0,item->getPath().lastIndexOf("/")) == pathDir) {
                    return true;
                } else {
                return false;
            }
      }
    } else {
        return false;
    }

}


void Core::findAllImageFromDir(QString fullPath)
{
    if (apiFunction()) {
        return;
    }
    if (isSamePath(fullPath)) {
        openImage(fullPath);
        return;
    }

    QFileInfo info(fullPath);
    QString path = info.absolutePath();//转成绝对路径
    QString filepath = info.absoluteFilePath();//转成绝对路径
    QDir dir(path);//实例化目录对象
    QStringList nameFilters;//格式过滤
    for (const QString &format : Variable::SUPPORT_FORMATS) {
        nameFilters<<"*."+format;//构造格式过滤列表
    }
    QStringList images = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);//获取所有支持的图片

    loadAlbum(path,images);

    openImage(filepath);
}

void Core::loadAlbum(QString path, QStringList list)
{
    //将所有图片存入队列
    int i = 0;
    for (QString &filename : list) {
        QString tmpFullPath = path+"/"+filename;
        //构造item
        MyStandardItem * item  = new MyStandardItem;
        item->setDragEnabled(false);
        item->setSizeHint(Variable::ALBUM_IMAGE_SIZE);
        item->setIcon(m_defultPixmap);

        item->setPath(tmpFullPath);//用来保存地址路径
        m_albumModel->insertRow(i,item);//插入model
        //加载缩略图
        AlbumThumbnail* thread= new AlbumThumbnail(tmpFullPath);
        connect(thread,&AlbumThumbnail::finished,thread,&AlbumThumbnail::deleteLater);
        connect(thread,&AlbumThumbnail::albumFinish,this,&Core::albumLoadFinish);
        thread->start();
        i++;
    }
}

bool Core::coreOperateTooOften()
{
    if (m_canProcess->isActive()) {
        return true;
    }
    m_canProcess->start(Variable::REFRESH_RATE);//刷新间隔
    return false;
}

void MyStandardItem::setPath(const QString &path)
{
    m_path = path;
}

QString MyStandardItem::getPath()
{
    return m_path;
}
