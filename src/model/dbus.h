#ifndef DBUS_H
#define DBUS_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>

#include "../global/variable.h"

class Dbus : public QObject
{
    Q_OBJECT
public:
    Dbus();
    void argumentsCommand(QString cmd);

public slots:
    void transmissionCMD(QStringList);

private:
    bool connectSeccess = false;//注册DBus成功
};

#endif // DBUS_H
