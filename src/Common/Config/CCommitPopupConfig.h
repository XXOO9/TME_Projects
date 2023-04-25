#ifndef CCOMMITPOPUPCONFIG_H
#define CCOMMITPOPUPCONFIG_H
#include <QObject>
#include "common_global.h"

class COMMONSHARED_EXPORT CCommitPopupConfig
{
public:
    CCommitPopupConfig();
    int     m_delBtnEnable;
    int     m_commitType;
    int     m_signatureExisted;
    int     m_autoCommitCountTime;
    int     m_userInfoPageExisted;
};

#endif // CCOMMITPOPUPCONFIG_H
