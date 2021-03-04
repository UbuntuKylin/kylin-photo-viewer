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
#include "toolbar.h"

//#include "menumodule.h"
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
    TitleBar *titlebar = nullptr;
    ToolBar *toolbar = nullptr;
    OpenImage *openImage = nullptr;

    QVBoxLayout *mainLayout;
    QWidget *mainWidget;

    QWidget *titleWidTest;


    void setStyle();
    void titlebarQSS();
    void initconnect();


private slots:
    void changSize();
    void showTwobar();
    void hideTwobar();
    void removeForfull();
//    void showmini();
//    void showFull();
//    void closewindow();

signals:

};

#endif // KYVIEW_H
