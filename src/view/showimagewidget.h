#ifndef SHOWIMAGEWIDGET_H
#define SHOWIMAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QClipboard>
#include <QProcess>
#include "controller/interaction.h"

class ShowImageWidget : public QWidget
{
    Q_OBJECT
public:
    ShowImageWidget(QWidget *parent,int w, int h);

    QPushButton *next; //下一张
    QPushButton *back; //上一张
    bool buttonState = true; //用来判断只有一张图片时隐藏左右滑动按钮

    void re_move(int w, int h);
    void _openImage(QString path);//打开图片
    void _startWithOpenImage(QString path);//由图片打开
private:
    Interaction *interaction = nullptr;
//    QWidget *imageWid;
//    QHBoxLayout *imageLayout;
    QSize iconSize;//左右按钮图片尺寸
    QLabel *showImage;//用来展示图片的区域
    QAction *copy;//复制
    QAction *setDeskPaper;//设置为桌面壁纸
    QAction *setLockPaper;//设置为锁屏壁纸
    QAction *print;//打印
    QAction *deleteImage;//删除
    QAction *showInFile;//在文件夹中显示
    QMenu * imageMenu;//图片右键菜单

    QString path = "";//打开文件夹的路径
    QPixmap copyImage;//留着复制可能用
    QString paperFormat;//用来判断可设置为壁纸的样式
    bool canSet = true;//针对于动图，只响应第一次的结果显示。

    void _initInteraction();
    void openFinish(QVariant var);//打开结束
    void _initConnect();

    void _nextImage();//下一张
    void _backImage();//上一张

    //右键菜单的各功能
    void _copy();//复制
    void _setDeskPaper();//设置为桌面壁纸
    void _setLockPaper();//设置为锁屏壁纸
    void _print();//打印
    void _deleteImage();//删除图片
    void _showInFile();//在文件夹中显示

    void m_setMenuAction();//检查当前图片是否可设置为壁纸

    void _processFinish(bool isFinish);//判断是否处理图片完成
    void albumFinish(QVariant var);//相册
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void perRate(QString num);//改变工具栏的缩小的百分比
    void ToshowImage();//告诉主界面需要show和hide的控件
    void showNavi(QPixmap img);//展示导航栏
    void changeInfor(QFileInfo info, QString imageSize,QString colorSpace);//获得信息栏的信息
    void titleName(QString imageName);//需要将图片名字发送给标题栏显示
    void clearImage();//删除完列表里的图片时需要清空界面，恢复默认状态
    void reduceChange();//滚轮对图片缩小，需要将此动作信号发送出去进行处理
    void enlargeChange();//滚轮对图片放大，需要将此动作信号发送出去进行处理
};

#endif // SHOWIMAGEWIDGET_H
