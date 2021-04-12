#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QListView(parent)
{
//    this->resize(SIDEBARSIZE);
//    this->setResizeMode(QListView::Adjust);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置无水平滑动条
    this->setDragEnabled(false);//设置不可拖动
    this->setSpacing(6); //为视图设置控件间距
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可输入文字
    this->setIconSize(SIDEITEMSIZE);//
    this->setViewMode(QListView::IconMode);

    m_itemModel = new QStandardItemModel(this);
    m_delegate = new SideBarDelegate();
    this->setItemDelegate(m_delegate);

    //图片没处理完毕时，显示转圈圈图
    m_loadingMovie = new QMovie(":/res/res/loadgif.gif");

}

void SideBar::getAlbum()
{

    connect(this,&SideBar::clicked,Interaction::getInstance(),&Interaction::changeImageFromClick);
    this->setModel(Interaction::getInstance()->getAlbumModel());
}

void SideBar::getSelect(int type)
{
    if (type >= 10) {
        this->resize(SIDEBARSIZE);
    } else {
        this->resize(SIDEBARSIZE.width(),(56 + 8 )*type + 20);
    }
//    this->setCurrentIndex(model()->index(type,0));
}

