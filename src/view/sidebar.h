#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QGridLayout>
#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStandardItem>
#include <QDebug>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QMovie>
#include <QScrollBar>
#include "controller/interaction.h"
#include "sizedate.h"
#include "sidebardelegate.h"

class SideBar : public QListView
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    void showItem();
public slots:
    void getSelect(int type);
//    void isDelete(bool isDel);
private:

//    QListView *m_albumView= nullptr;
//    QStandardItemModel *m_itemModel;
    QMovie *m_loadingMovie = nullptr;//没有处理完，则显示此动图
    SideBarDelegate * m_delegate = nullptr;
//    bool m_isDelete = false;
    void initConnect();


signals:

private slots:
    void setCUrrIndex(QModelIndex modelIndex);
};

#endif // SIDEBAR_H
