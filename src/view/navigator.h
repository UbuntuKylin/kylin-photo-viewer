#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include "src/controller/interaction.h"

class Navigator : public QWidget
{
    Q_OBJECT
public:
    explicit Navigator(QWidget *parent = nullptr);

private:
    QLabel *bottomImage;
    void _showNavigation(QPixmap pix);
    Interaction * interaction =nullptr;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void naviChange();
    void posChange(QPoint pos);

};

#endif // NAVIGATOR_H
