#include <QString>
#include <QStringList>
#include "./common_global.h"

const QString SIGN_EXECUTE_NAME = "SignaturePainter";
const QString SIGN_CMD_POSTION = "changeDialog %1 %2 %3 %4";
const QString SIGN_CMD_DISPLAY = "showWindow 1";
const QString SIGN_CMD_HIDE    = "showWindow 0";
const QString SIGN_CMD_CLEAR   = "clearPainter";
const QString SIGN_CMD_SAVE    = "saveToPng";
const QString SIGN_CMD_BLANK   = " ";

class COMMONSHARED_EXPORT CSignPainterCmd{
public:
    CSignPainterCmd();
public:
    static QStringList controlCmdList();
};
