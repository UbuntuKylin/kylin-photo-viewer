#ifndef FLIP_H
#define FLIP_H

#include "_processing.h"

class Flip : public _Processing
{

public:
    Mat processing(const Mat &mat , const QVariant &args);//旋转

private:
    Mat _rotateRight(const Mat &mat);//顺时针旋转
    Mat _rotateLeft(const Mat &mat);//顺时针旋转
    Mat _flipH(const Mat &mat);//水平翻转
    Mat _flipV(const Mat &mat);//垂直翻转

};

#endif // FLIP_H
