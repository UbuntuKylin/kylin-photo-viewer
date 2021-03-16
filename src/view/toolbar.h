#ifndef TOOLBAR_H
#define TOOLBAR_H
#define FITTHEMEWINDOW "org.ukui.style"

#include <QWidget>
#include <QFrame>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>
#include <QPainter>
#include <QGSettings>
#include <QGraphicsDropShadowEffect>
#include "src/controller/interaction.h"

class ToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    void changePerRate(QString num);
    void reduceImage();
    void enlargeImage();
    QFrame *tooleWid;//布局
private:

    QHBoxLayout *toolLayout;
    QGSettings *m_pGsettingThemeData = nullptr;
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

    QColor color = QColor(190 ,190, 190);
    QBrush brush = QBrush(Qt::white);

    Interaction *interaction = nullptr;

    void _initControlQss();//初始化布局
    void _setstyle();
//    bool event(QEvent *event);


    void _originalSize();
    void _adaptiveWidget();
    void _rotate();
    void _flipH();
    void _flipV();
    void _cutImage();
    void _filter();
    void _labelbar();
    void _sidebar();
    void _information();
    void _delImage();

    void _changeStyle();
    void _initGsetting();
    void _dealSystemGsettingChange(const QString);
    void _initConnect();
//    void paintEvent(QPaintEvent *event);


private slots:


signals:
    void _changeNum(QString num);
    void showInfor();

};

#endif // TOOLBAR_H
