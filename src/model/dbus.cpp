#include "dbus.h"
#include <QDebug>
Dbus::Dbus()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService(Variable::PHOTO_VIEW_DBUS_SERVICENAME))
        connectSeccess=sessionBus.registerObject(Variable::PHOTO_VIEW_DBUS_PARH,this,QDBusConnection::ExportAllContents);
}

bool Dbus::argumentsCommand(QStringList arguments)
{
    qDebug()<<arguments;
}
