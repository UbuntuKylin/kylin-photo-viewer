#ifndef INTERACTION_H
#define INTERACTION_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "core/core.h"

//对外暴露接口
class Interaction : public QObject
{
    Q_OBJECT
signals:
    void startWithOpenImage(QString);
    void openFinish(QVariant var);
    void albumFinish(QVariant var);
    void showNavigation(QPixmap pix);

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
private:
    static Interaction *m_interaction;//单例指针
};



//实际实现功能
class _Interaction : public Interaction
{
    Q_OBJECT

signals:
    QVariant _openImage(const QString &path);//打开图片  QVariant -> QHash<int,QList<int>>
    void _changeImage(const int &type); //切换图片
    void _changeWidgetSize(const QSize &size);//切换窗口大小
    void _changeImageShowSize(ImageShowStatus::ChangeShowSizeType);//图片显示状态（放大缩小）
    void _clickNavigation(const QPoint &point);//导航器点击
    void _flip(const Processing::FlipWay &way);//翻转处理
    void _deleteImage();//删除图片

public:
    _Interaction();

protected:
    virtual void creatCore(const QStringList &list);//初始化核心模块
    virtual void initUiFinish();//UI初始化完成
    virtual QVariant openImage(const QString &path);//打开图片  QVariant -> QHash<int,QList<int>>
    virtual void changeImage(const int &type);//切换图片
    virtual void nextImage();//切换图片
    virtual void backImage();//切换图片
    virtual void changeWidgetSize(const QSize &size);//切换窗口大小
    virtual void watchBigImage();//图片放大
    virtual void watchSmallImage();//图片缩小
    virtual void watchOriginalImage();//查看原图
    virtual void watchAutoImage();//自适应窗口大小
    virtual void clickNavigation(const QPoint &point);//导航器点击
    virtual void rotate(const bool &direction);//旋转
    virtual void flipH();//水平翻转
    virtual void flipV();//垂直翻转
    virtual void deleteImage();//删除图片

private:
    void _initConnect(Core *core);//初始化绑定
    bool _operateTooOften();//操作过于频繁
    bool _isCoreInit = false;//防止多次初始化核心
    QString _needStartWithOpenImagePath = "";//UI未初始化时设置，使其初始化完成立即加载图片
    QTimer *_canResize = nullptr;//限制修改大小频率，降低cpu占用

};

#endif // INTERACTION_H
