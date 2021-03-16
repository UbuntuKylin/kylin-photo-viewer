#ifndef _PROCESSING_H
#define _PROCESSING_H

#include "global/variable.h"
using namespace cv;

class _Processing
{
public:
    virtual Mat processing(const Mat &mat , const QVariant &args) = 0;//旋转
};

#endif // _PROCESSING_H
