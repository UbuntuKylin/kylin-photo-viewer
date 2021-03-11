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

signals:

};

#endif // NAVIGATOR_H
