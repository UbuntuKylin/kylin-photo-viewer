#ifndef DBUS_H
#define DBUS_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>

#include "global/variable.h"

class Dbus : public QObject
{
    Q_OBJECT

signals:
    void processingCommand(QStringList);

public:
    Dbus();
    void argumentsCommand(const QStringList &arguments);
    bool getConnectSeccess();

public slots:
    void getCmdFromOtherMe(const QStringList &cmd);

private:
    bool m_connectSeccess = false;//注册DBus成功
};

#endif // DBUS_H
