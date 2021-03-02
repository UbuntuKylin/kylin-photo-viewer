#include "tmp.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    interaction =Interaction::getInstance();
}

Widget::~Widget()
{
}

