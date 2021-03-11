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

    QWidget *titleWid; //布局
    menuModule *m_menu = nullptr;
    QLabel *imageName;

    void showImageName(QString name);
private:



    QHBoxLayout *titleLayout;
    QPushButton *logoBtn;//logo
    QLabel *logolb;//name
    QPushButton *minibtn;//最小化按钮
    QPushButton *fullscreen;//全屏
    QPushButton *closebtn;//关闭

    void initControlQss();//初始化顶栏布局
    void initConnect();//建立信号与槽的连接
    void setStyle();//
    void fullRecovery();
//    void leaveEvent(QEvent *event);
//    void enterEvent(QEvent *event);


signals:
    void changeSize();
    void recovery();

};

#endif // TITLEBAR_H
