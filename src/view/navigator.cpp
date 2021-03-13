#include "navigator.h"
#include "kyview.h"
Navigator::Navigator(QWidget *parent) : QWidget(parent)
{
    this->resize(Variable::NAVIGATION_SIZE);
    this->setStyleSheet("QWidget{background-color:rgba(0,0,0,0.4);}");
    bottomImage = new QLabel(this);
    bottomImage->setAlignment(Qt::AlignCenter);
    bottomImage->resize(this->width(),this->height());
    bottomImage->move(0,0);
    this->setMouseTracking(true);
    interaction = Interaction::getInstance();
    //此处绑定信号和槽
    connect(interaction,&Interaction::showNavigation,this,&Navigator::_showNavigation);//启动时打开图片
    connect(this,&Navigator::posChange,interaction,&Interaction::clickNavigation);

}

void Navigator::_showNavigation(QPixmap pix)
{
    if(pix.isNull()){
        this->hide();
        return;
    }
    bottomImage->setPixmap(pix);
    if(this->isHidden()){
        this->show();
        emit naviChange();
    }
}

void Navigator::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currpos =this->mapFromGlobal(QCursor().pos());
    emit posChange(currpos);
}

void Navigator::mousePressEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QPoint currpos =this->mapFromGlobal(QCursor().pos());
        emit posChange(currpos);
    }
}
