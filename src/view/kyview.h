#ifndef KYVIEW_H
#define KYVIEW_H

#include <QMainWindow>
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
#include <QScreen>

#include "toolbar.h"
#include "titlebar.h"
#include "openimage.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow *mutual;//指针类型静态成员变量

private:
    TitleBar *titlebar = nullptr;//顶栏
    ToolBar *toolbar = nullptr;//工具栏
    OpenImage *openImage = nullptr;//打开图片
    QWidget *centerWidget;
    QVBoxLayout *centerLayout;

    QWidget *openImageWidget;
    QHBoxLayout *openImageLayout;

    QWidget *toolbarWidget;
    QHBoxLayout *toolbarLayout;

    QLabel *showImage;//用来展示图片的区域
    QSize widgetSize;
    QSize widgetPosition;
    QPoint p;
    void setstyle();//设置QSS
    void initconnect();//初始化连接
    void mouseMoveEvent(QMouseEvent *event);
    void layout();
    bool event(QEvent *event);

private slots:
    void changFullScreen();
    void changOrigSize();
    void removeForfull();


signals:

};

#endif // KYVIEW_H
