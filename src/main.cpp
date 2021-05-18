#include "view/kyview.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include "global/log.h"
#include "controller/interaction.h"
#include "view/xatom-helper.h"

int main(int argc, char *argv[])
{ 
    //添加4K 屏幕支持。
#if(QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#else
    // do not support 4K.
#endif
    QApplication a(argc, argv);

    //注册MessageHandler
    qInstallMessageHandler(Log::msgHandler);
    qApp->setProperty("noChangeSystemFontSize",true);
    qApp->setWindowIcon(QIcon::fromTheme("kylin-photo-viewer", QIcon(":/res/res/kyview_logo.png")));

    //翻译
    QTranslator app_trans;
    QTranslator qt_trans;
    QString locale = QLocale::system().name();
    QString trans_path;
    if (QDir("/usr/share/kylin-photo-viewer/translations").exists()) {
        trans_path = "/usr/share/kylin-photo-viewer/translations";
    }
    else {
        trans_path = qApp->applicationDirPath() + "/translations";
    }
    QString qt_trans_path;
    qt_trans_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);// /usr/share/qt5/translations

    if (locale == "zh_CN") {
        if(!app_trans.load("kylin-photo-viewer_" + locale + ".qm", trans_path)){
            qDebug() << "Load translation file："<< "kylin-photo-viewer_" + locale + ".qm from" << trans_path << "failed!";
        } else {
            a.installTranslator(&app_trans);
        }

        if (!qt_trans.load("qt_" + locale + ".qm", qt_trans_path)){
            qDebug() << "Load translation file："<< "qt_" + locale + ".qm from" << qt_trans_path << "failed!";
        } else {
            a.installTranslator(&qt_trans);
        }
    }

    //主题框架
    KyView w(a.arguments());
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(w.winId(), hints);

    Interaction::getInstance()->initUiFinish();
    return a.exec();
}
