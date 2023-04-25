#include "CImage.h"
#include "CFilePath.h"
#include <QDir>
#include <opencv2/opencv.hpp>

using namespace  Common;

CImage::CImage()
{

}


bool CImage::saveImage(const string& strPath, const Mat& srcImage)
{

    // 创建路径
    size_t nFindPos = strPath.find_last_of('/');
    if(string::npos == nFindPos)
    {
        nFindPos = strPath.find_last_of('\\');
    }

    string strFilePath;
    if (string::npos == nFindPos)
    {
        strFilePath.append(".\\");
    }
    else
    {
        strFilePath = strPath.substr(0, nFindPos);
    }

    CFilePath::isDirExist(strFilePath, true);

    return imwrite(strPath, srcImage);
}

bool CImage::readImage(const string& strPath, Mat& outImage)
{
    outImage.release();
    outImage = imread(strPath);
    if(outImage.empty())
    {
        return false;
    }
    return true;
}
