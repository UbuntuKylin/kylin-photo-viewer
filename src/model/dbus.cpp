#include "dbus.h"
#include <QDebug>
Dbus::Dbus()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService(Variable::PHOTO_VIEW_DBUS_SERVICENAME))
        _connectSeccess=sessionBus.registerObject(Variable::PHOTO_VIEW_DBUS_PARH,Variable::PHOTO_VIEW_DBUS_INTERFACE,this,QDBusConnection::ExportAllSlots);
        //connectSeccess=sessionBus.registerObject(Variable::PHOTO_VIEW_DBUS_PARH,this,QDBusConnection::ExportAllContents);
}

void Dbus::argumentsCommand(const QStringList &arguments)
{
    QStringList cmd = arguments;
    cmd.removeFirst();
    QDBusInterface *interface = new QDBusInterface(Variable::PHOTO_VIEW_DBUS_SERVICENAME,
                                                   Variable::PHOTO_VIEW_DBUS_PARH,
                                                   Variable::PHOTO_VIEW_DBUS_INTERFACE,
                                                   QDBusConnection::sessionBus());
    interface->call("getCmdFromOtherMe",cmd);
    exit(0);
}

bool Dbus::getConnectSeccess()
{
    return _connectSeccess;
}

void Dbus::getCmdFromOtherMe(const QStringList &cmd)
{
    emit processingCommand(cmd);
    //qDebug()<<"从DBUS接收到命令："<<cmd;
}

