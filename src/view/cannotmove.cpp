#include "cannotmove.h"

CannotMove::CannotMove(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
}
