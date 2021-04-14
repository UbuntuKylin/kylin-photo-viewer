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
    //此处绑定信号和槽
    connect(Interaction::getInstance(),&Interaction::showNavigation,this,&Navigator::showNavigation);//启动时打开图片
    connect(this,&Navigator::posChange,Interaction::getInstance(),&Interaction::clickNavigation);

}
//显示导航器
void Navigator::showNavigation(QPixmap pix)
{
    if (pix.isNull()) {
        this->hide();
        return;
    }
    m_bottomImage->setPixmap(pix);
    if (this->isHidden()) {
        this->show();
        emit naviChange();
    }
}
//发送鼠标移动坐标
void Navigator::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currpos =this->mapFromGlobal(QCursor().pos());
    emit posChange(currpos);
}
//发送鼠标按下坐标
void Navigator::mousePressEvent(QMouseEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QPoint currpos =this->mapFromGlobal(QCursor().pos());
        emit posChange(currpos);
    }
}
