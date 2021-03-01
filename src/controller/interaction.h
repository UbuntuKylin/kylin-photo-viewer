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

public:
    static Interaction *getInstance();//单例取指针
    virtual void creatCore(const QStringList &list)=0;//初始化核心模块
private:
    static Interaction *m_interaction;//单例指针
};



//实际实现功能
class _Interaction : public Interaction
{
    Q_OBJECT

signals:
    void initCoreModel(const QStringList &list);//初始化核心模块

public:
    void creatCore(const QStringList &list);//初始化核心模块

private:
    bool isCoreInit = false;//防止多次初始化核心

};

#endif // INTERACTION_H
