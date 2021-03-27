#ifndef _INTERACTION_H
#define _INTERACTION_H

#include <QThread>
#include <QTimer>
#include "controller/core/core.h"
#include "controller/interaction.h"

//实际实现功能
class _Interaction :  public Interaction
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
    void _setAsBackground();//设置为背景图

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
    virtual void setAsBackground();//设置为背景图

private:
    void _initConnect(Core *core);//初始化绑定
    bool _operateTooOften();//操作过于频繁
    bool _isCoreInit = false;//防止多次初始化核心
    QString _needStartWithOpenImagePath = "";//UI未初始化时设置，使其初始化完成立即加载图片
    QTimer *_canResize = nullptr;//限制修改大小频率，降低cpu占用

};

#endif // _INTERACTION_H
