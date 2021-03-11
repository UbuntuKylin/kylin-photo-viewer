#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

class Navigator : public QWidget
{
    Q_OBJECT
public:
    explicit Navigator(QWidget *parent = nullptr);
public slots:
    void bottomwidget(QPixmap img);
private:
    QLabel *bottomImage;
    QWidget *darkWidget;



signals:

};

#endif // NAVIGATOR_H
