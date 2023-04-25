#ifndef CINTERACTION_H
#define CINTERACTION_H

#include "CCamerRegister.h"
#include <QObject>
#include <QDebug>
#include <QMap>
#include <QStringList>
#include <QImage>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QMetaType>
#include <QThread>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QProcess>
#include <QCoreApplication>
#include <QQueue>
#include <QElapsedTimer>
#include <QQmlApplicationEngine>
#include "CProjectEnums.h"
#include "CImageProvider.h"
#include "CCurStudentTestRet.h"
#include "CUserInfo.h"
#include "CTestStateManage.h"
#include "commondefin.h"
#include "CHttpConnection.h"
#include "../Common/HttpClient/CHttpQueryHistoryScore.h"
#include "CCommonDataModel.h"
#include "SteTestGuide/TestMode.h"
#include "CDevDataCfg.h"
#include "common_error.h"

#include "../Common/CSingletonPattern.h"
#include "../Common/HttpClient/CHttpRequestMgr.h"
#include "../Common/CTextToSpeech.h"
#include "CDevDataCfg.h"
#include "CHightPerformanceTimeCounter.h"
#include "./CSystemSettingMgr.h"

using namespace std;

#define DEFAULT_DISP_VALUE "--"

class CInterAction : public QObject, public CCamerRegister, public CSingletonPtr<CInterAction>
{
    Q_OBJECT

    //声明为单例模式signalAdvanceReportToViewRecogRet
    DECLARE_SINGLETON_CLASS(CInterAction)
    public:
        //自检进度
      Q_PROPERTY(float processVal READ getProcessVal WRITE setProcessVal NOTIFY updateProcessVal)

      //设备类型
      Q_PROPERTY(int devType READ getIDevType WRITE setIDevType NOTIFY signalUpdateDevType)

      //测试状态
      Q_PROPERTY(QString testState READ getTestState WRITE setTestState NOTIFY updateTestState)

      //服务器连接情况
      Q_PROPERTY(bool netConnectStatus READ checkNetConnected WRITE setNetConnectState NOTIFY updateNetConnectStatus)

      //当前界面ID
      Q_PROPERTY(int curPage READ getCurPage WRITE setCurrentPage NOTIFY signalPageChanged)

      //提交成功的成绩进度
      Q_PROPERTY(int m_commitFinished READ getCommitFinished WRITE setCommitFinished NOTIFY sigCommitFinishedCountChanged)

      //需要提交的成绩总数
      Q_PROPERTY(int m_commitTotalCount READ getCommitTotalCount WRITE setCommitTotalCount NOTIFY sigCommitTotalCountChanegd )

      //提交失败的错误码
      Q_PROPERTY(QString errorString READ getSerrorString WRITE setErrorString NOTIFY sigErrorStringChanged)

      //当前识别出来的用户姓名
      Q_PROPERTY(QString curRecogUserName READ getCurUserName WRITE setCurUserName NOTIFY sigRecogNewUser )

      //待提交表格是否为空
      Q_PROPERTY(bool commitTableEmpty READ getIsCommitTableEmpty WRITE setIsCommitTableEmpty NOTIFY sigCommitTableChanged )

      //一对一测试最大结果
      Q_PROPERTY(QString otoMaxRet READ getOtoMaxRet WRITE setOtoMaxRet NOTIFY sigOtoMaxResultChanged )

      //提交进度
      Q_PROPERTY(QString commitPer READ getCommitPersent WRITE setCommitPersent NOTIFY sigCommitPersentChanged)

      //多人测试的人数'
      Q_PROPERTY(QString multiUserCount READ getMultiUserCount WRITE setMultiUserCount NOTIFY sigMultiUserCountChanged )

      //组网模式, 设备连接的数量
      Q_PROPERTY(QString multiDevCount READ getMultiDevCount WRITE setMultiDevCount NOTIFY sigMultiDevCountChanged )

      //开发者模式
      Q_PROPERTY(bool isDeveloper READ getIsDeveloper WRITE setIsDeveloper NOTIFY sigIsDeveloperChanged )

