#ifndef TITLEBAR_H
#define TITLEBAR_H
#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>

#include "menumodule.h"
class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

    QWidget *g_titleWid; //布局
    menuModule *g_menu = nullptr;
    QLabel *g_imageName;
    QPushButton *g_fullscreen;//全屏

    void showImageName(QString name);
private:

    QHBoxLayout *m_titleLayout;
    QLabel *m_logoBtn;//logo
    QLabel *m_logolb;//name
    QPushButton *m_minibtn;//最小化按钮

    QPushButton *m_closebtn;//关闭

    void initControlQss();//初始化顶栏布局
    void initConnect();//建立信号与槽的连接
    void setStyle();//

//    void leaveEvent(QEvent *event);
//    void enterEvent(QEvent *event);


signals:

    void recovery();//处理主界面最大化和还原
    void openSignal();//标题栏夏卡菜单打开图片
    void aboutShow();//关于界面打开，两栏的展示情况

};

#endif // TITLEBAR_H
