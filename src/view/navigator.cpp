#include "navigator.h"

Navigator::Navigator(QWidget *parent) : QWidget(parent)
{
    this->resize(Variable::NAVIGATION_SIZE);
    this->setStyleSheet("QWidget{background-color:rgba(0,0,0,0.4);}");
    bottomImage = new QLabel(this);
    bottomImage->setAlignment(Qt::AlignCenter);
    bottomImage->resize(this->width(),this->height());
    bottomImage->move(0,0);

    interaction = Interaction::getInstance();
    //此处绑定信号和槽
    connect(interaction,&Interaction::showNavigation,this,&Navigator::_showNavigation);//启动时打开图片

}

void Navigator::_showNavigation(QPixmap pix)
{
    if(pix.isNull()){
        this->hide();
        return;
    }
    bottomImage->setPixmap(pix);
    if(this->isHidden())
        this->show();
}