    //数据表格类型
    enum ModelType{
        ALL_DATA_MODEL = -1,
        COMMON_DATA_MODEL,  //通用数据表，界面是只有一个表格时使用，适用于直连模式所有表格，组连模式正在测试表
        STUDENT_DATA_MODEL, //组连异步模式完成测试学生表
        SCORE_DATA_MODEL,   //组连模式提交成绩表
        DEV_NUM_DATA_MODEL  //设备编号设备
    };

    //自检项
    enum SELF_CHECK_ITEM{
        NetWork = 0,            //心跳自检
        Sync_UserInfo,          //用户信息同步
        Sync_ClassInfo,         //班级信息同步
        Self_check_end,         //自检完成
    };


private:
    explicit CInterAction(QObject *parent = nullptr );

    void init();
public:
    //2022.12.10， 刷新当前设备详细信息( 设备号，分机编号 )到设置类中
    void refreshDevDetailInfo();


    /****************************2022/10/08起， 新增的函数*************************** start*/

    //向后台设置立定跳远  起跳起始位置
    Q_INVOKABLE bool setStandingJumpStartPos( int startPos );

    //向后台设置设备时组网还是直连
    Q_INVOKABLE void setDevConnectedInfo( bool directConnected, bool autoChangeConnectType );

    //界面读取当前的设备连接模式信息
    Q_INVOKABLE QJsonObject getLocalDevConnectInfo();

    //计算时间差值
    Q_INVOKABLE QString calculateTimeDifference(short operation = CProjectEnums::CALCULATETIMEDIFFERENCE );


    /****************************2022/10/08起， 新增的函数*************************** end*/

    void setMainFrame(void *pMainFrame) {m_pMainFrame = pMainFrame; }

    void setQmlEngine(QQmlApplicationEngine *engine );
    //设置提交弹窗上显示的错误原因
    void setDisplayUploadErrorString( const QString &errString );

    //获取设备类型
    Q_INVOKABLE short getIDevType() const;

    //设置设备类型
    void setIDevType(short iDevType);

    //设置测试状态
    void setTestState( QString testState );

    //获取设备状态
    QString getTestState();

    //获取当前页面
    inline short getCurPage(){return m_iCurPage;}

    //设置设备状态
    void setDevStatus( short devType , bool isDirectConnect, short testMode, bool bActualDirectConnect = true);

    //设置提交按钮可用不可用 （提交按钮根据不同的测试模式进行设置）
    void setBtnEnableCommit(const bool &bEnable);

    //初始化测试配置
    void setTestInfoSetting();

public slots:
    //设置服务器连接状态
    void setNetConnectState( bool isConnected );

public:

    //获取服务器连接情况
    bool checkNetConnected();

    //获取自检进度
    float getProcessVal();

    //设置自检进度
    void setProcessVal( float selfCheckPersent );

    //刷新实时结果
    void setCurTestRet( QString ret, bool bRefresh = true );

    //刷新身高体重数据-同时测试
    void setCurBodyTestRet(QString weight, QString height);

    //读取视频路径配置文件
    void readConfigFile();

    //向界面设置上传成绩到服务器的结果
    void setUpdateCurTestRet( bool bSuccess );

    //发送  提前告知识别结果的信号
    void advancedReportToViewRecogRet( int ret );

    //设置当前测试类型
    void setCurTestType(short testType);

    //获取当前测试类型
    short getCurTestType();

    //设备测试异常
    void setDevTestError();

    //设置过程照片
    void setTestingPicture( int index, QString filePath );

    //设备异常
    void showAbNormalPopup();

    //设置显示,底层是否存在电池, true为存在电池
    void setDisplayPowerType( bool bIsBatteryExisted );

    //设置是否显示为充电状态, true为充电,状态 充电时为绿色，未充电时为白色
    void setDisplayChargeState( bool bCharged );

    //设置显示的电池电量
    void setDisplayBattreyQuantity( int quantity );

    //向表格添加数据 modelType:表格类型
    void appendModelData(int modelType, QStringList data, bool isOrdered = true);

    //更新表格数据 columnIndex:更新依据序号（学号、设备号) columnData:更新比对内容 data:更新内容（整行）
    void updateModelRowData(int modelType, int columnIndex, QString columnData, QStringList data);

    //更新表格单元格数据  updateIndex：更新单元格列数
    void updateModelCellData(int modelType, int columnIndex, QString columnData, int updateIndex, QString data);

