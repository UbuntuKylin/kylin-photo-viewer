#ifndef OPENIMAGE_H
#define OPENIMAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QIcon>
#include <QSize>

class OpenImage : public QWidget
{
    Q_OBJECT
public:
    explicit OpenImage(QWidget *parent = nullptr);
private:
    QPushButton *openInCenter;
    QPushButton *addFile;
    QLabel *openText;
    QSize iconsize;
    void setstyle();
    void initconnect();
    void openimage();
private slots:

signals:

};

#endif // OPENIMAGE_H
