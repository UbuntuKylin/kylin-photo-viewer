#ifndef OPENIMAGE_H
#define OPENIMAGE_H
#define FITTHEMEWINDOW "org.ukui.style"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QIcon>
#include <QSize>
#include <QStandardPaths>
#include <QFileDialog>
#include <QGSettings>
#include "global/variable.h"
#include "controller/interaction.h"
class OpenImage : public QWidget
{
    Q_OBJECT
public:
    explicit OpenImage(QWidget *parent = nullptr);
private:
    Interaction *interaction = nullptr;
    QGSettings *m_pGsettingThemeData = nullptr;
    QPushButton *openInCenter;//圆形图标
    QPushButton *addFile;//“+”图标
    QLabel *openText;//下方文字
    QSize iconsize;

    void _setstyle();
    void _initconnect();
    void openimage();

    void _initGsettings();
    void _dealSystemGsettingChange();

private slots:

signals:
    void openImage(QString path);
};

#endif // OPENIMAGE_H
