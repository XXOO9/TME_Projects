#ifndef CCAMERREGISTER_H
#define CCAMERREGISTER_H
#include "common_global.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

class COMMONSHARED_EXPORT CCamerRegister
{
public:
    CCamerRegister();
    virtual ~CCamerRegister();
    virtual void setCamerData(const cv::Mat &matImage);

protected:
    void dealImage(cv::Mat &mat);
};

#endif // CCAMERREGISTER_H
