#include "menumodule.h"
#include "kyview.h"
#include "xatom-helper.h"

menuModule::menuModule(QWidget *parent = nullptr) : QWidget(parent)
{
    init();
}


void menuModule::init(){
    initAction();
}

void menuModule::initAction(){

    bodyAppName = new QLabel();

    bodyAppVersion = new QLabel();
    bodySupport = new QLabel();
    titleText = new QLabel();
    iconSize = QSize(30,30);


    bodySupport->setText(tr("Service & Support Team: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(0,0,0,1)\">"
                         "support@kylinos.cn</a>");
    bodyAppVersion->setText(tr("Version: ") + appVersion);

    menuButton = new QToolButton;
    menuButton->setToolTip(tr("menu"));
    menuButton->setIcon(QIcon::fromTheme("open-menu-symbolic"));
    menuButton->setProperty("isWindowButton", 0x1);
    menuButton->setProperty("useIconHighlightEffect",0x2);
    menuButton->setIconSize(QSize(16,16));
    menuButton->setPopupMode(QToolButton::InstantPopup);
    menuButton->setFixedSize(iconSize);
    menuButton->setAutoRaise(true);

    m_menu = new QMenu();

    QList<QAction *> actions ;
    QAction *actionOpen = new QAction(m_menu);
    actionOpen->setText(tr("Open.."));
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
    QMenu *themeMenu = new QMenu;
    QActionGroup *themeMenuGroup = new QActionGroup(this);
    QAction *autoTheme = new QAction("Auto",this);
    themeMenuGroup->addAction(autoTheme);
    themeMenu->addAction(autoTheme);
    autoTheme->setCheckable(true);
    QAction *lightTheme = new QAction("Light",this);
    themeMenuGroup->addAction(lightTheme);
    themeMenu->addAction(lightTheme);
    lightTheme->setCheckable(true);
    QAction *darkTheme = new QAction("Dark",this);
    themeMenuGroup->addAction(darkTheme);
    themeMenu->addAction(darkTheme);
    darkTheme->setCheckable(true);
    QList<QAction* > themeActions;
    themeActions<<autoTheme<<lightTheme<<darkTheme;
    actionTheme->setMenu(themeMenu);
    menuButton->setMenu(m_menu);
    connect(m_menu,&QMenu::triggered,this,&menuModule::triggerMenu);
    connect(this,&menuModule::openSignal,KyView::mutual,&KyView::menuopen);
    connect(this,&menuModule::menuModuleClose,KyView::mutual,&KyView::close);

    initGsetting();
}



void menuModule::setStyleByThemeGsetting(){
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {
        setThemeDark();
    }else if("ukui-default" == nowThemeStyle || "ukui-light" == nowThemeStyle || "ukui-white" == nowThemeStyle){
        setThemeLight();
    }else{
        return;
    }
}

void menuModule::triggerMenu(QAction *act){



    QString str = act->text();
    if(tr("Quit") == str){
        emit menuModuleClose();
    }else if(tr("About") == str){
        aboutAction();
    }else if(tr("Help") == str){
        helpAction();
    }else if(tr("Open..") == str){
        emit openSignal();
    }
}

void menuModule::aboutAction(){
//    关于点击事件处理

    initAbout();
}

void menuModule::helpAction(){
//    帮助点击事件处理

    appName = "tools/indicator-china-weather";
    if(!ipcDbus){
        ipcDbus = new DaemonDbus();
    }

    if(!ipcDbus->daemonIsNotRunning()){
        ipcDbus->showGuide(appName);
    }
}

void menuModule::initAbout(){
    KyView::mutual->titlebar->hide();
    KyView::mutual->toolbar->hide();
    aboutWindow->deleteLater();
    aboutWindow = new QDialog();
    aboutWindow->installEventFilter(this);
    aboutWindow->setWindowModality(Qt::ApplicationModal);
    aboutWindow->setWindowFlag(Qt::Tool);
    aboutWindow->setAutoFillBackground(true);
    aboutWindow->setBackgroundRole(QPalette::Base);


    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(aboutWindow->winId(), hints);

    aboutWindow->setFixedSize(420,324);
    aboutWindow->setMinimumHeight(324);
    QVBoxLayout *mainlyt = new QVBoxLayout();
    QHBoxLayout *titleLyt = initTitleBar();
    QVBoxLayout *bodylyt = initBody();
    mainlyt->setMargin(0);
    mainlyt->addLayout(titleLyt);
    mainlyt->addLayout(bodylyt);
    mainlyt->addStretch();
    aboutWindow->setLayout(mainlyt);
    //TODO:在中央显示
    QRect availableGeometry = this->parentWidget()->parentWidget()->geometry();
    aboutWindow->move(availableGeometry.center()-aboutWindow->rect().center());
    aboutWindow->show();
}

QHBoxLayout* menuModule::initTitleBar(){
    QLabel* titleIcon = new QLabel();
    titleIcon->setFixedSize(QSize(24,24));

    appShowingName = tr("kylin photo view");
    iconPath = ":/res/res/kyview_logo.png";
    titleIcon->setPixmap(QPixmap::fromImage(QImage(iconPath)));
    titleIcon->setScaledContents(true);

    QPushButton *titleBtnClose = new QPushButton;
    titleBtnClose->setIcon(QIcon::fromTheme("window-close-symbolic"));
    titleBtnClose->setToolTip(tr("close"));
    titleBtnClose->setFixedSize(30,30);
    titleBtnClose->setFocusPolicy(Qt::NoFocus);//设置焦点类型
    titleBtnClose->setProperty("isWindowButton", 0x2);
    titleBtnClose->setProperty("useIconHighlightEffect", 0x8);
    titleBtnClose->setFlat(true);
    connect(titleBtnClose,&QPushButton::clicked,[=](){aboutWindow->close();});

    QHBoxLayout *hlyt = new QHBoxLayout;
    titleText->setText(tr("Kylin Photo View"));
    hlyt->setSpacing(0);
    hlyt->setMargin(4);
    hlyt->addSpacing(4);
    hlyt->addWidget(titleIcon,0,Qt::AlignBottom); //居下显示
    hlyt->addSpacing(8);
    hlyt->addWidget(titleText,0,Qt::AlignBottom);
    hlyt->addStretch();
    hlyt->addWidget(titleBtnClose,0,Qt::AlignBottom);
    return hlyt;
}

QVBoxLayout* menuModule::initBody(){
    appVersion = "1.0.0";
    QLabel* bodyIcon = new QLabel();
    bodyIcon->setFixedSize(96,96);
    bodyIcon->setPixmap(QPixmap::fromImage(QImage(iconPath)));
    bodyIcon->setStyleSheet("font-size:14px;");
    bodyIcon->setScaledContents(true);
    bodyAppName->setFixedHeight(28);
    bodyAppName->setText(tr(appShowingName.toLocal8Bit()));
    bodyAppVersion->setFixedHeight(24);
    bodyAppVersion->setText(tr("Version: ") + appVersion);
    bodyAppVersion->setAlignment(Qt::AlignLeft);

    connect(bodySupport,&QLabel::linkActivated,this,[=](const QString url){
        QDesktopServices::openUrl(QUrl(url));
    });
    bodySupport->setFixedHeight(24);
    bodySupport->setContextMenuPolicy(Qt::NoContextMenu);
    QVBoxLayout *vlyt = new QVBoxLayout;
    vlyt->setMargin(0);
    vlyt->setSpacing(0);
    vlyt->addSpacing(20);
    vlyt->addWidget(bodyIcon,0,Qt::AlignHCenter);
    vlyt->addSpacing(16);
    vlyt->addWidget(bodyAppName,0,Qt::AlignHCenter);
    vlyt->addSpacing(12);
    vlyt->addWidget(bodyAppVersion,0,Qt::AlignHCenter);
    vlyt->addSpacing(12);
    vlyt->addWidget(bodySupport,0,Qt::AlignHCenter);
    vlyt->addStretch();
    return vlyt;
}

void menuModule::initGsetting(){
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW)){
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
    }

}

