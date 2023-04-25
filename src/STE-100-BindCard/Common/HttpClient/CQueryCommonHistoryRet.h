#ifndef CQUERYCOMMONHISTORYRET_H
#define CQUERYCOMMONHISTORYRET_H

#include <QObject>
#include "../common_global.h"
#include "CAbstractResult.h"

class COMMONSHARED_EXPORT CQueryCommonHistoryRet : public CAbstractResult
{
public:
    CQueryCommonHistoryRet();
    virtual ~CQueryCommonHistoryRet() override;
};

#endif // CQUERYCOMMONHISTORYRET_H
