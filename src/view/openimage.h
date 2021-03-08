#ifndef OPENIMAGE_H
#define OPENIMAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QIcon>
#include <QSize>
#include <QStandardPaths>
#include <QFileDialog>
#include "../global/variable.h"
#include "../controller/interaction.h"
class OpenImage : public QWidget
{
    Q_OBJECT
public:
    explicit OpenImage(QWidget *parent = nullptr);
private:
    Interaction *interaction = nullptr;

    QPushButton *openInCenter;//圆形图标
    QPushButton *addFile;//“+”图标
    QLabel *openText;//下方文字
    QSize iconsize;

    void setstyle();
    void initconnect();
    void openimage();

//    void _initInteraction();
//    void _startWithOpenImage(QString path);
//    void _openImage(QString path);
//    void openFinish(QVariant var);

private slots:

signals:
    void openImage(QString path);
};

#endif // OPENIMAGE_H
