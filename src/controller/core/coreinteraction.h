#ifndef CoreINTERACTIONCoreH
#define CoreINTERACTIONCoreH

#include <QThread>
#include <QTimer>
#include "controller/core/core.h"
#include "controller/interaction.h"

//实际实现功能
class CoreInteraction :  public Interaction
{
    Q_OBJECT

signals:
    QVariant coreOpenImage(const QString &path);//打开图片  QVariant -> QHash<int,QList<int>>
    QStandardItemModel * coreGetAlbumModel();//设置为背景图
    void coreChangeImage(const int &type); //切换图片
    void coreChangeWidgetSize(const QSize &size);//切换窗口大小
    void coreChangeImageShowSize(ImageShowStatus::ChangeShowSizeType);//图片显示状态（放大缩小）
    void coreClickNavigation(const QPoint &point);//导航器点击
    void coreFlip(const Processing::FlipWay &way);//翻转处理
    void coreDeleteImage();//删除图片
    void coreSetAsBackground();//设置为背景图
    void coreOpenInfile();//文件夹中打开
    void coreClose();//结束进程

public:
    CoreInteraction();

protected:
    virtual void creatCore(const QStringList &list);//初始化核心模块
    virtual void initUiFinish();//UI初始化完成
    virtual void openImage(const QString &path);//打开图片  QVariant -> QHash<int,QList<int>>
    virtual void changeImage(const int &type);//切换图片
    virtual void nextImage();//切换图片
    virtual void backImage();//切换图片
    virtual void changeWidgetSize(const QSize &size);//切换窗口大小
    virtual void watchBigImage();//图片放大
    virtual void watchSmallImage();//图片缩小
    virtual void watchOriginalImage();//查看原图
    virtual void watchAutoImage();//自适应窗口大小
    virtual void clickNavigation(const QPoint &point);//导航器点击
    virtual void rotate(const bool &clockwise);//旋转
    virtual void flipH();//水平翻转
    virtual void flipV();//垂直翻转
    virtual void deleteImage();//删除图片
    virtual void setAsBackground();//设置为背景图
    virtual void openImageInfile();//在文件夹中打开
    virtual void close();//关闭进程
    virtual QStandardItemModel * getAlbumModel();//获取相册model指针

private:
    void initConnect();//初始化绑定
    bool coreOperateTooOften();//操作过于频繁
    bool m_isCoreInit = false;//防止多次初始化核心
    QString m_needStartWithOpenImagePath = "";//UI未初始化时设置，使其初始化完成立即加载图片
    QTimer *m_canResize = nullptr;//限制修改大小频率，降低cpu占用
    Core *m_core = nullptr;//核心对象

};

#endif // CoreINTERACTIONCoreH
