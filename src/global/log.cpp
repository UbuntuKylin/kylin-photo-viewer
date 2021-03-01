#include "log.h"

#include <QDateTime>
#include <unistd.h>

void Log::msgHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    static FILE *fp = NULL;
    static char logPath[255] = {0};
    static int uid = -1;

    Q_UNUSED(context);

    // 初始执行时，设置log文件路径
    if (uid == -1) {
        uid = getuid();
        sprintf(logPath, "/run/user/%d/%s.log", uid, Variable::PROGRAM_NAME);
    }
    if (access(logPath, F_OK|W_OK) == 0) {
        // log文件存在且可写
        if (fp == NULL) {
            fp = fopen(logPath, "a+");
        }
    } else {
        if (fp != NULL)
            fclose(fp);
        fp = NULL;
    }

    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeStr = currentTime.toString("yy.MM.dd hh:mm:ss +zzz");

    // 获取用于控制命令行输出的环境变量
    char *ctrlEnv = getenv(Variable::ENV_LOGLEVEL.toLocal8Bit().data());
    QString env;

    Variable::LOGLEVEL thisLevel;

    QString outMsg;
    switch (type) {
    case QtDebugMsg:
        outMsg = QString("[%1 D]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::NORMAL;
        break;
    case QtInfoMsg:
        outMsg = QString("[%1 I]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::ALL;
        break;
    case QtWarningMsg:
        outMsg = QString("[%1 W]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::DEBUG;
        break;
    case QtCriticalMsg:
        outMsg = QString("[%1 C]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::RELEASE;
        break;
    case QtFatalMsg:
        outMsg = QString("[%1 F]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::RELEASE;
    }

    if (fp != NULL) {
        fprintf(fp, "%s\n", outMsg.toUtf8().data());
        fflush(fp);
    }

    if (ctrlEnv == NULL || ctrlEnv == "")
        env = "normal";
    else
        env = QString(ctrlEnv).toLower();

    // 根据环境变量级别，将信息输出到命令行
    Variable::LOGLEVEL envLogLevel = Variable::NOLOG;
    if ((env == "release") || (env == "1"))
        envLogLevel = Variable::RELEASE;
    else if((env == "normal") || (env == "2"))
        envLogLevel = Variable::NORMAL;
    else if((env == "debug") || (env == "3"))
        envLogLevel = Variable::DEBUG;
    else if((env == "all") || (env == "4"))
        envLogLevel = Variable::ALL;
    if(thisLevel<=envLogLevel){
        fprintf(stdout, "%s\n", outMsg.toStdString().c_str());
        fflush(stdout);
    }

    // 遇到致命错误，需要终止程序
    if (type == QtFatalMsg)
        abort();
}
