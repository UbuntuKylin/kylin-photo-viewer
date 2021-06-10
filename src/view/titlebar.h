#ifndef TITLEBAR_H
#define TITLEBAR_H
#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "menumodule.h"
#include "controller/interaction.h"
#include "edit.h"
class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

    QWidget *g_titleWid; //布局
    menuModule *g_menu = nullptr;
    QLabel *g_imageName;//图片名字
    Edit *g_myEdit = nullptr;
    QPushButton *g_fullscreen;//最大化/还原

    void showImageName(QString name, QString imagePath);
    void needRename(int mode);//需要重命名
private:

    QHBoxLayout *m_titleLayout;
    QPushButton *m_logoBtn;//左上角logo
//    QLabel *m_logoBtn;//左上角logo
    QLabel *m_logolb;//左上角logoname
    QPushButton *m_minibtn;//最小化按钮
    QPushButton *m_closebtn;//关闭
    QString m_imagePath;//文件路径
    QString m_oldName;//存重命名之前的名字
    void initControlQss();//初始化顶栏布局
    void initConnect();//建立信号与槽的连接
    void longText(QLabel *nameC, QString text);
    void reName();
    void showOrigName();
    void editSelected();//lineedit选中
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:

    void recovery();//处理主界面最大化和还原
    void openSignal();//标题栏夏卡菜单打开图片
    void aboutShow();//关于界面打开，两栏的展示情况
    void toInforChangeName();//更改成功，告诉信息栏改名字---需要更新更改时间。。。。
    void toCoreChangeName(QString oldName, QFileInfo newName);//后端改名字
    void updateInformation(QFileInfo newName);//更新信息栏

};

#endif // TITLEBAR_H
