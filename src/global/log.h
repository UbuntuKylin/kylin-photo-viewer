#ifndef LOG_H
#define LOG_H

#include "variable.h"
#include <QDebug>

class Log
{
public:
    static void msgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOG_H
