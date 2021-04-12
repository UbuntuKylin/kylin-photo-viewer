#include "toolbar.h"
#include "kyview.h"
#include "sizedate.h"
ToolBar::ToolBar(QWidget *parent) : QWidget(parent)
{

//    this->resize(678 +4 ,40 + 4);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(TOOLBARSIZE);
    //布局
    g_tooleWid = new QFrame(this);
    m_toolLayout = new QHBoxLayout(this);
    //缩小
    m_reduce = new QPushButton(this);
    m_reduce->setFixedSize(TOOLBUTTON);
    m_reduce->setFocusPolicy(Qt::NoFocus);
//    reduce->setAutoRepeat(true);
//    reduce->setAutoRepeatDelay(ms++);
    //百分比
    m_percentage = new QLabel(this);
    m_percentage->resize(TOOLPER);
    m_percentage->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
//    m_percentage->setText("98%");
    //放大
    m_enlarge = new QPushButton(this);
    m_enlarge->setFixedSize(TOOLBUTTON);
    m_enlarge->setFocusPolicy(Qt::NoFocus);
//    enlarge->setAutoRepeat(true);
//    enlarge->setAutoRepeatDelay(ms++);
    //原始尺寸
    m_originalSize = new QPushButton(this);
    m_originalSize->setFixedSize(TOOLBUTTON);
    m_originalSize->setFocusPolicy(Qt::NoFocus);
    //适应窗口
    m_adaptiveWidget = new QPushButton(this);
    m_adaptiveWidget->setFixedSize(TOOLBUTTON);
    m_adaptiveWidget->setFocusPolicy(Qt::NoFocus);
    //旋转
    m_rotate = new QPushButton(this);
    m_rotate->setFixedSize(TOOLBUTTON);
    m_rotate->setFocusPolicy(Qt::NoFocus);
    //水平翻转
    m_flipH = new QPushButton(this);
    m_flipH->setFixedSize(TOOLBUTTON);
    m_flipH->setFocusPolicy(Qt::NoFocus);
    //垂直翻转
    m_flipV = new QPushButton(this);
    m_flipV->setFixedSize(TOOLBUTTON);
    m_flipV->setFocusPolicy(Qt::NoFocus);
//裁剪
//    cutImage = new QPushButton(this);
//    cutImage->setFixedSize(TOOLBUTTON);
//    cutImage->setFocusPolicy(Qt::NoFocus);

    m_line1 = new QLabel(this);
    m_line1->setFixedSize(TOOLLINE);
//滤镜
//    filter = new QPushButton(this);
//    filter->setFixedSize(TOOLBUTTON);
//    filter->setFocusPolicy(Qt::NoFocus);
//标注
//    labelbar = new QPushButton(this);//
//    labelbar->setFixedSize(TOOLBUTTON);
//    labelbar->setFocusPolicy(Qt::NoFocus);

//    line2 = new QLabel(this);
//    line2->setFixedSize(TOOLLINE);
    //侧边栏--相册
    m_sidebar = new QPushButton(this);
    m_sidebar->setFixedSize(TOOLBUTTON);
    m_sidebar->setFocusPolicy(Qt::NoFocus);
    //信息栏
    m_information = new QPushButton(this);//
    m_information->setFixedSize(TOOLBUTTON);
    m_information->setFocusPolicy(Qt::NoFocus);
    //删除
    m_delImage = new QPushButton(this);
    m_delImage->setFixedSize(TOOLBUTTON);
    m_delImage->setFocusPolicy(Qt::NoFocus);
    //绘制阴影
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(TOOLCOLOR);       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(BLURRADIUS);        //设定阴影的模糊半径，数值越大越模糊
    g_tooleWid->setGraphicsEffect(effect);

    this->initGsetting();
    this->initControlQss();
    this->initConnect();

    m_interaction=Interaction::getInstance();
}
//布局
void ToolBar::initControlQss()
{

    m_toolLayout->addWidget(m_reduce,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(10);
    m_toolLayout->addWidget(m_percentage,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(10);
    m_toolLayout->addWidget(m_enlarge,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_adaptiveWidget,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_originalSize,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_rotate,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_flipH,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_flipV,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
//    m_toolLayout->addWidget(m_cutImage,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(10);
    m_toolLayout->addWidget(m_line1,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(10);
//    m_toolLayout->addWidget(m_filter,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
//    m_toolLayout->addWidget(m_labelbar,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(16);
//    m_toolLayout->addWidget(m_line2,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_sidebar,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_information,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_delImage,0,Qt::AlignCenter);
//    m_toolLayout->setMargin(20);
    m_toolLayout->setContentsMargins(18,8,18,12);
    g_tooleWid->setLayout(m_toolLayout);
    g_tooleWid->move(4,4);
    g_tooleWid->resize(this->width()-8,this->height()-2);

}
//信号槽
void ToolBar::initConnect()
{

    connect(m_reduce,&QPushButton::clicked,this,&ToolBar::reduceImage);
    connect(m_enlarge,&QPushButton::clicked,this,&ToolBar::enlargeImage);
    connect(m_rotate,&QPushButton::clicked,this,&ToolBar::rotate);
    connect(m_originalSize,&QPushButton::clicked,this,&ToolBar::originalSize);
    connect(m_adaptiveWidget,&QPushButton::clicked,this,&ToolBar::adaptiveWidget);
    connect(m_flipH,&QPushButton::clicked,this,&ToolBar::flipH);
    connect(m_flipV,&QPushButton::clicked,this,&ToolBar::flipV);
//    connect(m_cutImage,&QPushButton::clicked,this,&ToolBar::cutImage);
//    connect(m_filter,&QPushButton::clicked,this,&ToolBar::filter);
//    connect(m_labelbar,&QPushButton::clicked,this,&ToolBar::labelbar);
    connect(m_sidebar,&QPushButton::clicked,this,&ToolBar::sidebar);
    connect(m_information,&QPushButton::clicked,this,&ToolBar::information);
    connect(m_delImage,&QPushButton::clicked,this,&ToolBar::delImage);


}
//改变百分比的数值
void ToolBar::changePerRate(QString num)
{
    this->m_percentage->setText(num);
}
//缩小
void ToolBar::reduceImage()
{

    m_interaction->watchSmallImage();
}
//放大
void ToolBar::enlargeImage()
{
    m_interaction->watchBigImage();
}

//原始大小
void ToolBar::originalSize()
{
    m_interaction->watchOriginalImage();
}
//适应窗口
void ToolBar::adaptiveWidget()
{
    m_interaction->watchAutoImage();
}
//旋转
void ToolBar::rotate()
{
    m_interaction->rotate();
}
//水平镜像
void ToolBar::flipH()
{
    m_interaction->flipH();
}
//垂直镜像
void ToolBar::flipV()
{
    m_interaction->flipV();
}
//裁剪
void ToolBar::cutImage()
{
    qDebug()<<"裁剪";
}
//滤镜
void ToolBar::filter()
{
    qDebug()<<"滤镜";
}
//标注
void ToolBar::labelbar()
{
    qDebug()<<"标注";
}
//侧边栏
void ToolBar::sidebar()
{
    qDebug()<<"相册侧栏";
    emit showSidebar();
}
//信息栏展示
void ToolBar::information()
{
    emit showInfor();
}
//删除图片
void ToolBar::delImage()
{
    m_interaction->deleteImage();
}
//监听主题
void ToolBar::initGsetting(){
    if (QGSettings::isSchemaInstalled(FITTHEMEWINDOW)) {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&ToolBar::dealSystemGsettingChange);
    }

}

void ToolBar::dealSystemGsettingChange(const QString key){
    if (key == "styleName") {
        changeStyle();
    }
}
void ToolBar::changeStyle()
{
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if ("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {
        this->setStyleSheet("QWidget{border-radius:4px;}");
        g_tooleWid->setStyleSheet("background-color:rgba(0,0,0,0.66);border-radius:4px;");
        m_percentage->setStyleSheet("QLabel{background-color:transparent;}");
        m_line1->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");
    //    m_line2->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");
        m_reduce->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1reduce.png);}"
                              "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1reduce_hover.png);}"
                              "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1reduce_hover.png);}");

        m_enlarge->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1enlarge.png);}"
                               "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1enlarge_hover.png);}"
                               "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1enlarge_hover.png);}");

        m_originalSize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1originalSize.png);}"
                                    "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1originalSize_hover.png);}"
                                    "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1originalSize_hover.png);}");

        m_adaptiveWidget->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1adaptiveWidget.png);}"
                                      "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1adaptiveWidget_hover.png);}"
                                      "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1adaptiveWidget_hover.png);}");

        m_rotate->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1rotate.png);}"
                              "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1rotate_hover.png);}"
                              "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1rotate_hover.png);}");

        m_flipH->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1flipH.png);}"
                             "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1flipH_hover.png);}"
                             "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1flipH_hover.png);}");

        m_flipV->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1flipV.png);}"
                             "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1flipV_hover.png);}"
                             "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1flipV_hover.png);}");

    //    cutImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1cutImage.png);}"
    //                            "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1cutImage_hover.png);}"
    //                            "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1cutImage_hover.png);}");

    //    filter->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1filter.png);}"
    //                          "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1filter_hover.png);}"
    //                          "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1filter_hover.png);}");

    //    labelbar->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1labelbar.png);}"
    //                            "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1labelbar_hover.png);}"
    //                            "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1labelbar_hover.png);}");

        m_sidebar->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1sidebar.png);}"
                               "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1sidebar_hover.png);}"
                               "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1sidebar_hover.png);}");

        m_information->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1information.png);}"
                                   "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1information_hover.png);}"
                                   "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1information_hover.png);}");

        m_delImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1delImage.png);}"
                                "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1delImage_hover.png);}"
                                "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/1delImage_hover.png);}");

    }else{
        this->setStyleSheet("QWidget{border-radius:4px;}");
        g_tooleWid->setStyleSheet("background-color:rgba(255,255,255,1);");
        m_percentage->setStyleSheet("QLabel{background-color:transparent;}");
        m_line1->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");
    //    m_line2->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");
        m_reduce->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/reduce.png);}"
                              "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/reduce_hover.png);}"
                              "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/reduce_hover.png);}");

        m_enlarge->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/enlarge.png);}"
                               "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/enlarge_hover.png);}"
                               "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/enlarge_hover.png);}");

        m_originalSize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/originalSize.png);}"
                                    "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/originalSize_hover.png);}"
                                    "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/originalSize_hover.png);}");

        m_adaptiveWidget->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/adaptiveWidget.png);}"
                                      "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/adaptiveWidget_hover.png);}"
                                      "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/adaptiveWidget_hover.png);}");

        m_rotate->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/rotate.png);}"
                              "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/rotate_hover.png);}"
                              "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/rotate_hover.png);}");

        m_flipH->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/flipH.png);}"
                             "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/flipH_hover.png);}"
                             "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/flipH_hover.png);}");

        m_flipV->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/flipV.png);}"
                             "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/flipV_hover.png);}"
                             "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/flipV_hover.png);}");

    //    cutImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/cutImage.png);}"
    //                            "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/cutImage_hover.png);}"
    //                            "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/cutImage_hover.png);}");

    //    filter->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/filter.png);}"
    //                          "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/filter_hover.png);}"
    //                          "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/filter_hover.png);}");

    //    labelbar->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/labelbar.png);}"
    //                            "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/labelbar_hover.png);}"
    //                            "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/labelbar_hover.png);}");

        m_sidebar->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/sidebar.png);}"
                               "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/sidebar_hover.png);}"
                               "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/sidebar_hover.png);}");

        m_information->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/information.png);}"
                                   "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/information_hover.png);}"
                                   "QPushButton::pressed{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/information_hover.png);}");

        m_delImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/delImage.png);}"
                                "QPushButton::hover{border:0px;border-radius:4px;background:transparent;background-image: url(:/res/res/delImage_hover.png);}"
                                "QPushButton::pressed{border:0px;border-radius:4px;background:;background-image: url(:/res/res/delImage_hover.png);}");

    }
}




