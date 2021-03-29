#include "toolbar.h"
#include "kyview.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent)
{

//    this->resize(678 +4 ,40 + 4);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->resize(479 +4+14 ,Variable::BAR_HEIGHT + 4);
    tooleWid = new QFrame(this);
    toolLayout = new QHBoxLayout(this);

    reduce = new QPushButton(this);
    reduce->setFixedSize(24,24);
    reduce->setFocusPolicy(Qt::NoFocus);
//    reduce->setAutoRepeat(true);
//    reduce->setAutoRepeatDelay(ms++);

    percentage = new QLabel(this);
    percentage->setFixedSize(43,16);
    percentage->setText("98%");

    enlarge = new QPushButton(this);
    enlarge->setFixedSize(24,24);
    enlarge->setFocusPolicy(Qt::NoFocus);
//    enlarge->setAutoRepeat(true);
//    enlarge->setAutoRepeatDelay(ms++);

    originalSize = new QPushButton(this);
    originalSize->setFixedSize(24,24);
    originalSize->setFocusPolicy(Qt::NoFocus);

    adaptiveWidget = new QPushButton(this);
    adaptiveWidget->setFixedSize(24,24);
    adaptiveWidget->setFocusPolicy(Qt::NoFocus);

    rotate = new QPushButton(this);
    rotate->setFixedSize(24,24);
    rotate->setFocusPolicy(Qt::NoFocus);

    flipH = new QPushButton(this);
    flipH->setFixedSize(24,24);
    flipH->setFocusPolicy(Qt::NoFocus);

    flipV = new QPushButton(this);
    flipV->setFixedSize(24,24);
    flipV->setFocusPolicy(Qt::NoFocus);

//    cutImage = new QPushButton(this);
//    cutImage->setFixedSize(24,24);
//    cutImage->setFocusPolicy(Qt::NoFocus);

    line1 = new QLabel(this);
    line1->setFixedSize(1,22);

//    filter = new QPushButton(this);
//    filter->setFixedSize(24,24);
//    filter->setFocusPolicy(Qt::NoFocus);

//    labelbar = new QPushButton(this);//
//    labelbar->setFixedSize(24,24);
//    labelbar->setFocusPolicy(Qt::NoFocus);

//    line2 = new QLabel(this);
//    line2->setFixedSize(1,22);

//    sidebar = new QPushButton(this);//
//    sidebar->setFixedSize(24,24);
//    sidebar->setFocusPolicy(Qt::NoFocus);

    information = new QPushButton(this);//
    information->setFixedSize(24,24);
    information->setFocusPolicy(Qt::NoFocus);

    delImage = new QPushButton(this);
    delImage->setFixedSize(24,24);
    delImage->setFocusPolicy(Qt::NoFocus);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(QColor(54,54,54));       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(10);        //设定阴影的模糊半径，数值越大越模糊
    tooleWid->setGraphicsEffect(effect);

    this->_initGsetting();
    this->_initControlQss();
    this->_setstyle();
    this->_initConnect();


    interaction=Interaction::getInstance();
}
//布局
void ToolBar::_initControlQss()
{

    toolLayout->addWidget(reduce,0,Qt::AlignCenter);
    toolLayout->setSpacing(10);
    toolLayout->addWidget(percentage,0,Qt::AlignCenter);
    toolLayout->setSpacing(10);
    toolLayout->addWidget(enlarge,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(originalSize,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(adaptiveWidget,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(rotate,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(flipH,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(flipV,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
//    toolLayout->addWidget(cutImage,0,Qt::AlignCenter);
//    toolLayout->setSpacing(20);
    toolLayout->addWidget(line1,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
//    toolLayout->addWidget(filter,0,Qt::AlignCenter);
//    toolLayout->setSpacing(20);
//    toolLayout->addWidget(labelbar,0,Qt::AlignCenter);
//    toolLayout->setSpacing(16);
//    toolLayout->addWidget(line2,0,Qt::AlignCenter);
//    toolLayout->setSpacing(20);
//    toolLayout->addWidget(sidebar,0,Qt::AlignCenter);
//    toolLayout->setSpacing(20);
    toolLayout->addWidget(information,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(delImage,0,Qt::AlignCenter);
//    toolLayout->setMargin(20);
    toolLayout->setContentsMargins(18,8,18,12);
    tooleWid->setLayout(toolLayout);
    tooleWid->move(4,4);
    tooleWid->resize(this->width()-8,this->height()-2);

}
//样式
void ToolBar::_setstyle()
{
    this->setStyleSheet("QWidget{border-radius:4px;}");
    tooleWid->setStyleSheet("background-color:white;border-radius:4px;");
    percentage->setStyleSheet("QLabel{background-color:transparent;}");
    line1->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");
//    line2->setStyleSheet("QLabel{border: 1px solid #393939;background-color: #393939;}");

    reduce->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/reduce.png);}"
                          "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/reduce_hover.png);}"
                          "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/reduce_hover.png);}");

    enlarge->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/enlarge.png);}"
                           "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/enlarge_hover.png);}"
                           "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/enlarge_hover.png);}");

    originalSize->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/originalSize.png);}"
                                "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/originalSize_hover.png);}"
                                "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/originalSize_hover.png);}");

    adaptiveWidget->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/adaptiveWidget.png);}"
                                  "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/adaptiveWidget_hover.png);}"
                                  "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/adaptiveWidget_hover.png);}");

    rotate->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/rotate.png);}"
                          "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/rotate_hover.png);}"
                          "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/rotate_hover.png);}");

    flipH->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipH.png);}"
                         "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipH_hover.png);}"
                         "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipH_hover.png);}");

    flipV->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/flipV.png);}"
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

    information->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/information.png);}"
                               "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/information_hover.png);}"
                               "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/information_hover.png);}");

    delImage->setStyleSheet("QPushButton{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/delImage.png);}"
                            "QPushButton::hover{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/delImage_hover.png);}"
                            "QPushButton::pressed{border:0px;border-radius:4px;background:white;background-image: url(:/res/res/delImage_hover.png);}");


}
void ToolBar::_initConnect()
{

    connect(reduce,&QPushButton::clicked,this,&ToolBar::reduceImage);
    connect(enlarge,&QPushButton::clicked,this,&ToolBar::enlargeImage);
    connect(rotate,&QPushButton::clicked,this,&ToolBar::_rotate);
    connect(originalSize,&QPushButton::clicked,this,&ToolBar::_originalSize);
    connect(adaptiveWidget,&QPushButton::clicked,this,&ToolBar::_adaptiveWidget);
    connect(flipH,&QPushButton::clicked,this,&ToolBar::_flipH);
    connect(flipV,&QPushButton::clicked,this,&ToolBar::_flipV);
//    connect(cutImage,&QPushButton::clicked,this,&ToolBar::_cutImage);
//    connect(filter,&QPushButton::clicked,this,&ToolBar::_filter);
//    connect(labelbar,&QPushButton::clicked,this,&ToolBar::_labelbar);
//    connect(sidebar,&QPushButton::clicked,this,&ToolBar::_sidebar);
    connect(information,&QPushButton::clicked,this,&ToolBar::_information);
    connect(delImage,&QPushButton::clicked,this,&ToolBar::_delImage);


}
//改变百分比的数值
void ToolBar::changePerRate(QString num)
{
    this->percentage->setText(num);
}

