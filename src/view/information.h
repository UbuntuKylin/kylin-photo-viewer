#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QFont>
class Information : public QWidget
{
    Q_OBJECT
public:
    explicit Information(QWidget *parent = nullptr);
    void contentText(QFileInfo info,QString sizeImage,QString spaceColor);
private:
    QLabel *widName;//窗口名--信息
    QLabel *name;//名称
    QLabel *format;//格式
    QLabel *storageSize;//存储大小
    QLabel *pixelSize;//像素尺寸
    QLabel *colorSpace; //颜色空间
    QLabel *creationTime;//创建时间
    QLabel *revisionTime;//修改时间

    QLabel *nameC;//名称内容
    QLabel *formatC;//格式内容
    QLabel *storageSizeC;//存储大小内容
    QLabel *pixelSizeC;//像素尺寸内容
    QLabel *colorSpaceC; //颜色空间内容
    QLabel *creationTimeC;//创建时间内容
    QLabel *revisionTimeC;//修改时间内容

    QWidget *inforWid;
    QGridLayout *gdLayout;

    QFont ft;

    void _setstyle();
    void _layout();
    void _longText(QLabel *nameC, QString text);

signals:

};

#endif // INFORMATION_H
