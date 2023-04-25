#include "CCamerRegister.h"

CCamerRegister::CCamerRegister()
{
}

CCamerRegister::~CCamerRegister()
{

}

void CCamerRegister::setCamerData(const cv::Mat &matImage)
{
    if(matImage.empty())
        return;
}

void CCamerRegister::dealImage(cv::Mat &mat)
{
    transpose(mat, mat);

//    int nWidth(mat.cols), nHeight(mat.rows);

//    //取位于图像右侧的人脸部分
//    mat  = mat(cv::Rect(205, 0, nWidth - 205, nHeight));

//    //恢复原始比例，宽度减小，高度也需要减小
//    int nHeightTemp = nHeight * mat.cols / nWidth;

//    //上下各减去这么宽，然后恢复原始尺寸
//    int nDis = (nHeight - nHeightTemp) / 2;
//    mat = mat(cv::Rect(0, nDis, mat.cols, nHeightTemp - 1));
//    cv::resize(mat, mat, cv::Size(nWidth, nHeight));
}