    //删除表格数据 columnIndex:删除依据序号（学号、设备号) columnData:删除比对内容
    void deleteModelData(int modelType, int columnIndex, QString columnData);

    //检测某列数据是否存在(limitRowIndex限制行中进行查找 >= 0表示 从 0 ~ 行索引中查找 小于0就是全部内容)
    bool checkModelDataExist(int modelType, const int &colIndex, const QString &columData, int limitRowIndex = -1);

    int getModelDataRows(int modelType);

    //清理表格数据
    void clearModelData(int modelType);

    //获取单元格数据
    QString getModelCellData(const int &modelType, int nRow, int nCol);

    //一对相关
    //更新刷卡设备有效刷卡倒计时
    void setNFCVaildTime(int count);

    //设置一对多测试过程中提示语
    void setTestTipText(QString tip);

    //是否存在可提交成绩、是否全部测试完成
    void setStuTestStatus(bool commitable, bool bTestFinished);

    //设置一对多项目同步模式计时时间
    void setSyncTestCountTime(int time);

    //响应错误提示超时
    void onCheckErrorTipTimeOut();

    //设置在界面最顶层显示的错误信息
    void setViewErorrString( int errorCode, bool display = true );

    //清除指定的错误信息
    void removeSpecificViewErrorString( int index );

    //清除所有的最顶层的界面信息
    void clearViewErrorString();

    //设置自检进度, 传入Self_check_end代表自检全部通过
    void setSelfCheckPersent( SELF_CHECK_ITEM item, bool ok = true );

    Q_INVOKABLE void testSetSelfCheck( int val );

    //向后台提供的关闭提交弹窗的接口
    void  closeCommitPopup();

    //设置一对一测试最大结果
    QString getOtoMaxRet() const;

    //设置提交测试最大值
    void setOtoMaxRet(const QString &otoMaxRet);

    //设置身高体重测量值
    void setOtoBodyValue(const QString &weight, const QString &height);

    //设置多人测试的人数
    QString getMultiUserCount() const;
    void setMultiUserCount(const QString &multiUserCount);

    //设置组网模式, 已连接的设备数量
    QString getMultiDevCount() const;
    void setMultiDevCount(const QString &multiDevCount);

    //提交进度
    QString getCommitPersent() const;
    void setCommitPersent(const QString &commitPersent);

    //设置识别成功后,显示用户姓名
    QString getCurUserName() const;
    void setCurUserName(const QString &curUserName);

    //设置待提交表格是否为空
    bool getIsCommitTableEmpty() const;
    void setIsCommitTableEmpty(bool isCommitTableEmpty);

    //通知座位体前屈测试开始
    void testStarted();

    //控制按钮属性
    void setBtnEnable(short btnType, bool bEnable);

    //打开提交弹窗
    void autoCommite();

    //获取当前签名路径
    QString getSignatureFilePath(const bool &bDefault = false) const;

    bool getIsDeveloper() const;

    void setIsDeveloper( bool ok );

    void closeRootPopup();

    //肺通气量波形添加数据点
    void appendPVVChartPoint(int chartType, float xVal, float yVal, int stageType = CProjectEnums::Expiration);

    //肺通气量采样率修改
    void setPVVSamplingRate(int iSR);

    //设置连接上的设备编号
    void setConnectedDevNumbers( const QMap<unsigned int, int> &devs);

    //获取当前同步提交按钮状态
    bool getBtnEnableState(short btnType);

    //获取当前校准的项目
    Q_INVOKABLE short getCurCorrectItem() const;

public:
    //获取页面超时时间
    Q_INVOKABLE int getPageTimeout();

    //校准身高
    Q_INVOKABLE void correctHeight();

    //校准体重
    Q_INVOKABLE void correctWidth( int weight );

    //设置当前校准的项目
    Q_INVOKABLE void setCurCorrectItem( short item );
    //获取连接上的设备编号
    Q_INVOKABLE QVariantList getConnectedDevNumbers();

    //测试代码->  多人识别
    Q_INVOKABLE void otmTestUserRecog();

    //判断是否存在之前的删除签字文件，如果有，就删除
    Q_INVOKABLE void delSignatureFile();

    //处理界面按钮点击事件
    Q_INVOKABLE void procBtnClicked(short btnType);

