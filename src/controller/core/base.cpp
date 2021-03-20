#include "base.h"

Mat ImageShowStatus::_changeImage(Mat mat)
{
    if(_backMat.data)
        _backMat.release();
    _backMat = _nowMat;
    _nowMat = mat;
    _imageSize = QString::number(_nowImage.width())+"x"+QString::number(_nowImage.height());
    switch (_nowMat.type()) {
    case CV_8UC4:
        _colorSpace = "RGBA";
        break;
    case CV_8UC3:
        _colorSpace = "RGB";
        break;
    case CV_8UC1:
        _colorSpace = "GRAY";
        break;
    }
    //gif走的是apng流程，所以这里要单独判断
    if(QFileInfo(_nowpath).suffix().toLower() == "gif")
        _colorSpace = "RGB";
    return _nowMat;
}

void ImageShowStatus::_changeImageType(int num)
{
    _matListIndex = 0;
    if(num == 0){//回滚
        _imageUrlList.remove(_nowType);
        _nowType=_backType;
        _nowpath = _backpath;
        return;
    }
    _backType = _nowType;
    _nowType = num;
    _backpath = _nowpath;
    _nowpath = _imageUrlList.getPath(num);
}

void NavigationStatus::_creatNavigation()
{
    //导航栏背景
    QSize navigationSize = Variable::NAVIGATION_SIZE;
    _navigationImage = Processing::resizePix(_nowImage,navigationSize).toImage();

    //记录空白区域
    _spaceWidth = (navigationSize.width()-_navigationImage.width())/2;
    _spaceHeight = (navigationSize.height()-_navigationImage.height())/2;

    //待显示图
    QSize pixSize = _nowImage.size() * _proportion / 100;
    _showPix = Processing::resizePix(_nowImage,pixSize);

    //高亮区域大小
    _hightlightSize.setWidth(_navigationImage.width() * _size.width() /  _showPix.width());
    _hightlightSize.setHeight(_navigationImage.height() * _size.height() /  _showPix.height());
    if(_hightlightSize.width()>_navigationImage.width())
        _hightlightSize.setWidth(_navigationImage.width());
    if(_hightlightSize.height()>_navigationImage.height())
        _hightlightSize.setHeight(_navigationImage.height());
}

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
