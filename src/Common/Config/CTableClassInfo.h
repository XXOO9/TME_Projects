#ifndef CTABLECLASSINFO_H
#define CTABLECLASSINFO_H
#include "CAbstractConfigInfo.h"
#include "CClassInfo.h"

class COMMONSHARED_EXPORT CTableClassInfo: public CAbstractConfigInfo
{
public:
    CTableClassInfo();

    bool getClassInfo(int nUniqueID, string &strClassInfo);
    bool updateClassInfo(const CClassInfo &info);
    bool updateClassInfo(const int &nUniqueID, const string &strClassInfo, const string &strTimeStamp);
    bool updateClassInfos(const vector<CClassInfo> &vecInfo);

    bool DeleteInfo(vector<int> &vecDeleteinfo);
    bool DeleteInfoByTime(const long long &llTimeStamp);

    bool deleteAllInfo();
};

#endif // CTABLECLASSINFO_H
