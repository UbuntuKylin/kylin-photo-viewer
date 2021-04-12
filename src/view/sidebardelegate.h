#ifndef SIDEBARDELEGATE_H
#define SIDEBARDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
class SideBarDelegate : public QStyledItemDelegate
{
public:
    SideBarDelegate();
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SIDEBARDELEGATE_H
