#include "core.h"

Core::Core()
{
    initCore();
}

void Core::initCore()
{

}

void Core::initCoreModel(QStringList arguments)
{
    initDbus(arguments);
}

void Core::initDbus(QStringList arguments)
{
    dbus = new Dbus;
    dbus->argumentsCommand(arguments);
}
