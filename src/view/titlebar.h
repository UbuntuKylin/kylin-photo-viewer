#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

#include "menumodule.h"
class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);
    menuModule *m_menu = nullptr;

    QWidget *titleWid;
    QHBoxLayout *titleLayout;
    QPushButton *logoBtn;
    QLabel *logolb;
    QPushButton *minibtn;
    QPushButton *fullscreen;
    QPushButton *closebtn;

    void initControlQss();//初始化顶栏布局
    void initConnect();//建立信号与槽的连接
    void setStyle();//
    void fullRecovery();
private:
signals:
    void changeSize();

};

#endif // TITLEBAR_H
