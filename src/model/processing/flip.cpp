#include "flip.h"

Mat Flip::processing(const Mat &mat, const QVariant &args)
{
    int type = args.toInt();
    switch (type) {
    case FlipWay::clockwise:
        return _rotateRight(mat);
    case FlipWay::counterclockwise:
        return _rotateLeft(mat);
    case FlipWay::horizontal:
        return _flipH(mat);
    case FlipWay::vertical:
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
    flip(mat,dst,0);
    return dst;
}

Mat Flip::_flipV(const Mat &mat)
{
    Mat dst;
    flip(mat,dst,1);
    return dst;
}
