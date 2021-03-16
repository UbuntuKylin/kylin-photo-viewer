#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QClipboard>
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
    QAction * copy;//复制
    QAction * setDeskPaper;//设置为桌面壁纸
    QAction * setLockPaper;//设置为锁屏壁纸
    QAction * print;//打印
    QAction * deleteImage;//删除
    QAction * showInFile;//在文件夹中显示
    QMenu * imageMenu;//图片右键菜单

    QString path = "";//打开文件夹的路径
    QPixmap copyImage;

    void _initInteraction();
    void openFinish(QVariant var);
    void _initConnect();

    void _nextImage();
    void _backImage();

    //右键菜单的各功能
    void _copy();
    void _setDeskPaper();
    void _setLockPaper();
    void _print();
    void _deleteImage();
    void _showInFile();

    void albumFinish(QVariant var);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void perRate(QString num);
    void ToshowImage();//告诉主界面需要show和hide的控件
    void showNavi(QPixmap img);
    void changeInfor(QFileInfo info, QString imageSize,QString colorSpace);
    void titleName(QString imageName);
    void clearImage();
    void deleteCurrImage();
    void reduceChange();
    void enlargeChange();
};

#endif // SHOWIMAGEWIDGET_H
