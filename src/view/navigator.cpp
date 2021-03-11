#include "navigator.h"

Navigator::Navigator(QWidget *parent) : QWidget(parent)
{
    this->resize(200,133);
    bottomImage = new QLabel(this);
    bottomImage->resize(this->width(),this->height());
    bottomImage->move(0,0);
    darkWidget = new QWidget(this);
    darkWidget->resize(this->width(),this->height());
    darkWidget->move(0,0);
    darkWidget->setStyleSheet("background-color:rgba(0, 0, 0, 0.45);");
    darkWidget->show();

}

void Navigator::bottomwidget(QPixmap img)
{
    QImage tempImage = img.toImage();
    qDebug()<<img.size();
    bottomImage->setPixmap(img);
}

