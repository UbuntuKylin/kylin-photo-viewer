#include "flip.h"

Mat Flip::processing(const Mat &mat, const QVariant &args)
{
    int type = args.toInt();
    switch (type) {
    case 0:
        return _rotateRight(mat);
    case 1:
        return _rotateLeft(mat);
    case 2:
        return _flipH(mat);
    case 3:
        return _flipV(mat);
    }

    return mat;
}

Mat Flip::_rotateRight(const Mat &mat)
{
    Mat temp,dst;
    transpose(mat, temp);
    flip(temp,dst,1);
    return dst;
}

Mat Flip::_rotateLeft(const Mat &mat)
{
    Mat temp,dst;
    transpose(mat, temp);
    flip(temp,dst,-1);
    return dst;
}

Mat Flip::_flipH(const Mat &mat)
{
    Mat dst;
    flip(mat,dst,1);
    return dst;
}

Mat Flip::_flipV(const Mat &mat)
{
    Mat dst;
    flip(mat,dst,0);
    return dst;
}
