#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "../controller/interaction.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Interaction *interaction = nullptr;
};
#endif // WIDGET_H
