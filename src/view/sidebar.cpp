#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QListView(parent)
{
//    this->resize(SIDEBAR_SIZE);
//    this->setResizeMode(QListView::Adjust);
//    this->setUpdatesEnabled(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置无水平滑动条
    this->verticalScrollBar()->setProperty("drawScrollBarGroove",false);//设置滑动条无滑槽
    this->setDragEnabled(false);//设置不可拖动
    this->setSpacing(8); //为视图设置控件间距
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可输入文字
    this->setIconSize(SIDEITEM_SIZE);//
    this->setViewMode(QListView::IconMode);

//    m_itemModel = new QStandardItemModel(this);
    m_delegate = new SideBarDelegate();
    this->setItemDelegate(m_delegate);
    this->showItem();
    //响应拖拽事件
    this->setAcceptDrops(false);
    this->setFocusPolicy(Qt::NoFocus);
    this->initConnect();

    //图片没处理完毕时，显示转圈圈图
    m_loadingMovie = new QMovie(":/res/res/loadgif.gif");//暂不需要此动图

}

void SideBar::showItem()
{

    this->setModel(Interaction::getInstance()->getAlbumModel());
}

void SideBar::getSelect(int type)
{
    if (type >= 9) {
        this->resize(SIDEBAR_SIZE);
    } else {
        this->resize(SIDEBAR_SIZE.width(),(56 +6 +8 )*type +10);
    }
}

void SideBar::initConnect()
{
    connect(this,&SideBar::clicked,Interaction::getInstance(),&Interaction::changeImageFromClick);
    connect(Interaction::getInstance(),&Interaction::changeAlbumHighLight,this,&SideBar::setCUrrIndex);
}

void SideBar::setCUrrIndex(QModelIndex modelIndex)
{

    //当为第一张图时，回到顶部
    if (modelIndex.row() == 1) {
        this->verticalScrollBar()->setValue(0);
    }
    if (modelIndex.row() == 0) {
       this->setCurrentIndex(modelIndex.model()->index(1,0));
    } else {
        this->setCurrentIndex(modelIndex);
    }

}

