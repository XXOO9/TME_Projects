#ifndef CTableTestItemInfo_H
#define CTableTestItemInfo_H
#include "CAbstractConfigInfo.h"

typedef struct Test_Item_Info{
    int nItem;
    string strItemDescribe;
    string strUnit;
    string strUnitDes;
} TEST_ITEM_INFO, *PTEST_ITEM_INFO;

class COMMONSHARED_EXPORT CTableTestItemInfo: public CAbstractConfigInfo
{
public:
    CTableTestItemInfo();
    ~CTableTestItemInfo() override;
public:
    virtual bool Check_Config_Table() override;

    //针对项目信息提供接口进行增删查改
    bool updateInfo(int nItem, string strItemDescribe, string strUnit, string strUnitDes);
    bool getInfo(int nItem, string &strItemDescribe, string &strUnit, string &strUnitDes);
    bool deleteInfo(int nItem);
};

#endif // CTableTestItemInfo_H
