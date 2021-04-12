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
    void saveMovieFinish(const QString &path);//异步处理完成
    void close();//关闭进程
    QStandardItemModel * getAlbumModel();//获取相册model指针
    void albumLoadFinish(QVariant var);//预览加载完成

private:
    void initCore();//初始化核心
    Dbus *m_dbus = nullptr;//DBus模块对象
    File *m_file = nullptr;//File模块对象
    void showImage(const QPixmap &pix);//显示图片
    void showImageOrMovie();//显示图片或动图
    void creatImage(const int &proportion = -1);//生成图像
    void processingCommand(const QStringList &cmd);//处理终端命令
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
    bool shouldClose = false;//可以关闭
    void progremExit();//结束进程

};

#endif // CORE_H
