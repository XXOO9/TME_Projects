#ifndef CGETSYNCVIDEOURLSRET_H
#define CGETSYNCVIDEOURLSRET_H

#include <QObject>
#include "CAbstractResult.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CGetSyncVideoUrlsRet : public CAbstractResult
{
public:
    CGetSyncVideoUrlsRet();
    virtual ~CGetSyncVideoUrlsRet() override;
};

#endif // CGETSYNCVIDEOURLSRET_H