    Q_INVOKABLE void testFunc1();

    QMap<QString, QVariant> testFunc2(QString userId);

    Q_INVOKABLE void testFunc3();

    Q_INVOKABLE void testFunc4();

    //一对一点击提交按钮过后,立马尝试获取最大的测试结果
    Q_INVOKABLE void tryToGetMaxTestResult();

    //关闭提交弹窗(后端调用
    Q_INVOKABLE void closeConfirmPopup();

    //学生信息相关接口
    Q_INVOKABLE QString getCurStuName();
    Q_INVOKABLE QString getCurStuAge();
    Q_INVOKABLE QString getCurStuGrade();
    Q_INVOKABLE QString getCurStuClass();
    Q_INVOKABLE bool getCurStuSex();
    Q_INVOKABLE QString getCurStuSchoolNO();

    //获取软件版本号
    Q_INVOKABLE QString getSoftWareVersion();
    Q_INVOKABLE void setSoftWareVersion( QString version );

    //刷新当前测试的结果( 所有测试通用 )
    Q_INVOKABLE QString getCurTestRet();

    //清空当前数据模型
    Q_INVOKABLE void cleanDataModel();

    //输出日志
    Q_INVOKABLE void qmlLogout( QString content );

    //重置数据( 测试次数，测试成绩 )
    Q_INVOKABLE void resetTest();

    //设置当前页面
    Q_INVOKABLE void setCurrentPage(short page );

    //停止识别nfcTimer
    Q_INVOKABLE void stopNFCValidTimer();

    //取消绑定识别nfcTimer
    Q_INVOKABLE void closeBindPopup();

    //nfc刷卡timerout
    Q_INVOKABLE void nfcValidTimeOut();

    //发起请求查询成绩
    Q_INVOKABLE void queryHistoryScore( int testItem, int timeInterval, int scoreType );

    //退出软件
    Q_INVOKABLE void quit();

    //退出登录
    Q_INVOKABLE void exitLogin();

    //查找成绩下拉框改变
    Q_INVOKABLE void responseGradeTypeChange(int testItem);

    //用于 显示和影藏签字版
    Q_INVOKABLE void startSignature(QString qsShowFlag);

    //清除widget签字版
    Q_INVOKABLE void clearSignPainter();

    //保存widget签字版为png图片
    Q_INVOKABLE void saveWidgetSignPainter();

    Q_INVOKABLE void changedSignPainterPos(QString width, QString height, QString x, QString y);

    //获取中长跑长度配置
    Q_INVOKABLE int getRunMideLength();

    //切换同步异步模式
    Q_INVOKABLE void changedTestModel( short testModel );

    //设置中长跑长度
    Q_INVOKABLE bool changedRunMidLength(int iLength);

    //是否显示成绩
    Q_INVOKABLE void setIsDisplayScore(bool bDisplay);

    //获取是否显示成绩
    Q_INVOKABLE bool getIsDisplayScore();

    //获取当前信道号
    Q_INVOKABLE void getDevChannelConfig();

    //获取当前最大数量号
    Q_INVOKABLE void getDevMaxConnectCount();

    //发出重测信号
    Q_INVOKABLE void reTest();

    //配置信道 dev：设备类型 channel：信道号 DevMaxNum：最大连接数量
    Q_INVOKABLE void setChannelConfig(int channel, int devMaxNum);

    //获取信道配置
    Q_INVOKABLE QString getChannelCongigJsonStr();

    //配置分机编号
    Q_INVOKABLE void setCurDevNo(QString dev, int num);

    //删除成绩
    Q_INVOKABLE void delStudentScore(QString index);

    //取消绑定
    Q_INVOKABLE void unBindStudent(QString index);

    //是否录入学号
    Q_INVOKABLE void procAddUserID(bool bStart, QString id);

    //获取提交成功的成绩数量
    Q_INVOKABLE int getCommitFinished() const;

    //设置提交成功的成绩数量
    Q_INVOKABLE void setCommitFinished(int commitFinished);

    //获取需要提交的成绩数量
    Q_INVOKABLE int getCommitTotalCount() const;

    //设置需要提交的成绩数量
    Q_INVOKABLE void setCommitTotalCount(int commitTotalCount);

    //获取错误信息
    Q_INVOKABLE QString getSerrorString() const;

