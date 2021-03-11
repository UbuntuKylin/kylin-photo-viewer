#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QFileInfo>
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

    void re_move(int w, int h);
private:
    Interaction *interaction = nullptr;
    QWidget *imageWid;
    QHBoxLayout *imageLayout;
    QSize iconSize;
    QLabel *showImage;//用来展示图片的区域
    QAction * action_wallpaper;//设置壁纸
    QMenu * imageMenu;//图片右键菜单

    void _initInteraction();
    void openFinish(QVariant var);
    void _initConnect();

    void _nextImage();
    void _backImage();
    void _setWallpaper();

    void albumFinish(QVariant var);
    void resizeEvent(QResizeEvent *event);

signals:
    void perRate(QString num);
    void ToshowImage();//告诉主界面需要show和hide的控件
    void showNavi(QPixmap img);
    void changeInfor(QFileInfo info);
    void titleName(QString imageName);
    void clearImage();
};

#endif // SHOWIMAGEWIDGET_H
