#include "toolbar.h"
#include "xatom-helper.h"

ToolBar::ToolBar(QWidget *parent) : QDialog(parent)
{

    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(this->winId(), hints);

    this->resize(678,40);
    tooleWid = new QWidget(this);
    toolLayout = new QHBoxLayout(this);

    reduce = new QPushButton(this);
    reduce->setFixedSize(25,25);

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

    this->initControlQss();
    this->setstyle();
}
void ToolBar::initControlQss()
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
    tooleWid->setLayout(toolLayout);
    tooleWid->resize(this->width(),this->height());
    tooleWid->move(0,0);

}
void ToolBar::setstyle()
{
//    this->setObjectName("mainWid");
//    this->setStyleSheet("background:blue;");

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

//void ToolBar::enterEvent(QEvent *event)
//{
//    qDebug()<<"鼠标移入";
//    emit showTwobar();
////    QWidget::enterEvent(event);
//}
//void ToolBar::leaveEvent(QEvent *event)
//{
//    qDebug()<<"鼠标移出";
//    emit hideTwobar();
////    QWidget::leaveEvent(event);
//}
