#ifndef CSYNCUSERINFORET_H
#define CSYNCUSERINFORET_H

#include "CAbstractResult.h"
#include "../common_global.h"
#include <QObject>

namespace SyncUserInfoKeys {
            const QString keyId =  "id" ;
            const QString keyYear =  "year" ;
            const QString keyClassId = "clazzId" ;
            const QString keyRole = "role" ;
            const QString keyCode = "code" ;
            const QString keyCardNo = "cardNo" ;
            const QString keyName = "name" ;
            const QString keyGender = "gender" ;
            const QString keyNation = "nation" ;
            const QString keyFaceFeature = "faceFeature" ;
            const QString keyDescription = "description" ;
            const QString keyAvatarUrl = "avatarUrl" ;
            const QString keyUpdateTime = "updateTime" ;
            const QString keyBirthday = "birthday" ;
}

class COMMONSHARED_EXPORT CSyncUserInfoRet : public CAbstractResult
{
public:
    CSyncUserInfoRet();
    virtual ~CSyncUserInfoRet() override;

private:
};

#endif // CSYNCUSERINFORET_H
