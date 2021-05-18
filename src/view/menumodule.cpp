#include "menumodule.h"
#include "kyview.h"
#include "xatom-helper.h"
#include "sizedate.h"
menuModule::menuModule(QWidget *parent = nullptr) : QWidget(parent)
{
    init();
}


void menuModule::init(){
    initAction();
}

void menuModule::initAction(){

    m_bodyAppName = new QLabel();

    m_bodyAppVersion = new QLabel();
    m_bodySupport = new QLabel();
    m_titleText = new QLabel();

    m_bodySupport->setText(tr("Service & Support Team: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(0,0,0,1)\">"
                         "support@kylinos.cn</a>");
    m_bodyAppVersion->setText(tr("Version: ") + m_appVersion);

    menuButton = new QToolButton;
    menuButton->setToolTip(tr("menu"));
    menuButton->setIcon(QIcon::fromTheme("open-menu-symbolic"));
    menuButton->setProperty("isWindowButton", 0x1);
    menuButton->setProperty("useIconHighlightEffect",0x2);
    menuButton->setIconSize(MICON_SIZES);
    menuButton->setPopupMode(QToolButton::InstantPopup);
    menuButton->setFixedSize(MBTN_SIZE);
    menuButton->setAutoRaise(true);
    menuButton->setFocusPolicy(Qt::NoFocus);
    m_menu = new QMenu();

    QList<QAction *> actions ;
    QAction *actionOpen = new QAction(m_menu);
    actionOpen->setText(tr("Open"));
    QAction *actionTheme = new QAction(m_menu);
    actionTheme->setText(tr("Theme"));
    QAction *actionHelp = new QAction(m_menu);
    actionHelp->setText(tr("Help"));
    QAction *actionAbout = new QAction(m_menu);
    actionAbout->setText(tr("About"));
    QAction *actionQuit = new QAction(m_menu);
    actionQuit->setText(tr("Quit"));
    actions<<actionOpen/*<<actionTheme*/<<actionHelp<<actionAbout<<actionQuit;//暂时禁掉主题切换按钮
    m_menu->addActions(actions);
//    互斥按钮组
    QMenu *m_themeMenu = new QMenu;
    QActionGroup *m_themeMenuGroup = new QActionGroup(this);
    QAction *autoTheme = new QAction("Auto",this);
    m_themeMenuGroup->addAction(autoTheme);
    m_themeMenu->addAction(autoTheme);
    autoTheme->setCheckable(true);
    QAction *lightTheme = new QAction("Light",this);
    m_themeMenuGroup->addAction(lightTheme);
    m_themeMenu->addAction(lightTheme);
    lightTheme->setCheckable(true);
    QAction *darkTheme = new QAction("Dark",this);
    m_themeMenuGroup->addAction(darkTheme);
    m_themeMenu->addAction(darkTheme);
    darkTheme->setCheckable(true);
    QList<QAction* > themeActions;
    themeActions<<autoTheme<<lightTheme<<darkTheme;
    actionTheme->setMenu(m_themeMenu);
    menuButton->setMenu(m_menu);
    connect(m_menu,&QMenu::triggered,this,&menuModule::triggerMenu);
    connect(this,&menuModule::menuModuleClose,Interaction::getInstance(),&Interaction::close);
//    connect(Interaction::getInstance(),&Interaction::progremExit, KyView::mutual, &KyView::close);

    initGsetting();
}



void menuModule::setStyleByThemeGsetting(){
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {
        setThemeDark();
    } else if ("ukui-default" == nowThemeStyle || "ukui-light" == nowThemeStyle || "ukui-white" == nowThemeStyle){
        setThemeLight();
    } else {
        return;
    }
}

void menuModule::triggerMenu(QAction *act){



    QString str = act->text();
    if (tr("Quit") == str) {
        emit menuModuleClose();
    } else if (tr("About") == str) {
        aboutAction();
    } else if (tr("Help") == str) {
        helpAction();
    } else if (tr("Open") == str) {
        emit openSignal();
    }
}

void menuModule::aboutAction(){
//    关于点击事件处理

    initAbout();
}

void menuModule::helpAction(){
//    帮助点击事件处理

    m_appName = USER_GUIDE;
    if (!m_ipcDbus) {
        m_ipcDbus = new DaemonDbus();
    }

    if (!m_ipcDbus->daemonIsNotRunning()) {
        m_ipcDbus->showGuide(m_appName);
    }
}

void menuModule::initAbout(){

    emit aboutShow();
    m_aboutWindow->deleteLater();
    m_aboutWindow = new QDialog();
    m_aboutWindow->installEventFilter(this);
    m_aboutWindow->setWindowModality(Qt::ApplicationModal);
    m_aboutWindow->setWindowFlag(Qt::Tool);
    m_aboutWindow->setAutoFillBackground(true);
    m_aboutWindow->setBackgroundRole(QPalette::Base);


    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(m_aboutWindow->winId(), hints);

    m_aboutWindow->setFixedSize(M_ABOUT);
    m_aboutWindow->setMinimumHeight(M_ABOUT.height());
    QVBoxLayout *mainlyt = new QVBoxLayout();
    QHBoxLayout *titleLyt = initTitleBar();
    QVBoxLayout *bodylyt = initBody();
    mainlyt->setMargin(0);
    mainlyt->addLayout(titleLyt);
    mainlyt->addLayout(bodylyt);
    mainlyt->addStretch();
    m_aboutWindow->setLayout(mainlyt);
    //TODO:在中央显示
    QRect availableGeometry = this->parentWidget()->parentWidget()->geometry();
    m_aboutWindow->move(availableGeometry.center()-m_aboutWindow->rect().center());
    m_aboutWindow->show();
}

QHBoxLayout* menuModule::initTitleBar(){
//    QLabel* titleIcon = new QLabel();
    QPushButton *titleIcon = new QPushButton();
    titleIcon->setFixedSize(MICON_SIZEM);
    titleIcon->setIcon(QIcon::fromTheme("kylin-photo-viewer", QIcon(":/res/res/kyview_logo.png")));
    titleIcon->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                             "QPushButton::hover{border:0px;border-radius:4px;background:transparent;}"
                             "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;}");
    titleIcon->setIconSize(MICON_SIZEM);
    m_appShowingName = tr("Pictures");
    m_iconPath = ":/res/res/kyview_logo.png";
//    titleIcon->setPixmap(QPixmap::fromImage(QImage(m_iconPath)));
//    titleIcon->setScaledContents(true);

    QPushButton *titleBtnClose = new QPushButton;
    titleBtnClose->setIcon(QIcon::fromTheme("window-close-symbolic"));
    titleBtnClose->setToolTip(tr("close"));
    titleBtnClose->setFixedSize(MBTN_SIZE);
    titleBtnClose->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    titleBtnClose->setProperty("isWindowButton", 0x2);
    titleBtnClose->setProperty("useIconHighlightEffect", 0x8);
    titleBtnClose->setFlat(true);
    connect(titleBtnClose,&QPushButton::clicked,[=](){m_aboutWindow->close();});

    QHBoxLayout *hlyt = new QHBoxLayout;
    m_titleText->setText(tr("Pictures"));
    hlyt->setSpacing(0);
    hlyt->setMargin(4);
    hlyt->addSpacing(4);
    hlyt->addWidget(titleIcon,0,Qt::AlignBottom); //居下显示
    hlyt->addSpacing(8);
    hlyt->addWidget(m_titleText,0,Qt::AlignBottom);
    hlyt->addStretch();
    hlyt->addWidget(titleBtnClose,0,Qt::AlignBottom);
    return hlyt;
}

