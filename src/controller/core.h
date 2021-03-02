#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QString>
#include "src/model/dbus.h"
#include "src/model/file.h"

class Core : public QObject
{
    Q_OBJECT

signals:
    void needOpenImage(QString path);

public:
    Core();
    void loadCoreModel(QStringList arguments);//初始化核心功能
    QList<int> findAllImageFromeDir(QString fullPath);//寻找目录下所有支持的图片
    QList<int> openImage(QString fullPath);//打开图片

private:
    void _initCore();//初始化核心
    void _initDbus(QStringList arguments);//初始化Dbus模块
    Dbus *_dbus = nullptr;//DBus模块对象
    void _initFile();//初始化写模块
    File *_file = nullptr;//读写模块对象

    void _processingCommand(QStringList cmd);//处理终端命令

    QMap<int,QString> _imageUrlMap;//图片队列
    int _maxType = 0;//记录动态分配标签的最大值
    int _showingNowType = 0;//记录当前打开
    Mat _showingNowMat;
};

#endif // CORE_H
