#include "toolbar.h"
#include "kyview.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent)
{


//    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    toolLayout = new QHBoxLayout(this);
    reduce = new QPushButton(this);
    reduce->setFixedSize(24,24);

    reduce->setFocusPolicy(Qt::NoFocus);

    percentage = new QLabel(this);
    percentage->setFixedSize(43,16);
    percentage->setText("98%");
    percentage->setAlignment(Qt::AlignCenter);

    enlarge = new QPushButton(this);
    enlarge->setFixedSize(24,24);
    enlarge->setFocusPolicy(Qt::NoFocus);

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

    cutImage = new QPushButton(this);
    cutImage->setFixedSize(24,24);
    cutImage->setFocusPolicy(Qt::NoFocus);

    line1 = new QLabel(this);
    line1->setFixedSize(1,22);

    filter = new QPushButton(this);
    filter->setFixedSize(24,24);
    filter->setFocusPolicy(Qt::NoFocus);

    labelbar = new QPushButton(this);//
    labelbar->setFixedSize(24,24);
    labelbar->setFocusPolicy(Qt::NoFocus);

    line2 = new QLabel(this);
    line2->setFixedSize(1,22);

    sidebar = new QPushButton(this);//
    sidebar->setFixedSize(24,24);
    sidebar->setFocusPolicy(Qt::NoFocus);

    information = new QPushButton(this);//
    information->setFixedSize(24,24);
    information->setFocusPolicy(Qt::NoFocus);

    delImage = new QPushButton(this);
    delImage->setFixedSize(24,24);
    delImage->setFocusPolicy(Qt::NoFocus);

    //设置窗体透明
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    this->_initControlQss();
    this->_setstyle();
    this->_initConnect();

//    installEventFilter(this);


    interaction=Interaction::getInstance();
}
//布局
void ToolBar::_initControlQss()
{

    toolLayout->addWidget(reduce,0,Qt::AlignCenter);
    toolLayout->setSpacing(10);
    toolLayout->addWidget(percentage,0,Qt::AlignLeft);
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
    toolLayout->addWidget(cutImage,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(line1,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(filter,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(labelbar,0,Qt::AlignCenter);
    toolLayout->setSpacing(16);
    toolLayout->addWidget(line2,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(sidebar,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(information,0,Qt::AlignCenter);
    toolLayout->setSpacing(20);
    toolLayout->addWidget(delImage,0,Qt::AlignCenter);
//    toolLayout->setMargin(20);
    toolLayout->setContentsMargins(20,8,20,8);
    this->setLayout(toolLayout);

//    tooleWid->move(0,4);

}
//样式
void ToolBar::_setstyle()
{
//    this->setObjectName("mainWid");
//    this->setStyleSheet("background-color:white;");

    percentage->setStyleSheet("QLabel{background-color:transparent;}");
    line1->setStyleSheet("QLabel{background-color:transparent;border:1px;color:black;}");
    line2->setStyleSheet("QLabel{background-color:transparent;}");

    reduce->setStyleSheet("QPushButton{background-image: url(:/res/res/reduce.png);}"
                          "QPushButton::hover{background-image: url(:/res/res/reduce_hover.png);}"
                          "QPushButton::pressed{background-image: url(:/res/res/reduce_hover.png);}");

    enlarge->setStyleSheet("QPushButton{background-image: url(:/res/res/enlarge.png);}"
                           "QPushButton::hover{background-image: url(:/res/res/enlarge_hover.png);}"
                           "QPushButton::pressed{background-image: url(:/res/res/enlarge_hover.png);}");

    originalSize->setStyleSheet("QPushButton{background-image: url(:/res/res/originalSize.png);}"
                                "QPushButton::hover{background-image: url(:/res/res/originalSize_hover.png);}"
                                "QPushButton::pressed{background-image: url(:/res/res/originalSize_hover.png);}");

    adaptiveWidget->setStyleSheet("QPushButton{background-image: url(:/res/res/adaptiveWidget.png);}"
                                  "QPushButton::hover{background-image: url(:/res/res/adaptiveWidget_hover.png);}"
                                  "QPushButton::pressed{background-image: url(:/res/res/adaptiveWidget_hover.png);}");

    rotate->setStyleSheet("QPushButton{background-image: url(:/res/res/rotate.png);}"
                          "QPushButton::hover{background-image: url(:/res/res/rotate_hover.png);}"
                          "QPushButton::pressed{background-image: url(:/res/res/rotate_hover.png);}");

    flipH->setStyleSheet("QPushButton{background-image: url(:/res/res/flipH.png);}"
                         "QPushButton::hover{background-image: url(:/res/res/flipH_hover.png);}"
                         "QPushButton::pressed{background-image: url(:/res/res/flipH_hover.png);}");

    flipV->setStyleSheet("QPushButton{background-image: url(:/res/res/flipV.png);}"
                         "QPushButton::hover{background-image: url(:/res/res/flipV_hover.png);}"
                         "QPushButton::pressed{background-image: url(:/res/res/flipV_hover.png);}");

    cutImage->setStyleSheet("QPushButton{background-image: url(:/res/res/cutImage.png);}"
                            "QPushButton::hover{background-image: url(:/res/res/cutImage_hover.png);}"
                            "QPushButton::pressed{background-image: url(:/res/res/cutImage_hover.png);}");

    filter->setStyleSheet("QPushButton{background-image: url(:/res/res/filter.png);}"
                          "QPushButton::hover{background-image: url(:/res/res/filter_hover.png);}"
                          "QPushButton::pressed{background-image: url(:/res/res/filter_hover.png);}");

    labelbar->setStyleSheet("QPushButton{background-image: url(:/res/res/labelbar.png);}"
                            "QPushButton::hover{background-image: url(:/res/res/labelbar_hover.png);}"
                            "QPushButton::pressed{background-image: url(:/res/res/labelbar_hover.png);}");

    sidebar->setStyleSheet("QPushButton{background-image: url(:/res/res/sidebar.png);}"
                           "QPushButton::hover{background-image: url(:/res/res/sidebar_hover.png);}"
                           "QPushButton::pressed{background-image: url(:/res/res/sidebar_hover.png);}");

    information->setStyleSheet("QPushButton{background-image: url(:/res/res/information.png);}"
                               "QPushButton::hover{background-image: url(:/res/res/information_hover.png);}"
                               "QPushButton::pressed{background-image: url(:/res/res/information_hover.png);}");

    delImage->setStyleSheet("QPushButton{background-image: url(:/res/res/delImage.png);}"
                            "QPushButton::hover{background-image: url(:/res/res/delImage_hover.png);}"
                            "QPushButton::pressed{background-image: url(:/res/res/delImage_hover.png);}");


}
void ToolBar::_initConnect()
{

    connect(reduce,&QPushButton::clicked,this,&ToolBar::_reduceImage);
    connect(enlarge,&QPushButton::clicked,this,&ToolBar::_enlargeImage);
    connect(rotate,&QPushButton::clicked,this,&ToolBar::_rotate);
    connect(originalSize,&QPushButton::clicked,this,&ToolBar::_originalSize);
    connect(adaptiveWidget,&QPushButton::clicked,this,&ToolBar::_adaptiveWidget);
    connect(flipH,&QPushButton::clicked,this,&ToolBar::_flipH);
    connect(flipV,&QPushButton::clicked,this,&ToolBar::_flipV);
    connect(cutImage,&QPushButton::clicked,this,&ToolBar::_cutImage);
    connect(filter,&QPushButton::clicked,this,&ToolBar::_filter);
    connect(labelbar,&QPushButton::clicked,this,&ToolBar::_labelbar);
    connect(sidebar,&QPushButton::clicked,this,&ToolBar::_sidebar);
    connect(information,&QPushButton::clicked,this,&ToolBar::_information);
    connect(delImage,&QPushButton::clicked,this,&ToolBar::_delImage);

}
//改变百分比的数值
void ToolBar::changePerRate(QString num)
{
    this->percentage->setText(num);
}

void ToolBar::_reduceImage()
{
    interaction->watchSmallImage();
}

void ToolBar::_enlargeImage()
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
    qDebug()<<"详细信息";
}

void ToolBar::_delImage()
{
    qDebug()<<"删除";
}



void ToolBar::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(QRect(4, 4, this->width() - 2 * 4, this->height() - 2 * 4), QBrush(Qt::white));

    QColor color(50, 50, 50, 30);
    for (int i = 0; i < 4; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 40);
        painter.setPen(color);
        // 圆角阴影边框;
        painter.drawRoundedRect(4 - i, 4 - i, this->width() - (4 - i) * 2, this->height() - (4 - i) * 2, 4, 4);
    }


}




