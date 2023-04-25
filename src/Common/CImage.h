#ifndef CIMAGE_H
#define CIMAGE_H

#include "common_global.h"

#include<opencv2/core/mat.hpp>
namespace  Common
{

using namespace  cv;
using namespace  std;

class COMMONSHARED_EXPORT CImage
{
public:
    CImage();

    static bool saveImage(const string& strPath, const Mat& srcImage);
    static bool readImage(const string& strPath, Mat& outImage);
};

}

#endif // CIMAGE_H
