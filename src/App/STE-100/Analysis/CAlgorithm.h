#ifndef CALGORITHM_H
#define CALGORITHM_H

#define ULONG_ALGORITHM unsigned long

class CAlgorithm
{
public:
    //计算两点积分
    static float integralTwoPt(float a, float b, float fa, float fb, float eps = 0.1f, bool bUseAbs = false);

    template<class T>
    static bool getMaxMin( T &max, ULONG_ALGORITHM &lMaxPos, T &min, ULONG_ALGORITHM &lMinPos, ULONG_ALGORITHM lSrcNum, T *pSrcData, ULONG_ALGORITHM lStart = 0)
    {
        if(lStart >= lSrcNum)
            return false;

        T Temp;
        max = *(pSrcData + lStart);
        min	= *(pSrcData + lStart);
        lMaxPos = lStart;
        lMinPos	= lStart;

        for(ULONG_ALGORITHM i = lStart + 1; i < lSrcNum; i++)
        {
            Temp = *(pSrcData + i);
            if(Temp > max)
            {
                max = Temp;
                lMaxPos = i;
            }
            if(Temp < min)
            {
                min = Temp;
                lMinPos = i;
            }
        }

        return true;
    }
};

#endif // CALGORITHM_H
