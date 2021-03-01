#ifndef DBUS_H
#define DBUS_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>

#include "../global/variable.h"

class Dbus : public QObject
{
public:
    Dbus();
    bool argumentsCommand(QStringList arguments);

private:
    bool connectSeccess = false;
};

#endif // DBUS_H
