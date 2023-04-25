#ifndef CSYNCCLASSINFO_H
#define CSYNCCLASSINFO_H

#include <QObject>
#include "CAbstractResult.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CSyncClassInfoRet : public CAbstractResult
{
public:
    CSyncClassInfoRet();
    virtual ~CSyncClassInfoRet() override;

//    void setId( QString id );
//    void setName( QString className );
//    void setYear( QString year );
//    void setSchoolId( QString schoolId );
//    void setType( QString type );
//    void setEnable( QString enable );
//    void setGraduate( QString graduate );
//    void setDescription( QString description );
//    void setUpdateTime( QString updateTime );
//    void setGrade( QString grade );
};

#endif // CSYNCCLASSINFO_H
