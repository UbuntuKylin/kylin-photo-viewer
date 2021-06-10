#include "edit.h"

Edit::Edit(QWidget *parent) :
    QLineEdit(parent)
{
}
void Edit::focusOutEvent(QFocusEvent *event)
{
    clearFocus();
    close();
    emit showOrigName();
    QLineEdit::focusOutEvent(event);
}
void Edit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        Text = this->text();
        close();
        emit renamefinished();
    }
    QLineEdit::keyPressEvent(event);
}
