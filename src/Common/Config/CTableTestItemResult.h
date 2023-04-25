#ifndef TESTITEMRESULT_H
#define TESTITEMRESULT_H
#include "CAbstractConfigInfo.h"
#include "CTestResultInfo.h"
using namespace Database;

class COMMONSHARED_EXPORT CTableTestItemResult: public CAbstractConfigInfo
{
public:
    CTableTestItemResult();
    ~CTableTestItemResult() override;

    virtual bool Check_Config_Table() override;

public:
    bool addResultInfo(const CTestResultInfo &info);
    bool updateResultInfo(const CTestResultInfo &info);
    bool addResultInfo(const vector<CTestResultInfo> &vecResult);
    bool getResultByInfo(const CTestResultInfo &info, vector<CTestResultInfo> &vecResult, bool bGetAll = false);
    bool getResultByInfo(vector<CTestResultInfo> &vecResult, const long long &llUserID, const int &iItem, const long long &llTime, const int &iNature);
    bool getResultByInfo(const CTestResultInfo &info, vector<TestItemResult> &vecResult, bool bGetAll = false);
    bool deleteResultInfo(const CTestResultInfo &info, bool bDeleteAll = false);
    bool deleteResultInfo(const vector<CTestResultInfo> &vecResult);

    bool updateSync(const long long &llID, const int &iItem, const string &strTime, bool bFlag);

    static void equal(TestItemResult &targeInfo, const CTestResultInfo &srcInfo);
    static void equal(CTestResultInfo &targeInfo, const TestItemResult &srcInfo);

    //生成当前信息的条件
    bool gendrateQueryCriteria(const CTestResultInfo &Info, string &query);
private:
    //生成单个查询条件
    static bool getQurty(short shType, const CTestResultInfo &Info, string &query);

};

#endif // TESTITEMRESULT_H