void menuModule::dealSystemGsettingChange(const QString key){
    if(key == "styleName"){
        refreshThemeBySystemConf();
    }
}

void menuModule::refreshThemeBySystemConf(){
    QString themeNow = m_pGsettingThemeData->get("styleName").toString();
//    qDebug()<<"themenow:"<<themeNow;
    if("ukui-dark" == themeNow || "ukui-black" == themeNow){
        setThemeDark();
    }else{
        setThemeLight();
    }
}

void menuModule::setThemeDark(){


    titleText->setStyleSheet("color:rgba(255,255,255,1);font-size:14px;");
    bodyAppName->setStyleSheet("color:rgba(255,255,255,1);font-size:18px;");
    bodyAppVersion->setStyleSheet("color:rgba(255,255,255,1);font-size:14px;");
    bodySupport->setStyleSheet("color:rgba(255,255,255,1);font-size:14px;");

    bodySupport->setText(tr("Service & Support Team: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(255,255,255,1)\">"
                         "support@kylinos.cn</a>");
}

void menuModule::setThemeLight(){


    titleText->setStyleSheet("color:rgba(0,0,0,1);font-size:14px;");
    bodyAppName->setStyleSheet("color:rgba(0,0,0,1);font-size:18px;");
    bodyAppVersion->setStyleSheet("color:rgba(0,0,0,1);font-size:14px;");
    bodySupport->setStyleSheet("color:rgba(0,0,0,1);font-size:14px;");

    bodySupport->setText(tr("Service & Support Team: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(0,0,0,1)\">"
                         "support@kylinos.cn</a>");
}


