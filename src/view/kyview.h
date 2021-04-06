#ifndef KYVIEW_H
#define KYVIEW_H
#define FITTHEMEWINDOW "org.ukui.style"
#define FITCONTROLTRANS "org.ukui.control-center.personalise"
//#include <QKyView>
#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>
#include <QGSettings>
#include <QStyleOption>
#include <QMimeData>
#include <QUrl>
#include <QMessageBox>

#include "toolbar.h"
#include "titlebar.h"
#include "openimage.h"
#include "showimagewidget.h"
#include "navigator.h"
#include "information.h"
#include "daemondbus.h"
#include "sidebar.h"

#include "global/variable.h"
#include "controller/interaction.h"
#include "global/variable.h"

class KyView : public QWidget
{
    Q_OBJECT
public:
    explicit KyView(const QStringList &args);
    ~KyView();
    static KyView *mutual;//指针类型静态成员变量


private:
    Interaction *m_interaction = nullptr;
    ToolBar *m_toolbar = nullptr;//工具栏
    TitleBar *m_titlebar = nullptr;//顶栏
    Navigator *m_navigator = nullptr;//导航器
    OpenImage *m_openImage = nullptr;//打开图片
    ShowImageWidget *m_showImageWidget = nullptr;//展示图片
    Information *m_information = nullptr;//信息窗口

    QGSettings *m_pGsettingThemeData = nullptr;//主题
    QGSettings *m_pGsettingControlTrans = nullptr;//控制面板透明度

    QTimer *m_timer;
    QTimer *m_timerinfor;
    QTimer *m_timernavi;

    // 用户手册功能
    DaemonDbus *m_DaemonIpcDbus;

    bool m_inforState = true;//信息栏状态

    void delayHide();//顶栏工具栏的延时隐藏
    void delayHide_infor();//顶栏工具栏的延时隐藏
    void delayHide_navi();//导航栏在鼠标离开界面时隐藏
//    void delayHide_leave();//顶栏和底栏在鼠标离开界面时隐藏



    void initconnect();//初始化连接
    void openState();//  判断打开应用应该是什么状态
    void aboutShow();//关于界面打开，两栏隐藏

    void titlebarChange();//改变顶栏位置和大小
    void openImageChange();//改变中间打开图片位置和大小
    void showImageChange();//改变图片展示的位置和大小
    void toolbarChange();//改变工具栏位置和大小
    void naviChange();//改变导航栏位置
    void inforChange();//改变信息栏位置

    void showInforWid();//展示或隐藏图片信息窗口

    void clearImage();//无图片，需要返回默认界面
    void hoverChange(int y);//hover态，顶栏和工具栏的状态

    void initGsetting();//监听主题变化
//    void initGSetting_tran();//监听控制面板变化
    void themeChange();//主题变化
    void transChange();//控制面板变化


    void mouseMoveEvent(QMouseEvent *event);//hover态时两栏和按钮状态--检测鼠标所在区域
    void resizeEvent(QResizeEvent *event);//改变窗口尺寸，各控件大小位置变化
    void leaveEvent(QEvent *event);//鼠标离开，按钮、两栏隐藏
    void enterEvent(QEvent *event);//鼠标进入
    void paintEvent(QPaintEvent *event);// 解决毛玻璃特效的问题
    void keyPressEvent(QKeyEvent *event);// 键盘响应事件--f1
    void mouseDoubleClickEvent(QMouseEvent *event);//双击响应--全屏和还原
    bool eventFilter(QObject *obj, QEvent *event);//事件过滤器--信息栏响应两栏show
    void dragEnterEvent(QDragEnterEvent *event);//文件拖拽显示事件--判断是否响应dropevent
    void dropEvent(QDropEvent *event);//文件拖拽响应

private slots:

    void changOrigSize();//主界面最大化和还原
    void Toshowimage();//显示图片


signals:



};

#endif // KYVIEW_H
