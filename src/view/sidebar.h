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
    void openEmptyFile(bool changeHigh);//判断是不是打开了图片，之后设置相册高亮

private:

    QMovie *m_loadingMovie = nullptr;//没有处理完，则显示此动图
    SideBarDelegate * m_delegate = nullptr;
    QModelIndex m_modelIndexOld;//记录上一次的index

    void initConnect();

signals:

private slots:
    void setCUrrIndex(QModelIndex modelIndex);
};

#endif // SIDEBAR_H
