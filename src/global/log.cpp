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
        thisLevel = Variable::DEBUG;
        break;
    case QtInfoMsg:
        outMsg = QString("[%1 I]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::INFO;
        break;
    case QtWarningMsg:
        outMsg = QString("[%1 W]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::WARNING;
        break;
    case QtCriticalMsg:
        outMsg = QString("[%1 C]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::CRITICAL;
        break;
    case QtFatalMsg:
        outMsg = QString("[%1 F]: %2").arg(timeStr).arg(msg);
        thisLevel = Variable::FATAL;
    }

    //写入日志文件
    if (fp != NULL && Variable::LOGTOFILE) {
        fprintf(fp, "%s\n", outMsg.toUtf8().data());
        fflush(fp);
    }

    if (ctrlEnv == NULL || ctrlEnv == "")
        env = "debug";
    else
        env = QString(ctrlEnv).toLower();

    // 根据环境变量级别，将信息输出到命令行
    Variable::LOGLEVEL envLogLevel = Variable::NOLOG;
    if ((env == "fatal") || (env == "1"))
        envLogLevel = Variable::FATAL;
    else if((env == "critical") || (env == "2"))
        envLogLevel = Variable::CRITICAL;
    else if((env == "info") || (env == "3"))
        envLogLevel = Variable::INFO;
    else if((env == "debug") || (env == "4"))
        envLogLevel = Variable::DEBUG;
    else if((env == "warning") || (env == "5"))
        envLogLevel = Variable::WARNING;
    if(thisLevel<=envLogLevel){
        //打印到控制台
        fprintf(stdout, "%s\n", msg.toStdString().c_str());
        fflush(stdout);
    }

    // 遇到致命错误，需要终止程序
    if (type == QtFatalMsg)
        abort();
}
