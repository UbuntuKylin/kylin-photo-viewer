#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QListView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置无水平滑动条
    this->verticalScrollBar()->setProperty("drawScrollBarGroove",false);//设置滑动条无滑槽
    this->setDragEnabled(false);//设置不可拖动
    this->setSpacing(8); //为视图设置控件间距
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可输入文字
    this->setIconSize(SIDEITEM_SIZE);//
    this->setViewMode(QListView::IconMode);

    m_delegate = new SideBarDelegate();
    this->setItemDelegate(m_delegate);
    this->showItem();
    //绘制阴影
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(ALBUM_COLOR); //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(ALBUM_RADIUS);        //设定阴影的模糊半径，数值越大越模糊
    this->setGraphicsEffect(effect);
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
    emit sizeChange();
}

void SideBar::openEmptyFile(bool changeHigh)
{
    if (changeHigh == true) {
        setCUrrIndex(m_modelIndexOld);
    }
}

void SideBar::initConnect()
{
//    connect(this,&SideBar::clicked,Interaction::getInstance(),&Interaction::changeImageFromClick);
    connect(Interaction::getInstance(),&Interaction::changeAlbumHighLight,this,&SideBar::setCUrrIndex);
}

void SideBar::mouseReleaseEvent(QMouseEvent *e)
{
    //处理滑动，或拖出相册时，相册选中态和主界面显示的图片保持一致
    Interaction::getInstance()->changeImageFromClick(this->currentIndex());
    QListView::mouseReleaseEvent(e);
}

void SideBar::setCUrrIndex(QModelIndex modelIndex)
{
    //当为第一张图时，回到顶部
    if (modelIndex.row() == 1) {
        this->verticalScrollBar()->setValue(0);
    }
    //存上一个，为了点击加号，但不打开时，能切换到正确的显示
    if (modelIndex.row() != 0) {
        emit changeImage(true);
        m_modelIndexOld = modelIndex;
    } else {
        emit changeImage(false);
    }
    this->setCurrentIndex(modelIndex);


}

