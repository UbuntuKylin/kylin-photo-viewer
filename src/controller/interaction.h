#ifndef INTERACTION_H
#define INTERACTION_H

#include <QObject>
#include <QPixmap>
#include <QVariant>

class _Interaction;

class Interaction : public QObject
{
    Q_OBJECT

signals:
    void startWithOpenImage(QString);//启动时打开图片
    void openFinish(QVariant var);//图片打开完成
    void albumFinish(QVariant var);//相册缩略图加载完成
    void showNavigation(QPixmap pix);//显示或隐藏导航栏事件
    void deleteImageOnAlbum(QList<int> list,int type);//从相册中删除图片事件（删除图片/图片打开失败）
    void processingFinish(bool);//正在处理图片或处理完成状态

public:
    static Interaction *getInstance();//单例取指针
    virtual void creatCore(const QStringList &list)=0;//初始化核心模块
    virtual void initUiFinish()=0;//UI初始化完成
    virtual QVariant openImage(const QString &path)=0;//打开图片，返回相册标签列表
    virtual void changeImage(const int &type)=0;//切换图片
    virtual void nextImage()=0;//切换图片
    virtual void backImage()=0;//切换图片
    virtual void changeWidgetSize(const QSize &size)=0;//切换窗口大小
    virtual void watchBigImage()=0;//图片放大
    virtual void watchSmallImage()=0;//图片缩小
    virtual void watchOriginalImage()=0;//查看原图
    virtual void watchAutoImage()=0;//自适应窗口大小
    virtual void clickNavigation(const QPoint &point)=0;//导航器点击
    virtual void rotate(const bool &direction = true)=0;//旋转
    virtual void flipH()=0;//水平翻转
    virtual void flipV()=0;//垂直翻转
    virtual void deleteImage()=0;//删除图片
    virtual void setAsBackground()=0;//设置为背景图

private:
    static Interaction *m_interaction;//单例指针
};


#endif // INTERACTION_H
