#include "CAlgorithm.h"
#include "math.h"

float CAlgorithm::integralTwoPt(float a, float b, float fa, float fb, float eps, bool bUseAbs)
{
    int n = 1;
        double h, k, tn, tn1, fh, fh1;

        //两点看做直线计算两点公式
        float fK = (fb - fa) / (b - a);
        float fB = fa - fK * a;

        tn = static_cast<double>((b - a) * (fa + fb) / 2);

        //判断两点值，分别处理正波和负波
        if(fa * fb < 0)
        {
            //求出零点所在位置
            float x1;
            x1 = -fB / fK;

            float value1 = integralTwoPt(a, x1, fa, 0.0, eps);
            float value2 = integralTwoPt(x1, b, 0.0, fb, eps);

            return value1 + value2;
        }
        else
        {
            do{
                for(k = 0, fh1 = 0; k < n; k++)
                {
                    h = static_cast<double>((b - a) / n);
                    fh = fK * (a + (2 * k + 1)*(b - a)/(2 * n)) + fB;
                    fh = fh + fh1;
                    fh1 = fh;
                }
                tn1 = tn;
                tn = (tn1 + fh * h) / 2;
                n = 2 * n;

            }while(fabs(tn - tn1) >= static_cast<double>(eps));

            if(bUseAbs)
            {
                return static_cast<float>(fabs(tn));
            }
            else
            {
                return static_cast<float>(tn);
            }

        }
}
