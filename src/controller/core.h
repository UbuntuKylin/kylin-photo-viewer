#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QString>
#include "src/model/dbus.h"
#include "src/model/file.h"

class Core : public QObject
{
    Q_OBJECT
public:
    Core();
    void initCoreModel(QStringList arguments);//初始化核心功能

private:
    void initCore();//初始化核心
    void initDbus(QStringList arguments);//初始化Dbus模块
    Dbus *dbus = nullptr;//DBus模块对象
    void initFile();//初始化写模块
    File *file = nullptr;//读写模块对象

    void processingCommand(QStringList cmd);//处理终端命令
};

#endif // CORE_H
