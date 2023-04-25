#ifndef CSYNCSCORESTDRET_H
#define CSYNCSCORESTDRET_H

#include <QObject>
#include "CAbstractResult.h"
#include "../common_global.h"

namespace syncScoreStdKeys{
          const QString grade = "grade";
          const QString testItem = "testItem";
          const QString score = "score";
          const QString gender = "gender";
          const QString resultStart = "resultStart";
          const QString resultEnd = "resultEnd";
}

class COMMONSHARED_EXPORT CSyncScoreStdRet : public CAbstractResult
{
public:
    CSyncScoreStdRet( QObject *parent = nullptr );
    virtual ~CSyncScoreStdRet() override;
};

#endif // CSYNCSCORESTDRET_H
