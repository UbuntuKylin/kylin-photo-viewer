#ifndef MENUMODULE_H
#define MENUMODULE_H
#define DEBUG_MENUMODULE 0  //DEBUG模式开关，代码上线之前务必关掉
#define FITTHEMEWINDOW "org.ukui.style"
//#define APPDATA "org.kylin-photo-viewer-data.settings"   0301-cancel
#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QUrl>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QRect>
#include <QScreen>
#include <QGSettings>
#include <QMetaEnum>
#include <QApplication>

//#include "titlebar.h"
//#include "kyview.h"
#include "daemondbus.h"

class menuModule : public QWidget
{
    Q_OBJECT
public:
    explicit menuModule(QWidget *);
    void themeUpdate();
//    void changeVolumePos(int posX, int posY, int width, int height);
signals:
    void menuModuleClose();
    void openSignal();
//    void menuModuleSetThemeStyle(QString);
public:
    QToolButton *menuButton = nullptr;
    QMenu *m_menu = nullptr;
public:
    QString appName = "appName字段未填充!"; //格式kylin-usb-creator
    QString appShowingName = "app展示名字段未填充"; //格式kylin usb creator ,用来在前端展示
    QString appVersion = "appVersion字段未填充!";
    QString appDesc = "appDesc字段未填充!";
    QString iconPath = "iconPath字段未填充!";
    QString confPath = "org.china-weather-data.settings";

private:
//    int volunmPosX;
//    int volunmPosY;
//    int volunmPosWidth;
//    int volunmPosHeight;

    QMenu *themeMenu = nullptr;
    QSize iconSize;
//    QString appPath = "tools/kylin-usb-creator"; //拉起帮助菜单时使用appName字段
    QDialog *aboutWindow = nullptr;
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingThemeStatus = nullptr;
    DaemonDbus *ipcDbus = nullptr;
    enum typeThemeStatus {
        themeAuto = 0,
        themeBlackOnly = 1,
        themeLightOnly = 2
    } themeStatus;

protected:
//    bool nativeEvent(const QByteArray &eventType, void *message, long *result)override;
//    bool eventFilter(QObject *obj, QEvent *event)override;

public slots:
    void dealSystemGsettingChange(const QString);
private:
    void init();
    QHBoxLayout* initTitleBar(); //关于窗口标题栏初始化
    QVBoxLayout* initBody();    // 关于窗口body初始化
    void initGsetting();
    void initAction();
    void triggerMenu(QAction* act); //主菜单动作4
    void triggerThemeMenu(QAction* act); //主题动作
    void aboutAction();
    void initAbout(); //关于窗口初始化
    void helpAction();
    void setThemeFromLocalThemeSetting(QList<QAction* >); //获取本地主题配置
    void setStyleByThemeGsetting(); //通过外部主题配置设置主题
    void setThemeStyle();
    void setThemeLight();
    void setThemeDark();

    void refreshThemeBySystemConf();    //通过系统配置更改主题

//    void moveVolSliderWid();


    QLabel* titleText = nullptr;
    QLabel* bodyAppVersion = nullptr;
    QLabel* bodySupport = nullptr;
    QLabel* bodyAppName = nullptr;
};

#endif // MENUMODULE_H
