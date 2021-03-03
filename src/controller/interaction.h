#ifndef INTERACTION_H
#define INTERACTION_H

#include <QObject>
//开辟线程运行核心
#include <QThread>
#include "core.h"

//对外暴露接口
class Interaction : public QObject
{
    Q_OBJECT
signals:
    void startWithOpenImage(QString);
    void openFinish(QVariant var);

public:
    static Interaction *getInstance();//单例取指针
    virtual void creatCore(const QStringList &list)=0;//初始化核心模块
    virtual void initUiFinish()=0;//UI初始化完成
    virtual QVariant openImage(const QString &path)=0;//打开图片，返回相册标签列表
    virtual void changeImage(const int &type)=0;//切换图片
    virtual void nextImage()=0;//切换图片
    virtual void backImage()=0;//切换图片
private:
    static Interaction *m_interaction;//单例指针
};



//实际实现功能
class _Interaction : public Interaction
{
    Q_OBJECT

signals:
    void _loadCoreModel(const QStringList &list);//初始化核心模块
    QVariant _openImage(const QString &path);//打开图片  QVariant -> QHash<int,QList<int>>
    void _changeImage(const int &type); //切换图片

public:
    virtual void creatCore(const QStringList &list);//初始化核心模块
    virtual void initUiFinish();//UI初始化完成
    virtual QVariant openImage(const QString &path);//打开图片  QVariant -> QHash<int,QList<int>>
    virtual void changeImage(const int &type);//切换图片
    virtual void nextImage();//切换图片
    virtual void backImage();//切换图片

    void needOpenImage(const QString &path);//处理启动时立即打开图片的场景

private:
    void _initConnect(Core *core);
    bool _isCoreInit = false;//防止多次初始化核心
    bool _initUiFinish = false;//UI初始化完成
    QString _needStartWithOpenImagePath = "";//UI未初始化时设置，使其初始化完成立即加载图片

};

#endif // INTERACTION_H
