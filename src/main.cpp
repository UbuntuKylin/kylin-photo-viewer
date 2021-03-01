#include "view/tmp.h"

#include <QApplication>
#include "global/log.h"
#include "controller/interaction.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(Log::msgHandler);//注册MessageHandler

    //如果初始化异常则退出
    Interaction *interaction =Interaction::getInstance();
    interaction->creatCore(a.arguments());

    Widget w;
    w.show();
    return a.exec();
}
