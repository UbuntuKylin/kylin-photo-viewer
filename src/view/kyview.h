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

#include <QPinchGesture>
#include <QTouchEvent>
#include <QGestureEvent>
#include <QPanGesture>
#include <QSwipeGesture>
#include <QTapGesture>
#include <QImageReader>
#include <QLocale>

#include <QX11Info>

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

#include <QThread>

class KyView : public QWidget
{
    Q_OBJECT
public:
    explicit KyView(const QStringList &args);
    ~KyView();
    static KyView *mutual;//指针类型静态成员变量


private:
    ToolBar *m_toolbar = nullptr;//工具栏
    TitleBar *m_titlebar = nullptr;//顶栏
    Navigator *m_navigator = nullptr;//导航器
    OpenImage *m_openImage = nullptr;//打开图片
    ShowImageWidget *m_showImageWidget = nullptr;//展示图片
    Information *m_information = nullptr;//信息窗口
    SideBar *m_sideBar = nullptr;//相册
    QGSettings *m_pGsettingThemeData = nullptr;//主题
    QGSettings *m_pGsettingControlTrans = nullptr;//控制面板透明度

    QTimer *m_timer;
    QTimer *m_timernavi;
    QTimer *m_timeNomove;//鼠标两秒不动

    // 用户手册功能
    DaemonDbus *m_DaemonIpcDbus;

    QLocale m_local;

    bool m_mousePress; //按下鼠标左键
    bool m_inforState = true;//信息栏状态
    bool m_albumState = true;//相册状态
    bool m_albumShow = true;//切换图片时，相册状态
    bool m_titleState = true; //鼠标离开进入时，有关菜单下拉列表show时的问题，先默认没有离开界面
    double m_tran = 0.6;
    QString m_icon;
    void initconnect();//初始化连接


    void titlebarChange();//改变顶栏位置和大小
    void openImageChange();//改变中间打开图片位置和大小
    void showImageChange();//改变图片展示的位置和大小
    void toolbarChange();//改变工具栏位置和大小
    void naviChange();//改变导航栏位置
    void inforChange();//改变信息栏位置
    void albumChange();//改变相册位置
    void hoverChange(int y);//hover态，顶栏和工具栏的状态
    void initGsetting();//监听主题变化
//    void initGSetting_tran();//监听控制面板变化
    void themeChange();//主题变化
    void transChange();//控制面板变化
    void avoidChange();//防止点击即切换的情况

    void mouseMoveEvent(QMouseEvent *event);//hover态时两栏和按钮状态--检测鼠标所在区域
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);//改变窗口尺寸，各控件大小位置变化
    void leaveEvent(QEvent *event);//鼠标离开，按钮、两栏隐藏
    void enterEvent(QEvent *event);//鼠标进入
    void paintEvent(QPaintEvent *event);// 解决毛玻璃特效的问题
    void keyPressEvent(QKeyEvent *event);// 键盘响应事件--f1
    void mouseDoubleClickEvent(QMouseEvent *event);//双击响应--全屏和还原
    bool eventFilter(QObject *obj, QEvent *event);//事件过滤器--信息栏响应两栏show
    void dragEnterEvent(QDragEnterEvent *event);//文件拖拽显示事件--判断是否响应dropevent
    void dropEvent(QDropEvent *event);//文件拖拽响应

    void x11EventEnd();
    QPoint m_mousePointFromWindow;//记录拖动时鼠标位置


    //手势相关
    void initGrabGesture(); // 初始化手势
    bool event(QEvent *event);
    bool gestureEvent(QEvent *event); // 手势识别
    void pinchTriggered(QPinchGesture *gesture);  // 捏手势
    void tapAndHoldGesture(QTapAndHoldGesture *gesture);  // 平移手势
    bool touchRotate(const qreal &lastAngle);//触控旋转
    int m_rotateLevel = 0;//手势旋转级别
    qreal m_lastDistance = 1;//上次手势距离
    qreal m_minDistance = 0.5;//最小生效距离
    //放大缩小和旋转同时只能进行一项
    bool m_isRotate = false;
    bool m_isResize = false;
    bool m_panTriggered = false;//长按事件触发中
    QPointF m_touchPoint;//滑动切换图片事件记录距离

private slots:

    void changOrigSize();//主界面最大化和还原
    void toShowImage();//显示图片
    void showSidebar();//显示相册
    void defaultSidebar();

    void delayHide();//顶栏工具栏的延时隐藏
    void delayHide_navi();//导航栏在鼠标离开界面时隐藏
    void delayHide_move();//鼠标两秒不动，隐藏两栏
    void openState();//  判断打开应用应该是什么状态
    void aboutShow();//关于界面打开，两栏隐藏
    void showInforWid();//展示或隐藏图片信息窗口
    void clearImage();//无图片，需要返回默认界面


signals:
    void albumState(bool isOpen);//相册在主界面隐藏或show的状态需要确定

};

#endif // KYVIEW_H
