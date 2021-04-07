#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>

#include "controller/interaction.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

private:
    Interaction *m_interaction = nullptr;

signals:

};

#endif // SIDEBAR_H
