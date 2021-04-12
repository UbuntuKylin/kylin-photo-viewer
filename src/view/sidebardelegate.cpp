#include "sidebardelegate.h"
#include <QDebug>
SideBarDelegate::SideBarDelegate()
{

}
QSize SideBarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(104, 56);
}
