#include "view/kyview.h"

#include <QApplication>
#include "global/log.h"
#include "controller/interaction.h"
#include "view/xatom-helper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(Log::msgHandler);//注册MessageHandler

    //如果初始化异常则退出
    Interaction *interaction =Interaction::getInstance();
    interaction->creatCore(a.arguments());

    MainWindow w;
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(w.winId(), hints);
    w.show();
    return a.exec();
}
