#include "base.h"

ImageUrlList::ImageUrlList()
{
    _list.clear();
}

QList<int> ImageUrlList::keys()
{
    QList<int> result;
    for(Pair &pair: _list)
        result.append(pair.first);
    return result;
}

void ImageUrlList::remove(const int &type)
{
    for(Pair &pair: _list)
        if(pair.first == type){
            _list.removeOne(pair);
            return;
        }
}

Enums::ChamgeImageType ImageUrlList::nextOrBack(const QString &oldPath, const QString &newPath)
{
    for(Pair &pair: _list){
        if(pair.second == oldPath)
            return nextImage;
        if(pair.second == newPath)
           return backImage;
    }
    qDebug()<<"判断切换状态错误";
    return nextImage;
}

int ImageUrlList::backKey(const int &key)
{
    if(_list.isEmpty())
        return -1;

    //第一张切上一张时，回到队列结尾
    if(_list.first().first == key)
        return _list.last().first;

    int tmp = -1;
    for(Pair &pair: _list){
        if(pair.first == key)
            return tmp;
        tmp = pair.first;
    }
    return -1;
}

int ImageUrlList::nextKey(const int &key)
{
    if(_list.isEmpty())
        return -1;

    //最后一张切下一张时，回到队列开头
    if(_list.last().first == key)
        return _list.first().first;

    bool tmp = false;
    for(Pair &pair: _list){
        if(tmp)
            return pair.first;
        if(pair.first == key)
            tmp = true;
    }
    return -1;
}

QString ImageUrlList::getPath(const int &key)
{
    for(Pair &pair: _list){
        if(pair.first == key)
            return pair.second;
    }
    return "";
}

bool ImageUrlList::isEmpty()
{
    return _list.isEmpty();
}

void ImageUrlList::clear()
{
    _list.clear();
}

int ImageUrlList::length()
{
    return _list.length();
}

void ImageUrlList::append(const int &key, const QString &path)
{
    _list.append(Pair(key,path));
}

void ImageUrlList::append(const ImageUrlList &list)
{
    QList<QPair<int,QString>> tmplist = list._list;
    _list.swap(tmplist);
    _list.append(tmplist);
}
