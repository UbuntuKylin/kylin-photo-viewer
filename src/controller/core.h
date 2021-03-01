#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "src/model/dbus.h"

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
};

#endif // CORE_H