    //设置错误信息
    Q_INVOKABLE void setErrorString(const QString &serrorString);

    //获取一对一身高体重
    Q_INVOKABLE QString getOtoBodyWeight();
    Q_INVOKABLE QString getOtoBodyHeight();

    //获取指导视频的路径
    Q_INVOKABLE QString getDevVideoPath(short devType);
    Q_INVOKABLE bool checkDevVideoPathExist(QString path);

    //通过设置参数来设置弹窗类型
    Q_INVOKABLE void setCommitPopupParam(int delScoreBtnEnable, int commitType, int signatureExist, int autoCountTime , int userInfoPage);

    //捕捉扫码枪信息
    Q_INVOKABLE void catchScanCodeInfo(QString code);
	
    Q_INVOKABLE void closeCommitStatePopupAbNorlmally();

    //保存弹窗配置到数据库
    Q_INVOKABLE void saveCommitPopupParamToDataBase(int delScoreBtnEnable, int type, int signatureExist, int countTime );

    //读取数据库中的提交弹窗配置
    Q_INVOKABLE void readCommitPopupConfigFromDataBase();

    //向后台发送配置信息
    Q_INVOKABLE void changeDevConfigInfo( QString jsonString );

    //配置信道 dev：设备类型 channel：信道号 DevMaxNum：最大连接数量
    Q_INVOKABLE bool setDevTestConfig(int devType, int testTimes, int waitTimes, QString qsTestItems);

    Q_INVOKABLE int getDevTestTimes(int devType);

    Q_INVOKABLE int getDevWaitStartTime(int devType);

    Q_INVOKABLE QString getDevTestItems(int devType);

    Q_INVOKABLE int getRopeSkipRecvDelay();

    //配置短跑起点模式
    Q_INVOKABLE bool getRunShortStartFlag();
    Q_INVOKABLE void setRunshortStartFlag(bool startFlag);

    //语音播报
    Q_INVOKABLE void textToSpeech( QString content );

    //停止语音播报
    Q_INVOKABLE void stopSpeech();

    //获取所有设置设备
    Q_INVOKABLE void getAllDevToSetNum();

    //设置校准进度
    Q_INVOKABLE void setCorrectProgress(int step, unsigned int devNumber, QVariant val = QVariant( 0 ) );

    //设置分机编号状态
    Q_INVOKABLE void updateDevModelData(QString dev, QString qsVisble);

    //开始计时
    Q_INVOKABLE void startElapsedTimer();

    //停止计时
    Q_INVOKABLE void stopElapsedTimer( QString tips = "" );

    //获取系统设置的参数 ---- 获取指定的设置项
    Q_INVOKABLE QVariantMap getSystemConfigInfo( int viewPageIndex, int elementIndex = -1 );

    //设置系统设置参数
    Q_INVOKABLE void setSystemConfigInfo(int viewPageIndex, QVariantMap newConfigInfo, int elementIndex = -1 );

    //根据设备Type 的值， 获取对应的信道号列表
    Q_INVOKABLE QStringList queryCorrespondingChannelList( int devType );

signals:
    //2023.02.08  设置足球，篮球地堡相关信息(地堡配对信道)的返回值，往硬件设置信息时，等到返回时间可能过长，故采用信号槽的方式在界面上提示用户
    Q_INVOKABLE void sigRefreshBlockHouseChannelFinished();     //更新地堡信道成功，暂时默认必定成功
    Q_INVOKABLE void sigClearBlockHouseChannelFinished();       //清除地堡信道信息成功，暂时默认必定成功

    //发送身高体重成绩到界面
    Q_INVOKABLE void sigHumanCompositionAvailable( const QVariantList humanCompositionList );

    //使能设备校准界面的两个按钮
    Q_INVOKABLE void sigEnableCorrectViewBtn();

    //关闭系统根部弹窗
    Q_INVOKABLE void sigCloseRootPopup();

    //设置按钮是否可用
    Q_INVOKABLE void sigSetBtnEnable(short btnType, bool bEnable);

    //关闭提交状态弹窗(一般用于提交异常时使用，例如提交的vec是空的)
    Q_INVOKABLE void sigCloseCommitStatePopup();

