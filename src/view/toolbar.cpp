#include "toolbar.h"
#include "kyview.h"
#include "sizedate.h"
ToolBar::ToolBar(QWidget *parent) : QWidget(parent)
{

//    this->resize(678 +4 ,40 + 4);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->resize(SizeDate::TOOLBARSIZE);
    g_tooleWid = new QFrame(this);
    m_toolLayout = new QHBoxLayout(this);

    m_reduce = new QPushButton(this);
    m_reduce->setFixedSize(SizeDate::TOOLBUTTON);
    m_reduce->setFocusPolicy(Qt::NoFocus);
//    reduce->setAutoRepeat(true);
//    reduce->setAutoRepeatDelay(ms++);

    m_percentage = new QLabel(this);
    m_percentage->setFixedSize(SizeDate::TOOLPER);
    m_percentage->setText("98%");

    m_enlarge = new QPushButton(this);
    m_enlarge->setFixedSize(SizeDate::TOOLBUTTON);
    m_enlarge->setFocusPolicy(Qt::NoFocus);
//    enlarge->setAutoRepeat(true);
//    enlarge->setAutoRepeatDelay(ms++);

    m_originalSize = new QPushButton(this);
    m_originalSize->setFixedSize(SizeDate::TOOLBUTTON);
    m_originalSize->setFocusPolicy(Qt::NoFocus);

    m_adaptiveWidget = new QPushButton(this);
    m_adaptiveWidget->setFixedSize(SizeDate::TOOLBUTTON);
    m_adaptiveWidget->setFocusPolicy(Qt::NoFocus);

    m_rotate = new QPushButton(this);
    m_rotate->setFixedSize(SizeDate::TOOLBUTTON);
    m_rotate->setFocusPolicy(Qt::NoFocus);

    m_flipH = new QPushButton(this);
    m_flipH->setFixedSize(SizeDate::TOOLBUTTON);
    m_flipH->setFocusPolicy(Qt::NoFocus);

    m_flipV = new QPushButton(this);
    m_flipV->setFixedSize(SizeDate::TOOLBUTTON);
    m_flipV->setFocusPolicy(Qt::NoFocus);

//    cutImage = new QPushButton(this);
//    cutImage->setFixedSize(SizeDate::TOOLBUTTON);
//    cutImage->setFocusPolicy(Qt::NoFocus);

    m_line1 = new QLabel(this);
    m_line1->setFixedSize(SizeDate::TOOLLINE);

//    filter = new QPushButton(this);
//    filter->setFixedSize(SizeDate::TOOLBUTTON);
//    filter->setFocusPolicy(Qt::NoFocus);

//    labelbar = new QPushButton(this);//
//    labelbar->setFixedSize(SizeDate::TOOLBUTTON);
//    labelbar->setFocusPolicy(Qt::NoFocus);

//    line2 = new QLabel(this);
//    line2->setFixedSize(SizeDate::TOOLLINE);

//    sidebar = new QPushButton(this);//
//    sidebar->setFixedSize(SizeDate::TOOLBUTTON);
//    sidebar->setFocusPolicy(Qt::NoFocus);

    m_information = new QPushButton(this);//
    m_information->setFixedSize(SizeDate::TOOLBUTTON);
    m_information->setFocusPolicy(Qt::NoFocus);

    m_delImage = new QPushButton(this);
    m_delImage->setFixedSize(SizeDate::TOOLBUTTON);
    m_delImage->setFocusPolicy(Qt::NoFocus);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(SizeDate::TOOLCOLOR);       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(SizeDate::BLURRADIUS);        //设定阴影的模糊半径，数值越大越模糊
    g_tooleWid->setGraphicsEffect(effect);

    this->initGsetting();
    this->initControlQss();
    this->setstyle();
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
    m_toolLayout->setSpacing(20);
//    m_toolLayout->addWidget(m_cutImage,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_line1,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
//    m_toolLayout->addWidget(m_filter,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
//    m_toolLayout->addWidget(m_labelbar,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(16);
//    m_toolLayout->addWidget(m_line2,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
//    m_toolLayout->addWidget(m_sidebar,0,Qt::AlignCenter);
//    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_information,0,Qt::AlignCenter);
    m_toolLayout->setSpacing(20);
    m_toolLayout->addWidget(m_delImage,0,Qt::AlignCenter);
//    toolLayout->setMargin(20);
    m_toolLayout->setContentsMargins(18,8,18,12);
    g_tooleWid->setLayout(m_toolLayout);
    g_tooleWid->move(4,4);
    g_tooleWid->resize(this->width()-8,this->height()-2);

}
//样式
void ToolBar::setstyle()
{
    this->setStyleSheet("QWidget{border-radius:4px;}");
    g_tooleWid->setStyleSheet("background-color:white;border-radius:4px;");
    m_percentage->setStyleSheet("QLabel{background-color:transparent;}");
    m_line1->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");
//    m_line2->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");

    m_reduce->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/reduce.png);}"
                          "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/reduce_hover.png);}"
                          "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/reduce_hover.png);}");

    m_enlarge->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/enlarge.png);}"
                           "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/enlarge_hover.png);}"
                           "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/enlarge_hover.png);}");

    m_originalSize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/originalSize.png);}"
                                "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/originalSize_hover.png);}"
                                "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/originalSize_hover.png);}");

    m_adaptiveWidget->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/adaptiveWidget.png);}"
                                  "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/adaptiveWidget_hover.png);}"
                                  "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/adaptiveWidget_hover.png);}");

    m_rotate->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/rotate.png);}"
                          "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/rotate_hover.png);}"
                          "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/rotate_hover.png);}");

    m_flipH->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipH.png);}"
                         "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipH_hover.png);}"
                         "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipH_hover.png);}");

    m_flipV->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipV.png);}"
                         "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipV_hover.png);}"
                         "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipV_hover.png);}");

//    cutImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/cutImage.png);}"
//                            "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/cutImage_hover.png);}"
//                            "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/cutImage_hover.png);}");

//    filter->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/filter.png);}"
//                          "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/filter_hover.png);}"
//                          "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/filter_hover.png);}");

//    labelbar->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/labelbar.png);}"
//                            "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/labelbar_hover.png);}"
//                            "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/labelbar_hover.png);}");

//    sidebar->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/sidebar.png);}"
//                           "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/sidebar_hover.png);}"
//                           "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/sidebar_hover.png);}");

    m_information->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/information.png);}"
                               "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/information_hover.png);}"
                               "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/information_hover.png);}");

    m_delImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/delImage.png);}"
                            "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/delImage_hover.png);}"
                            "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/delImage_hover.png);}");


}
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
//    connect(m_sidebar,&QPushButton::clicked,this,&ToolBar::sidebar);
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

void ToolBar::initGsetting(){
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW)){
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&ToolBar::dealSystemGsettingChange);
    }

}

void ToolBar::dealSystemGsettingChange(const QString key){
    if(key == "styleName"){
        changeStyle();
    }
}
void ToolBar::changeStyle()
{
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {
        qDebug()<<"换图标";
    }else{
        qDebug()<<"换图标";
    }
}




