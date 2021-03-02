#include "tmp.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    lable = new QLabel(this);
    lable->setFixedSize(500,500);


    _initInteraction();//一定要放到构造函数末尾
}

Widget::~Widget()
{
}

void Widget::_initInteraction()
{
    interaction =Interaction::getInstance();
    //此处绑定信号和槽
    connect(interaction,&Interaction::startWithOpenImage,this,&Widget::_startWithOpenImage);

    interaction->initUiFinish();
}

void Widget::_startWithOpenImage(QString path)
{
    _openImage(path);
}

void Widget::_openImage(QString path)
{
    QList<int> list= interaction->openImage(path);

    //创建相册列表
    qDebug()<<list;
}

