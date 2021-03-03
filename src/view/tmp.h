#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QStandardItemModel>
#include "../controller/interaction.h"
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
    QLabel *lable = nullptr;
};
#endif // WIDGET_H
