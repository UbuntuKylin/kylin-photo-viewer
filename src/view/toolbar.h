#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>
#include <QPainter>

class ToolBar : public QDialog
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
private:
    QWidget *tooleWid;//布局
    QHBoxLayout *toolLayout;

    QPushButton *reduce;//缩小
    QLabel *percentage;//百分比
    QPushButton *enlarge;//放大
    QPushButton *originalSize;//原始尺寸
    QPushButton *adaptiveWidget;//适应图片
    QPushButton *rotate;//旋转
    QPushButton *flipH;//水平翻转
    QPushButton *flipV;//垂直翻转
    QPushButton *cutImage;//裁剪
    QLabel *line1;
    QPushButton *filter;//滤镜
    QPushButton *labelbar;//标注栏---
    QLabel *line2;
    QPushButton *sidebar;//侧边栏---
    QPushButton *information;//详细信息---
    QPushButton *delImage;//删除图片

    void initControlQss();//初始化布局
    void setstyle();
    bool event(QEvent *event);
//    void paintEvent(QPaintEvent *event);

protected:
//    virtual void mousePressEvent(QMouseEvent *e);
//    virtual void mouseMoveEvent(QMouseEvent *e);
//    virtual void mouseReleaseEvent(QMouseEvent *e);



signals:


};

#endif // TOOLBAR_H
