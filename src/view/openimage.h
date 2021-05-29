#ifndef OPENIMAGE_H
#define OPENIMAGE_H
#define FITTHEMEWINDOW "org.ukui.style"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QIcon>
#include <QSize>
#include <QFileDialog>
#include <QGSettings>
#include "global/variable.h"
#include "controller/interaction.h"
class OpenImage : public QWidget
{
    Q_OBJECT
public:
    explicit OpenImage(QWidget *parent = nullptr);
    void openImagePath();//获取并存入配置文件图片路径
private:
    QGSettings *m_pGsettingThemeData = nullptr;
    QPushButton *m_openInCenter;//圆形图标
    QPushButton *m_addFile;//“+”图标
    QLabel *m_openText;//下方文字
    QSize m_iconsize;

    void initInteraction();
    void initconnect();
    void initGsettings();
    void dealSystemGsettingChange();

private slots:

signals:
    void openImageSignal(QString path);
    void openEmptyFile(bool changHigh);
};

#endif // OPENIMAGE_H
