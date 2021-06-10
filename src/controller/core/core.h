#ifndef CORE_H
#define CORE_H

#include "base.h"

#define SET_BACKGROUND_PICTURE_GSETTINGS_PATH "org.mate.background"
#define SET_BACKGROUND_PICTURE_GSETTINGS_NAME "pictureFilename"

class MyStandardItem :public QStandardItem
{
public:
    void setPath(const QString &path);
    QString getPath();
private:
    QString m_path;
};

class Core : public QObject , public NavigationStatus
{
    Q_OBJECT

signals:
    void needOpenImage(QString path);//启动时打开图片
    void openFinish(QVariant var);//打开完成
    void showNavigation(QPixmap pix);//操作导航器
    void coreProgremExit();//结束进程
    void changeAlbumHighLight(QModelIndex modelIndex);//在相册中选中到所切换的图片
    void delayShow(bool isLoading);//处理图片加载缓慢问题
    void openFromAlbum();//从相册打开

public:
    Core();
    QString initDbus(const QStringList &arguments);//初始化Dbus模块
    void findAllImageFromDir(QString fullPath);//寻找目录下所有支持的图片
    void openImage(QString fullPath);//打开图片
    void changeImage(const int &type); //切换图片
    void changeImageFromClick(QModelIndex modelIndex); //切换图片
    void changeWidgetSize(const QSize &size);//切换窗口大小
    void changeImageShowSize(ImageShowStatus::ChangeShowSizeType type);//图片显示状态（放大缩小）
    void clickNavigation(const QPoint &point = QPoint(-1,-1));//导航器点击
    void flipImage(const Processing::FlipWay &way);//翻转处理
    void deleteImage();//删除图片
    void setAsBackground();//设置为背景图
    void openInfile();//文件夹中打开
    void changeOpenIcon(QString theme);//主题改变时切换相册打开按钮
    void saveMovieFinish(const QString &path);//异步处理完成
    void close();//关闭进程
    QStandardItemModel * getAlbumModel();//获取相册model指针
    void albumLoadFinish(QVariant var);//预览加载完成
    void toCoreChangeName(QString oldName,QFileInfo newFile);//接收重命名的处理

private:
    void initCore();//初始化核心
    Dbus *m_dbus = nullptr;//DBus模块对象
    File *m_file = nullptr;//File模块对象
    void showImage(const QPixmap &pix);//显示图片
    void showImageOrMovie();//显示图片或动图
    void creatImage(const int &proportion = -1, bool defaultOpen = true);//生成图像，defaultOpen区分是否是打开图片，无其他操作
    void defaultImage(int proportion,int defaultProportion);//默认显示图片原则
    void operateImage(int proportion,int defaultProportion);//进行操作显示图片原则
    void processingCommand(const QStringList &cmd);//处理终端命令
    QString processingApi(const QStringList &cmd);//处理外部命令
    void loadAlbum(QString path, QStringList list);//加载相册
    void navigation(const QPoint &point = QPoint(-1,-1));//导航器
    void playMovie();//播放动图的槽函数
    inline void changeImageType(QString path = "");//修改图片标签
    Mat changeMat(Mat mat);//更改当前图片
    void creatNavigation();//创建导航器图片等数据，用于节省算力
    void deleteAlbumItem(const QString &path);//删除相册中的项
    ChamgeImageType nextOrBack(const QString &oldPath,const QString &newPath);
    QString nextImagePath(const QString & oldPath);
    QString backImagePath(const QString & oldPath);
    void setHighLight(const QString &path);
    bool m_shouldClose = false;//可以关闭
    bool m_isApi = false;//作为API接口运行
    bool m_apiReplaceFile = false;//api替换原图
    QStringList m_apiCmd;//保存操作命令
    QTimer *m_canProcess = nullptr;//限制操作频率，降低cpu占用
    bool coreOperateTooOften();//操作过于频繁
    void progremExit();//结束进程
    bool apiFunction();//处理api函数
    bool isSamePath(QString path);//判断打开的是不是相同路径
    void needSave();//判断是否需要保存图片
    bool m_isclose = false;
    MyStandardItem *m_item0  = nullptr;


};

#endif // CORE_H
