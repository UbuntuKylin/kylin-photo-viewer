#include "tmp.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(500,500);

    label = new QLabel;
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);



    _initInteraction();//一定要放到构造函数末尾
}

Widget::~Widget()
{
}

void Widget::_initInteraction()
{
    interaction =Interaction::getInstance();
    //此处绑定信号和槽
    connect(interaction,&Interaction::startWithOpenImage,this,&Widget::_startWithOpenImage);//启动时打开图片
    connect(interaction,&Interaction::openFinish,this,&Widget::openFinish);//图片打开完成，获取数据
    connect(interaction,&Interaction::albumFinish,this,&Widget::albumFinish);//相册缩略图打开完成，获取数据

    interaction->initUiFinish();
}

void Widget::_startWithOpenImage(QString path)
{
    _openImage(path);
}

void Widget::_openImage(QString path)
{
    QVariant var= interaction->openImage(path);

    //创建相册列表
    qDebug()<<var.value<QList<int>>();
}

void Widget::openFinish(QVariant var)
{
    ImageAndInfo package =var.value<ImageAndInfo>();
    QFileInfo info = package.info;//详情信息
    QPixmap pixmap = package.image;//图片
    int type = package.type;//在队列中的标签
    int proportion = package.proportion;//比例
    label->setPixmap(pixmap);
    qDebug()<<info<<type<<proportion;
}

void Widget::albumFinish(QVariant var)
{
    ImageAndInfo package =var.value<ImageAndInfo>();
    QFileInfo info = package.info;//详情信息
    QPixmap pixmap = package.image;//图片
    int type = package.type;//在队列中的标签
    qDebug()<<pixmap<<info<<type;
}

void Widget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    interaction->changeWidgetSize(label->size());
}

