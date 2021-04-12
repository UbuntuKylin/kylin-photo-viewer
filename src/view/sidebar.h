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
    void getAlbum();
public slots:
    void getSelect(int type);
private:

    QListView *m_albumView= nullptr;
    QStandardItemModel *m_itemModel;
    QMovie *m_loadingMovie = nullptr;//没有处理完，则显示此动图
    SideBarDelegate * m_delegate = nullptr;

signals:

};

#endif // SIDEBAR_H
