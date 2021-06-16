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
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include "controller/interaction.h"

class ToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    void changePerRate(QString num);
    void reduceImage();//缩小图片
    void enlargeImage();//放大图片
    void delImage();//删除
    void changeStyle();
    QFrame *g_tooleWid;//布局

private:

    QHBoxLayout *m_toolLayout;
    QGSettings *m_pGsettingThemeData = nullptr;
    QPushButton *m_reduce;//缩小
    QLabel *m_percentage;//百分比
    QPushButton *m_enlarge;//放大
    QPushButton *m_originalSize;//原始尺寸
    QPushButton *m_adaptiveWidget;//适应图片
    QPushButton *m_rotate;//旋转
    QPushButton *m_flipH;//水平翻转
    QPushButton *m_flipV;//垂直翻转
    QPushButton *m_cutImage;//裁剪
    QLabel *m_line1;
    QPushButton *m_filter;//滤镜
    QPushButton *m_labelbar;//标注栏
    QLabel *m_line2;
    QPushButton *m_sidebar;//侧边栏
    QPushButton *m_information;//详细信息
    QPushButton *m_delImage;//删除图片

    QColor m_color = QColor(190 ,190, 190);
    QBrush m_brush = QBrush(Qt::white);
    QWidget *m_zoomWid;//缩放按钮和比例窗口
    QFont m_ft;//

    void originalSize();//原始尺寸
    void adaptiveWidget();//适应窗口
    void rotate();//旋转
    void flipH();//水平翻转
    void flipV();//垂直翻转
    void cutImage();//裁剪
    void filter();//滤镜
    void labelbar();//标注
    void sidebar();//侧边栏--相册
    void information();//信息栏

    void initControlQss();//初始化布局
    void dealSystemGsettingChange(const QString);
    void initGsetting();
    void initConnect();

private slots:


signals:
    void showInfor();//主界面显示信息栏
    void showSidebar();//主界面显示相册
    void isDelete(bool isDel);//判断删除

};

#endif // TOOLBAR_H
