#include "base.h"

ImageUrlAndTypeList::ImageUrlAndTypeList()
{
    m_list.clear();
}

QList<int> ImageUrlAndTypeList::keys()
{
    QList<int> result;
    for (Pair &pair: m_list) {
        result.append(pair.first);
    }
    return result;
}

void ImageUrlAndTypeList::remove(const int &type)
{
    for (Pair &pair: m_list) {
        if (pair.first == type) {
            m_list.removeOne(pair);
            return;
        }
    }
}

Enums::ChamgeImageType ImageUrlAndTypeList::nextOrBack(const QString &oldPath, const QString &newPath)
{
    for (Pair &pair: m_list) {
        if (pair.second == oldPath) {
            return NEXT_IMAGE;
        }
        if (pair.second == newPath) {
            return BACK_IMAGE;
        }
    }
    qDebug()<<"判断切换状态错误";
    return NEXT_IMAGE;
}

int ImageUrlAndTypeList::backKey(const int &key)
{
    if (m_list.isEmpty()) {
        return -1;
    }

    //第一张切上一张时，回到队列结尾
    if (m_list.first().first == key) {
        return m_list.last().first;
    }

    int tmp = -1;
    for (Pair &pair: m_list) {
        if (pair.first == key) {
            return tmp;
        }
        tmp = pair.first;
    }
    return -1;
}

int ImageUrlAndTypeList::nextKey(const int &key)
{
    if (m_list.isEmpty()) {
        return -1;
    }

    //最后一张切下一张时，回到队列开头
    if (m_list.last().first == key) {
        return m_list.first().first;
    }

    bool tmp = false;
    for (Pair &pair: m_list) {
        if (tmp) {
            return pair.first;
        }
        if (pair.first == key) {
            tmp = true;
        }
    }
    return -1;
}

QString ImageUrlAndTypeList::getPath(const int &key)
{
    for (Pair &pair: m_list) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    return "";
}

bool ImageUrlAndTypeList::isEmpty()
{
    return m_list.isEmpty();
}

void ImageUrlAndTypeList::clear()
{
    m_list.clear();
}

int ImageUrlAndTypeList::length()
{
    return m_list.length();
}

void ImageUrlAndTypeList::append(const int &key, const QString &path)
{
    m_list.append(Pair(key,path));
}

void ImageUrlAndTypeList::append(const ImageUrlAndTypeList &list)
{
    QList<QPair<int,QString>> tmplist = list.m_list;
    m_list.swap(tmplist);
    m_list.append(tmplist);
}
