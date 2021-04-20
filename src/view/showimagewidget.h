#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H
#define FITTHEMEWINDOW "org.ukui.style"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QClipboard>
#include <QProcess>
#include <QMovie>
#include <QGSettings>
#include "controller/interaction.h"

class ShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    ShowImageWidget(QWidget *parent,int w, int h);

    QPushButton *g_next; //下一张
    QPushButton *g_back; //上一张
    bool g_buttonState = true; //用来判断只有一张图片时隐藏左右滑动按钮
    bool m_canSet = true;//针对于动图，只响应第一次的结果显示。
    void reMove(int w, int h);
    void openImage(QString path);//打开图片
    void nextImage();//下一张
    void backImage();//上一张
    void delayShow(bool isLoading);//图片加载过慢时，进行延时展示
public slots:
    void albumSlot(bool isOpen);//判断是否发送相册展示的事件
    void isDelete(bool isDel);//判断是删除
private:
    QGSettings *m_pGsettingThemeData = nullptr;
//    QWidget *imageWid;
//    QHBoxLayout *imageLayout;
    QLabel *m_showImage;//用来展示图片的区域
    QAction *m_copy;//复制
    QAction *m_setDeskPaper;//设置为桌面壁纸
    QAction *m_setLockPaper;//设置为锁屏壁纸
    QAction *m_print;//打印
    QAction *m_deleteImage;//删除
    QAction *m_showInFile;//在文件夹中显示
    QMenu * m_imageMenu;//图片右键菜单

    QString m_path = "";//打开文件夹的路径
    QPixmap m_copyImage;//留着复制可能用
    QString m_paperFormat;//用来判断可设置为壁纸的样式

    QMovie *m_loadingMovie = nullptr;

    int m_typeNum;
    bool m_isOpen = true;//是否自动打开相册
    bool m_isDelete = false;//判断是删除
    void initInteraction();
    void openFinish(QVariant var);//打开结束
    void initConnect();
    void sideState(int num);//需要根据条件判断侧栏的显示和大小相关信息



    //右键菜单的各功能
    void copy();//复制
    void setDeskPaper();//设置为桌面壁纸
    void setLockPaper();//设置为锁屏壁纸
    void print();//打印
    void deleteImage();//删除图片
    void showInFile();//在文件夹中显示

    void setMenuAction();//检查当前图片是否可设置为壁纸
    void startWithOpenImage(QString path);//由图片打开

    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void perRate(QString num);//改变工具栏的缩小的百分比
    void toShowImage();//告诉主界面需要show和hide的控件
    void showNavi(QPixmap img);//展示导航栏
    void changeInfor(QFileInfo info, QString imageSize,QString colorSpace);//获得信息栏的信息
    void titleName(QString imageName);//需要将图片名字发送给标题栏显示
    void clearImage();//删除完列表里的图片时需要清空界面，恢复默认状态
    void reduceChange();//滚轮对图片缩小，需要将此动作信号发送出去进行处理
    void enlargeChange();//滚轮对图片放大，需要将此动作信号发送出去进行处理
    void changeSideSize(int type);//确定相册的尺寸
    void toShowSide();//当图片大于2张及以上，默认打开侧栏
//    void toSelectHigh(bool isHigh);//判断删除时高亮的选择
};

#endif // SHOWIMAGEWIDGET_H
