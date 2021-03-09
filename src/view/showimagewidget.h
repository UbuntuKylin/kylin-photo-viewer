#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include "src/controller/interaction.h"
class ShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    ShowImageWidget(QWidget *parent,int w, int h);
    void _openImage(QString path);
    void _startWithOpenImage(QString path);
    QPushButton *next; //下一张
    QPushButton *back; //上一张
//    void changeImageSize(QImage *image);//适应窗口大小
//    void re_resize();
    void re_move(int w, int h);
private:
    Interaction *interaction = nullptr;

    QSize iconSize;
    QLabel *showImage;//用来展示图片的区域
    void _initInteraction();
    void openFinish(QVariant var);
    void _initConnect();

    void _nextImage();
    void _backImage();
    void _rightmenu();

    void albumFinish(QVariant var);
    void resizeEvent(QResizeEvent *event);

signals:
    void perRate(QString num);
    void ToshowImage();//告诉主界面需要show和hide的控件
};

#endif // SHOWIMAGEWIDGET_H