    //设置提交弹窗参数
    Q_INVOKABLE void sigCommitPopupParamChanged(int delScoreBtnEnable, int commitType, int signatureExist, int autoCountTime, int userInfoPage );

    //组网模式，设备连接数量的改变
    Q_INVOKABLE void sigMultiDevCountChanged();

    //弹出提交成绩的弹窗
    Q_INVOKABLE void sigOpenCommitPopup();

    //多人测试人数改变
    Q_INVOKABLE void sigMultiUserCountChanged();

    //提交进度改变
    Q_INVOKABLE void sigCommitPersentChanged();

    //一对一最大测试结果
    Q_INVOKABLE void sigOtoMaxResultChanged();

    //设置提交表格是否为空(暂时弃用)
    Q_INVOKABLE void sigCommitTableChanged(bool isEmpty );

    //关闭提交弹窗
    Q_INVOKABLE void sigCloseConfirmPopup();

    Q_INVOKABLE void sigRecogNewUser();

    //人脸识别成功
    Q_INVOKABLE void signalIdentifyUserSuccess();

    //刷新QML摄像头界面
    Q_INVOKABLE void signalNewQimageAvailable();

    //当前测试结果数据更新
    Q_INVOKABLE void signalCurrentTestRetAvailable( QString ret );

    //刷新身高体重数据
    Q_INVOKABLE void signalCurrentBodyTestValue(QString weightVal, QString heightVal);

    //测试状态更新
    Q_INVOKABLE void sendTestStateChangeSignal();

    //向界面发送自检结果
    Q_INVOKABLE void signalReportSelfCheckRet( bool selfCheckRet );

    //通知界面更新测试图片
    Q_INVOKABLE void signalSetTestingPicture( int index, QString filePath );

    //服务器连接状态更新
    Q_INVOKABLE void updateNetConnectStatus(bool status);

    //自检进度更新
    Q_INVOKABLE void updateProcessVal( float selfCheckPersent );

    //刷卡设备nfc识别倒计时更新
    Q_INVOKABLE void updateNFCRecogTime(int time);

    //  nfc识别timer刷卡暂停更新
    Q_INVOKABLE void sigStopNFCRecogTimer();

    //向界面发送提交成绩结果
    Q_INVOKABLE void signalGetUpdateTestRet( bool bSuccess );

    //显示设备异常弹框
    Q_INVOKABLE void signalShowAbNormalPopup();

    //查询成绩下拉框改变
    Q_INVOKABLE void signalGradeTypeChange(int testItem);

    //通知界面设备异常
    Q_INVOKABLE void signalDevTestError();

    //是否测试完成
    Q_INVOKABLE void signalStuTestStatus(bool commitFlag, bool testFinishedFlag);

    //设置底层是否存在电池
    Q_INVOKABLE void signalSetPowerType( bool bIsBatteryPowerd );

    //是否显示充电状态
    Q_INVOKABLE void signalSetDisplayChargeState( bool bCharged );

    //电池电量显示
    Q_INVOKABLE void signalBatteryQuantityChanged( QString batteryQunatity );

    //设备状态更新
    Q_INVOKABLE void signalUpdateDevStatus(int devType, bool isDirectConnect, int testMode);

    //设备类型更新
    Q_INVOKABLE void signalUpdateDevType(short dev);

    //通知界面测试中提示内容更新
    Q_INVOKABLE void siganUpdateTestTip(QString qsTip);

    //测试界面计时更新
    Q_INVOKABLE void sigUpdateTestCountTime(int time);

    //测试配置结果成功
    Q_INVOKABLE void sigUpdateHWConfig(bool aws);

    //在界面最顶层显示的错误信息
    Q_INVOKABLE void sigAppendViewDisplayErrorString( QString errorString );

    //清除指定的最顶层显示的错误信息
    Q_INVOKABLE void sigRemoveSpecificErorrString( int index );

    //清除所有的错误信息
    Q_INVOKABLE void sigClearViewErorrStrings();

    //提交成功的数量发成改变
    Q_INVOKABLE void sigCommitFinishedCountChanged();

    // 总共需要提交的数量发生改变
    Q_INVOKABLE void sigCommitTotalCountChanegd();

    //测试开始
    Q_INVOKABLE void sigTestStarted();

    //肺通气量数据点绘制
    Q_INVOKABLE void sigAddPointsToPVVChart(int chartType, int stageType, float xVal, float yVal);

