#ifndef CCLASSINFO_H
#define CCLASSINFO_H
#include <string>

#include "../common_global.h"
using namespace std;
//班级信息
class COMMONSHARED_EXPORT CClassInfo
{
public:
    CClassInfo();

    //班级信息唯一标识符
    int m_nUniqueID;
    string m_strClassDescribe;
    string m_timeStamp;
};

#endif // CCLASSINFO_H