void ToolBar::reduceImage()
{

    interaction->watchSmallImage();
}

void ToolBar::enlargeImage()
{
    interaction->watchBigImage();
}

void ToolBar::_originalSize()
{
    interaction->watchOriginalImage();
}

void ToolBar::_adaptiveWidget()
{
    interaction->watchAutoImage();
}

void ToolBar::_rotate()
{
    interaction->rotate();
}

void ToolBar::_flipH()
{
    interaction->flipH();
}

void ToolBar::_flipV()
{
    interaction->flipV();
}

void ToolBar::_cutImage()
{
    qDebug()<<"裁剪";
}

void ToolBar::_filter()
{
    qDebug()<<"滤镜";
}

void ToolBar::_labelbar()
{
    qDebug()<<"标注";
}

void ToolBar::_sidebar()
{
    qDebug()<<"相册侧栏";
}

void ToolBar::_information()
{
    emit showInfor();
}

void ToolBar::_delImage()
{
    interaction->deleteImage();
}

void ToolBar::_initGsetting(){
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW)){
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&ToolBar::_dealSystemGsettingChange);
    }

}

void ToolBar::_dealSystemGsettingChange(const QString key){
    if(key == "styleName"){
        _changeStyle();
    }
}
void ToolBar::_changeStyle()
{
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {

    }else{

    }
}




