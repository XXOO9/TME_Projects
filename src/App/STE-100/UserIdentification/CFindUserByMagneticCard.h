#ifndef CFindUserByMagneticCard_H
#define CFindUserByMagneticCard_H
#include "CFindUserBasic.h"

//磁卡查询线程,为了不影响
class CFindUserByMagneticCard : public CFindUserBasic
{
    Q_OBJECT
public:
    CFindUserByMagneticCard();

    //设置查找资源,在每个查找当中需要核对查找信息的准确性，每个类需要进行继承重写
    virtual void setFindUserInfo(const CUserInfo &userInfo) override;
protected:

    //本地识别
    virtual void findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes) override;
    //服务器识别
    virtual void findOnServer(CUserInfo &findCondition) override;
};

#endif // CFindUserByMagneticCard_H
