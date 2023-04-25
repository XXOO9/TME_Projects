#ifndef CFINDUSERBYSTUDENTID_H
#define CFINDUSERBYSTUDENTID_H
#include "CFindUserBasic.h"

class CFindUserByStudentID : public CFindUserBasic
{

public:
    CFindUserByStudentID();

    //设置查找资源,在每个查找当中需要核对查找信息的准确性，每个类需要进行继承重写
    virtual void setFindUserInfo(const CUserInfo &userInfo) override;
protected:
    //本地识别
    virtual void findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes) override;
    //服务器识别
    virtual void findOnServer(CUserInfo &findCondition) override;

    //处理线程逻辑，只需重载这个变量即可
    virtual void dealThreadLogic() override;
};

#endif // CFINDUSERBYSTUDENTID_H
