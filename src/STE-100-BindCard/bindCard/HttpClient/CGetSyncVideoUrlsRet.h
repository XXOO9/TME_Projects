#ifndef CGETSYNCVIDEOURLSRET_H
#define CGETSYNCVIDEOURLSRET_H

#include <QObject>
#include "CAbstractResult.h"

class CGetSyncVideoUrlsRet : public CAbstractResult
{
public:
    CGetSyncVideoUrlsRet( QObject *parent = nullptr );
    virtual ~CGetSyncVideoUrlsRet() override;
};

#endif // CGETSYNCVIDEOURLSRET_H
