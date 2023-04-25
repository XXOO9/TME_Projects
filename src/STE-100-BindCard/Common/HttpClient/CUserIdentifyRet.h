#ifndef CUSERIDENTIFYRET_H
#define CUSERIDENTIFYRET_H

#include <QObject>
#include "CAbstractResult.h"
#include "../common_global.h"

namespace userIdentifyKeys {
          const QString id = "id";
          const QString year = "year";
          const QString clazzId = "clazzId";
          const QString role = "role";
          const QString code = "code";
          const QString cardNo = "cardNo";
          const QString name = "name";
          const QString gender = "gender";
          const QString nation = "nation";
          const QString faceFeature = "faceFeature";
          const QString description = "description";
          const QString avatarUrl = "avatarUrl";
          const QString updateTime = "updateTime";
          const QString birthday = "birthday";

}

class COMMONSHARED_EXPORT CUserIdentifyRet : public CAbstractResult
{
public:
    CUserIdentifyRet( QObject *parent = nullptr );
    virtual ~CUserIdentifyRet() override;


};

#endif // CUSERIDENTIFYRET_H
