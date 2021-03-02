#include "dbus.h"
#include <QDebug>
Dbus::Dbus()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService(Variable::PHOTO_VIEW_DBUS_SERVICENAME))
        connectSeccess=sessionBus.registerObject(Variable::PHOTO_VIEW_DBUS_PARH,"dbus.me",this,QDBusConnection::ExportAllContents);
        //connectSeccess=sessionBus.registerObject(Variable::PHOTO_VIEW_DBUS_PARH,this,QDBusConnection::ExportAllContents);ExportAllSlots
}

void Dbus::argumentsCommand(QString cmd)
{
    if(!connectSeccess)//如果为首个实例不响应
        return;
    else
    {
        //QDBusInterface(Variable::PHOTO_VIEW_DBUS_SERVICENAME,Variable::PHOTO_VIEW_DBUS_PARH,"",QDBusConnection::sessionBus()).call();
        exit(0);
    }

}

void Dbus::transmissionCMD(QStringList arguments)
{
    qDebug()<<arguments;
}