    //肺通气量采样率修改
    Q_INVOKABLE void sigChangePVVSampleRate(int sample);

    //向界面展示签字版的参数
    Q_INVOKABLE void sigDisplaySignpainterParam( QString info );

    //获取信道号
    Q_INVOKABLE void sigSetDevChannelInfo(int channel);

    //获取最大连接数量
    Q_INVOKABLE void sigSetDevMaxConnectNum(int num);

    //获取指定页面的信息
    Q_INVOKABLE void sigGetSpecificSettingViewInfo( int viewIndex = -1 );
signals:
    //开发者模式状态变更
    void sigIsDeveloperChanged();

    //修改设备参数
    void sigDevConfigInfoChanged( QVariantMap data );

    //测试信号
    void sigTestSignal( QString stuId );

    //尝试向后台获取测试最大值(信号不进行使用，需要删除 cl)
    void sigTryTogetMaxTestResult();

    //关闭成绩提交弹窗的信号
    void sigCloseCommitScorePopup();

    //错误信息改变的信号
    void sigErrorStringChanged();

    //点击 开始测试 按钮
    void signalClickBtnStartTest(bool bTouristMode);
    //点击 返回首页 按钮
    void signalClickBtnBackHome();
    //测试界面点击返回
    void signalClickBtnTestPageBack();
    //信息界面点击返回
    void signalClickBtnInfoPageBack();
    //提交
    void signalClickBtnSubmitResult();
    //提交确认
    void signalClickBtnVerifySubmitResult();
    //取消测试
    void signalClickBtnCancelTest();

    //界面切换
    void signalPageChanged();

    //测试状态更新
    void updateTestState(QString state);

    //中长跑测试仪nfc失效
    void sigNfcValidTimeOut();

    //查询成绩
    void signalQueryHistoryScore( int testItem, int queryInterval, int scoreType );

    //提前将人脸识别的结果反馈到QML, 为了预先在界面上显示识别结果
    void signalAdvanceReportToViewRecogRet( int ret );

    //###
    //退出
    void signalExitLogin();

    //游客模式点击首页
    void signalClickHomePage();

    //用于显示和隐藏签字版
    void signalShowSignPainterWidget( QString qsShowFlag );

    //清除widget 签字版
    void signalClearPainter();

    //保存签字版为png图片
    void signalSaveToPng( QString path );

    //改变签字版的尺寸和位置
    void sigChangeSignPainter( QString cmd );

    //同步异步模式切换的信号
    void sigTestModelChanged( short testModel );

    //中长跑长度变更
    void sigRunMidLengthChanged(int iLength);

    //发出重测信息
    void signalClickBtnReTest();

    //删除学生成绩
    void sigDelScore(QString index);

    //取消绑定
    void sigUnbindUser(QString index);

    //通知信道变更
    void sigDevChannelConfigChanged(int channel, int devMaxCount);

    void sigSetDevNumber(QString devID, int devNum);

    //录入学生ID
    void sigProcAddUserID(bool bStart, QString id);

    //通知扫码枪扫描信息
    void sigCatchScanCodeInfo(QString code);

    //一对一测试退出
    void sigOtoStopTest();

    //测试项目变更
    void sigTestItemChanged();

    //获取分机编号
    void sigGetAllDevToSetNum();

    //界面关闭绑定弹窗
    void sigCloseBindPopup();

    //校准步骤
    void sigCorrectProgress(const int &step, const unsigned int &devID, const QVariant &val);

    //获取设备信道
    void sigGetCurDevChannel();

    //获取设备连接数量
    void sigGetCurDevConnectNum();
public slots:
    //响应人脸识别成功后， 放入学生信息到当前交互类
    void onFaceRecogSuccess( const CUserInfo &userInfo );

    void onSelfCheckFinished( bool bSuccess );

protected:
    virtual void setCamerData(const cv::Mat &matImage);

private:
    //计算登录学生年级(通过入学学年判断）
    void  calculateStuGrade(int stuClass);

public:
    QString m_sCurStuName     ;
    QString m_sCurStuAge      ;
    QString m_sCurStuGrade    ;
    QString m_sCurStuSchoolNO ;
    QString m_sSoftVersion    ;
    QString m_sCurClass       ;
    bool    m_sCurStuSex      ;