QVBoxLayout* menuModule::initBody(){
    m_appVersion = VERSION_NEM;
//    QLabel* bodyIcon = new QLabel();
    QPushButton *bodyIcon = new QPushButton();
    bodyIcon->setFixedSize(MICON_SIZEB);
//    bodyIcon->setPixmap(QPixmap::fromImage(QImage(m_iconPath)));
    bodyIcon->setIcon(QIcon::fromTheme("kylin-photo-viewer", QIcon(":/res/res/kyview_logo.png")));
//    bodyIcon->setStyleSheet("font-size:14px;background-color:transparent;");
    bodyIcon->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;}"
                            "QPushButton::hover{border:0px;border-radius:4px;background:transparent;}"
                            "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;}");
    bodyIcon->setIconSize(MICON_SIZEB);
//    bodyIcon->setScaledContents(true);
    m_bodyAppName->setFixedHeight(NAME_HEIGHT);
    m_bodyAppName->setText(tr(m_appShowingName.toLocal8Bit()));
    m_bodyAppVersion->setFixedHeight(VERSION_HEI);
    m_bodyAppVersion->setText(tr("Version: ") + m_appVersion);
    m_bodyAppVersion->setAlignment(Qt::AlignLeft);

    connect(m_bodySupport,&QLabel::linkActivated,this,[=](const QString url){
        QDesktopServices::openUrl(QUrl(url));
    });
    m_bodySupport->setFixedHeight(VERSION_HEI);
    m_bodySupport->setContextMenuPolicy(Qt::NoContextMenu);
    QVBoxLayout *vlyt = new QVBoxLayout;
    vlyt->setMargin(0);
    vlyt->setSpacing(0);
    vlyt->addSpacing(20);
    vlyt->addWidget(bodyIcon,0,Qt::AlignHCenter);
    vlyt->addSpacing(16);
    vlyt->addWidget(m_bodyAppName,0,Qt::AlignHCenter);
    vlyt->addSpacing(12);
    vlyt->addWidget(m_bodyAppVersion,0,Qt::AlignHCenter);
    vlyt->addSpacing(12);
    vlyt->addWidget(m_bodySupport,0,Qt::AlignHCenter);
    vlyt->addStretch();
    return vlyt;
}

void menuModule::initGsetting(){
    if (QGSettings::isSchemaInstalled(FITTHEMEWINDOW)) {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
    }

}

void menuModule::dealSystemGsettingChange(const QString key){
    if (key == "styleName") {
        refreshThemeBySystemConf();
    }
}

void menuModule::refreshThemeBySystemConf(){
    QString themeNow = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == themeNow || "ukui-black" == themeNow) {
        setThemeDark();
    } else {
        setThemeLight();
    }
}

void menuModule::setThemeDark(){


    m_titleText->setStyleSheet("color:rgba(255,255,255,1);font-size:14px;");
    m_bodyAppName->setStyleSheet("color:rgba(255,255,255,1);font-size:18px;");
    m_bodyAppVersion->setStyleSheet("color:rgba(255,255,255,1);font-size:14px;");
    m_bodySupport->setStyleSheet("color:rgba(255,255,255,1);font-size:14px;");

    m_bodySupport->setText(tr("Service & Support Team: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(255,255,255,1)\">"
                         "support@kylinos.cn</a>");
}

void menuModule::setThemeLight(){


    m_titleText->setStyleSheet("color:rgba(0,0,0,1);font-size:14px;");
    m_bodyAppName->setStyleSheet("color:rgba(0,0,0,1);font-size:18px;");
    m_bodyAppVersion->setStyleSheet("color:rgba(0,0,0,1);font-size:14px;");
    m_bodySupport->setStyleSheet("color:rgba(0,0,0,1);font-size:14px;");

    m_bodySupport->setText(tr("Service & Support Team: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(0,0,0,1)\">"
                         "support@kylinos.cn</a>");
}


