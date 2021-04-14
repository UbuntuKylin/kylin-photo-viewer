#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include "controller/interaction.h"

class Navigator : public QWidget
{
    Q_OBJECT
public:
    explicit Navigator(QWidget *parent = nullptr);

private:
    QLabel *m_bottomImage;
    void showNavigation(QPixmap pix);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void naviChange();
    void posChange(QPoint pos);

};

#endif // NAVIGATOR_H
