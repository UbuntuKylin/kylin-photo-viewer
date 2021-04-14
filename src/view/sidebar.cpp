#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QListView(parent)
{
//    this->resize(SIDEBAR_SIZE);
//    this->setResizeMode(QListView::Adjust);
//    this->setUpdatesEnabled(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置无水平滑动条
    this->verticalScrollBar()->setProperty("drawScrollBarGroove",false);//设置滑动条无滑槽
    this->setDragEnabled(false);//设置不可拖动
    this->setSpacing(6); //为视图设置控件间距
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可输入文字
    this->setIconSize(SIDEITEM_SIZE);//
    this->setViewMode(QListView::IconMode);

    m_itemModel = new QStandardItemModel(this);
    m_delegate = new SideBarDelegate();
    this->setItemDelegate(m_delegate);

    //图片没处理完毕时，显示转圈圈图
    m_loadingMovie = new QMovie(":/res/res/loadgif.gif");

}

void SideBar::getAlbum()
{
//    int size = this->size();

    connect(this,&SideBar::clicked,Interaction::getInstance(),&Interaction::changeImageFromClick);
    connect(Interaction::getInstance(),&Interaction::changeAlbumHighLight,this,&SideBar::setCUrrIndex);

    this->setModel(Interaction::getInstance()->getAlbumModel());
}

void SideBar::getSelect(int type)
{
    if (type >= 10) {
        this->resize(SIDEBAR_SIZE);
    } else {
        this->resize(SIDEBAR_SIZE.width(),(56 + 8 )*type + 20);
    }
    //    this->setCurrentIndex(model()->index(type,0));
}

void SideBar::setCUrrIndex(QModelIndex modelIndex)
{
    this->setCurrentIndex(modelIndex);
}

