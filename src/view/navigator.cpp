#include "navigator.h"
#include "kyview.h"
Navigator::Navigator(QWidget *parent) : QWidget(parent)
{
    this->resize(Variable::NAVIGATION_SIZE);
    this->setStyleSheet("QWidget{background-color:rgba(0,0,0,0.4);}");
    m_bottomImage = new QLabel(this);
    m_bottomImage->setAlignment(Qt::AlignCenter);
    m_bottomImage->resize(this->width(),this->height());
    m_bottomImage->move(0,0);
    this->setMouseTracking(true);
    m_interaction = Interaction::getInstance();
    //此处绑定信号和槽
    connect(m_interaction,&Interaction::showNavigation,this,&Navigator::showNavigation);//启动时打开图片
    connect(this,&Navigator::posChange,m_interaction,&Interaction::clickNavigation);

}

void Navigator::showNavigation(QPixmap pix)
{
    if(pix.isNull()){
        this->hide();
        return;
    }
    m_bottomImage->setPixmap(pix);
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
