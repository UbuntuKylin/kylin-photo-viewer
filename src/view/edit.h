#ifndef EDIT_H
#define EDIT_H
#include <QLineEdit>
#include <QKeyEvent>
class Edit : public QLineEdit
{
    Q_OBJECT
public:
    explicit Edit(QWidget *parent = 0);
public:
    QString Text;
private:

    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:
    void renamefinished();
    void showOrigName();
};

#endif // BUTTON_H
