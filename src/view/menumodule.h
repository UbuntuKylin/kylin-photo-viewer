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
#include "controller/interaction.h"
class menuModule : public QWidget
{
    Q_OBJECT
public:
    explicit menuModule(QWidget *);
    void setThemeLight();
    void setThemeDark();

signals:
    void menuModuleClose();//从标题栏点击退出的信号
    void openSignal();//从标题栏进行打开图片的信号
    void aboutShow();//打开关于，主界面两栏的显示情况

public:
    QToolButton *menuButton = nullptr;
    QMenu *m_menu = nullptr;
private:

    QString m_appShowingName;
    QString m_appVersion;
    QString m_appName;
    QString m_appDesc = "appDesc字段未填充!";
    QString m_iconPath;
    QString m_confPath = "org.china-weather-data.settings";

    Interaction *m_interaction = nullptr;
    QMenu *m_themeMenu = nullptr;
    QDialog *m_aboutWindow = nullptr;
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingThemeStatus = nullptr;
    DaemonDbus *m_ipcDbus = nullptr;
    enum typeThemeStatus {
        themeAuto = 0,
        themeBlackOnly = 1,
        themeLightOnly = 2
    } themeStatus;

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


    void refreshThemeBySystemConf();    //通过系统配置更改主题

    QLabel* m_titleText = nullptr;
    QLabel* m_bodyAppVersion = nullptr;
    QLabel* m_bodySupport = nullptr;
    QLabel* m_bodyAppName = nullptr;
};

#endif // MENUMODULE_H
