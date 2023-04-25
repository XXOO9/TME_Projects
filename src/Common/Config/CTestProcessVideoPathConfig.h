#ifndef CTESTPROCESSVIDEOPATHCONFIG_H
#define CTESTPROCESSVIDEOPATHCONFIG_H

#include <QSettings>
#include <string>
#include "../common_global.h"
#include "commondefin.h"

using namespace std;

class COMMONSHARED_EXPORT CTestProcessVideoPathConfig
{
public:
    CTestProcessVideoPathConfig();

    void Init();

    QString getGudieVideoPath(short devType);

    QString getINIDevKeyWords(short devType);

private:

    void      Default(QSettings& settings);

    void      Check_Field(const QSettings& settings, const string& strField);
};
#endif // CTESTPROCESSVIDEOPATHCONFIG_H