    QString m_sCurYear;
    QString m_sCurMonth;
    QString m_sCurDay;

    //当前测试的结果(可实时刷新)
    QString m_sCurTestRet{ "" };

    //当前测试的分数
    QString m_sCurTestRetScore;

    //当前显示的摄像头画面
    QImage  m_imagCamerData;

    //当前体测设备类型
    QString m_sDeviceType;

    //当前测试状态
    QString m_sCurTestState {QString("请准备")};

    //当前测试类型
    short m_iCurTestType {TEST_ITEM_UNKNOW};

    //当前自检进度
    float   m_iProcessVal {0.0};

private:
    //当前页面
    short   m_iCurPage{ CProjectEnums::PAGE_UNKNOWN };

    //已经提交成功的成绩数量
    int     m_commitFinished{ 0 };

    //需要提交的成绩数量
    int     m_commitTotalCount{ 0 };

    CCommonDataModel *getDataModelByType(const int &iDataModelType);

public:
    //C++ Qimage 向 QML 输出画面的Provider
    CImageProvider  m_objImgProvider;

    //通用测试数据模型-正在测试的数据表
    CCommonDataModel m_commonDataModel;

    //学生数据表-完成测试学生信息表
    CCommonDataModel m_studentDataModel;

    //成绩表-所有学生成绩表
    CCommonDataModel m_scoreDataModel;

    //设备分机编号信息表
    CCommonDataModel m_devNumDataModel;

    //当前学生信息
    CUserInfo m_objCurStuInfo;

    //设备连接模式
    bool m_bDirectConnect {true};

    //设备测试类型
    short m_devTestMode {TEST_SYNC};

    //服务器连接状态
    bool m_bNetConnect{ false };

    //设备类型
    short m_iDevType{DEV_TYPE_UNKNOW};

    //外部设置路径，若为空使用默认路径，非空代表的路径为 m_qsMarkPath + mark.png;
    QString m_qsMarkPath{"C:/defaultSign.png"};

    QString m_qsMarkDefaultPath{"C:/defaultSign.png"};

    //查询历史成绩的http
    HttpConnection  *m_pHttp;

    //用于跑步 跳绳的定时器
    QTimer  m_objTimer;

    //定时长度
    qint32 m_iCountTime;

    CSystemSettingMgr& getSystemSettingMgr();
private:
    CHttpQueryHistoryScore  m_queryScore;
    CHttpRequestMgr        *m_pRequestMgr;
    QVector<int>            m_vecErrorCode;
    //前面保存错误码，后面保存对应的索引位置
    map<int, int>           m_mapErrCode;

    QString                 m_serrorString{ "" };

    //用于人脸识别成功后, 显示在界面当前用户的名字
    QString                 m_curUserName{"def"};

    //待提交的表格是否为空（在点击提交时，由后端控制）
    bool                    m_isCommitTableEmpty{ true };

    //一对一测试最大结果
    QString                 m_otoMaxRet{"0.0"};

    //身高体重最终值
    QString                 m_otoWeightVal{"0.0"};
    QString                 m_otoHeightVal{"0.0"};

    QString                 m_commitPersent{"-1 / -1"};

    QString                 m_multiUserCount{"0"};
    QString                 m_multiDevCount{ "0" };

    bool                    m_isDeveloper = false;
    QVariantList            m_listConnectedDevNumbers;

    //增加用于警告显示缓存队列
    struct ERRORTIP
    {
        int  iErrorCode;
        bool bDisplay;
    };

    QQueue<ERRORTIP>        m_queueErrorTip;
    QMutex                  m_mutexErrorTip;

    QTimer                  m_timerErrorTipTimeOut;
    int                     m_nCheckErrorTipTimeOutInterval{1000};
    QTime                   m_time;
    short                   m_curCorrectItem = -1;

    QMap<short, bool>       m_btnEnableMap;
    QQmlApplicationEngine  *m_pQmlengine = nullptr;

    //主框架指针
    void *m_pMainFrame{nullptr};

    //毫秒级计时器
    CHightPerformanceTimeCounter     m_countTimer;
    CSystemSettingMgr                m_systemSettingMgr;
};

#endif // CINTERACTION_H
