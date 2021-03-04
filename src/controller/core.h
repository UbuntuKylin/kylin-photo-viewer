#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QImage>
#include "src/model/dbus.h"
#include "src/model/file.h"

class ImageShowStatus //记录显示图片状态
{
public:
    QMap<int,QString> _imageUrlMap;//图片队列
    int _maxType;//记录动态分配标签的最大值
    int _nowType;//记录当前打开
    int _backType;//记录上次打开
    Mat _nowMat;//记录当前打开
    Mat _backMat;//记录上次打开
    QImage *_nowImage = nullptr;
    Mat _changeImage(Mat mat);
    void _changeImageType(int num = 0);
};

struct ImageAndInfo
{
    QFileInfo info;
    QImage * image;
    int type;
};
Q_DECLARE_METATYPE(ImageAndInfo)//QVarant 注册自定义类型

class Core : public QObject , public ImageShowStatus
{
    Q_OBJECT

signals:
    void needOpenImage(QString path);
    void openFinish(QVariant var);

public:
    Core();
    void loadCoreModel(QStringList arguments);//初始化核心功能
    QVariant findAllImageFromeDir(QString fullPath);//寻找目录下所有支持的图片
    QVariant openImage(QString fullPath);//打开图片
    void changeImage(const int &type); //切换图片

private:
    void _initCore();//初始化核心
    void _initDbus(QStringList arguments);//初始化Dbus模块
    Dbus *_dbus = nullptr;//DBus模块对象
    void _initFile();//初始化写模块
    File *_file = nullptr;//读写模块对象

    void _processingCommand(QStringList cmd);//处理终端命令

};

#endif // CORE_H
