#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QStandardItemModel>
#include "src/controller/interaction.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Interaction *interaction = nullptr;
    void _initInteraction();
    void _startWithOpenImage(QString path);
    void _openImage(QString path);
    void openFinish(QVariant var);
    void albumFinish(QVariant var);
    void resizeEvent(QResizeEvent *event);
    QLabel *label = nullptr;
};
#endif // WIDGET_H
