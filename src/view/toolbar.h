#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

class ToolBar : public QDialog
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
private:
    QWidget *tooleWid;
    QHBoxLayout *toolLayout;
    QPushButton *reduce;
    QLabel *percentage;
    QPushButton *enlarge;
    QPushButton *originalSize;
    QPushButton *adaptiveWidget;
    QPushButton *rotate;
    QPushButton *flipH;
    QPushButton *flipV;
    QPushButton *cutImage;
    QLabel *line1;
    QPushButton *filter;
    QPushButton *labelbar;//
    QLabel *line2;
    QPushButton *sidebar;//
    QPushButton *information;//
    QPushButton *delImage;

    void initControlQss();//初始化布局
    void setstyle();

//    void enterEvent(QEvent *event);
//    void leaveEvent(QEvent *event);



signals:
    void showTwobar();
    void hideTwobar();

};

#endif // TOOLBAR_H
