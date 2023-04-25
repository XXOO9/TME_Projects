#ifndef ste100database_hpp
#define ste100database_hpp
#include "litesql.hpp"
namespace Database {
class UserIdentityInfo;
class TestItemInfo;
class TestItemResult;
class ScoreRules;
class ConfigVersion;
class DataSyncTime;
class ClassInfo;
class CameraDeviceConfig;
class DevHWInfoConfig;
class DevSWInfoConfig;
class DevTestInfoConfig;
class CommitPopupConfig;
class UserIdentityInfo : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class RoleTypeType : public litesql::FieldType {
    public:
        static const int SYSTEM_USER_UNKNOW;
        static const int SYSTEM_USER_STUDENT;
        static const int SYSTEM_USER_TEACHER;
        static const int SYSTEM_USER_ADMIN;
        RoleTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class RoleType {
    public:
        static const int SYSTEM_USER_UNKNOW;
        static const int SYSTEM_USER_STUDENT;
        static const int SYSTEM_USER_TEACHER;
        static const int SYSTEM_USER_ADMIN;
    };
    class GenderType : public litesql::FieldType {
    public:
        static const bool MALE;
        static const bool FEMALE;
        GenderType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class Gender {
    public:
        static const bool MALE;
        static const bool FEMALE;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType CodeUniqueID;
    litesql::Field<litesql::bigint> codeUniqueID;
protected:
    static std::vector < std::pair< std::string, std::string > > roleType_values;
public:
    static const UserIdentityInfo::RoleTypeType RoleType;
    litesql::Field<int> roleType;
    static const UserIdentityInfo::RoleTypeType Name;
    litesql::Field<std::string> name;
    static const UserIdentityInfo::RoleTypeType Brithday;
    litesql::Field<std::string> brithday;
protected:
    static std::vector < std::pair< std::string, std::string > > gender_values;
public:
    static const UserIdentityInfo::GenderType Gender;
    litesql::Field<bool> gender;
    static const UserIdentityInfo::GenderType StudentID;
    litesql::Field<std::string> studentID;
    static const UserIdentityInfo::GenderType IDCardNo;
    litesql::Field<std::string> iDCardNo;
    static const UserIdentityInfo::GenderType IDMagCard;
    litesql::Field<std::string> iDMagCard;
    static const UserIdentityInfo::GenderType NGrade;
    litesql::Field<int> nGrade;
    static const UserIdentityInfo::GenderType NClass;
    litesql::Field<int> nClass;
    static const UserIdentityInfo::GenderType Nation;
    litesql::Field<std::string> nation;
    static const UserIdentityInfo::GenderType FaceFeature;
    litesql::Field<std::string> faceFeature;
    static const UserIdentityInfo::GenderType FaceImagePath;
    litesql::Field<std::string> faceImagePath;
    static const UserIdentityInfo::GenderType CampusMagcard;
    litesql::Field<std::string> campusMagcard;
    static const UserIdentityInfo::GenderType TimeStamp;
    litesql::Field<std::string> timeStamp;
    static const UserIdentityInfo::GenderType Stage;
    litesql::Field<int> stage;
    static void initValues();
protected:
    void defaults();
public:
    UserIdentityInfo(const litesql::Database& db);
    UserIdentityInfo(const litesql::Database& db, const litesql::Record& rec);
    UserIdentityInfo(const UserIdentityInfo& obj);
    const UserIdentityInfo& operator=(const UserIdentityInfo& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<UserIdentityInfo> upcast() const;
    std::auto_ptr<UserIdentityInfo> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, UserIdentityInfo o);
class TestItemInfo : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType TestItem;
    litesql::Field<int> testItem;
    static const litesql::FieldType TestItemDescribe;
    litesql::Field<std::string> testItemDescribe;
    static const litesql::FieldType TestItemUnit;
    litesql::Field<std::string> testItemUnit;
    static const litesql::FieldType TestItemUntiDescribe;
    litesql::Field<std::string> testItemUntiDescribe;
protected:
    void defaults();
public:
    TestItemInfo(const litesql::Database& db);
    TestItemInfo(const litesql::Database& db, const litesql::Record& rec);
    TestItemInfo(const TestItemInfo& obj);
    const TestItemInfo& operator=(const TestItemInfo& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<TestItemInfo> upcast() const;
    std::auto_ptr<TestItemInfo> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, TestItemInfo o);
class TestItemResult : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType CodeUniqueID;
    litesql::Field<litesql::bigint> codeUniqueID;
    static const litesql::FieldType TestItem;
    litesql::Field<int> testItem;
    static const litesql::FieldType TestNature;
    litesql::Field<int> testNature;
    static const litesql::FieldType TestTime;
    litesql::Field<std::string> testTime;
    static const litesql::FieldType TestMulitResult;
    litesql::Field<std::string> testMulitResult;
    static const litesql::FieldType ProcessDataPath;
    litesql::Field<std::string> processDataPath;
    static const litesql::FieldType Confidence;
    litesql::Field<std::string> confidence;
    static const litesql::FieldType ResultSignature;
    litesql::Field<std::string> resultSignature;
    static const litesql::FieldType SyncServer;
    litesql::Field<bool> syncServer;
    static const litesql::FieldType DeviceID;
    litesql::Field<std::string> deviceID;
protected:
    void defaults();
public:
    TestItemResult(const litesql::Database& db);
    TestItemResult(const litesql::Database& db, const litesql::Record& rec);
    TestItemResult(const TestItemResult& obj);
    const TestItemResult& operator=(const TestItemResult& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<TestItemResult> upcast() const;
    std::auto_ptr<TestItemResult> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, TestItemResult o);
class ScoreRules : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType TestItem;
    litesql::Field<int> testItem;
    static const litesql::FieldType Grade;
    litesql::Field<int> grade;
    static const litesql::FieldType Gender;
    litesql::Field<bool> gender;
    static const litesql::FieldType ValueLeft;
    litesql::Field<float> valueLeft;
    static const litesql::FieldType ValueRight;
    litesql::Field<float> valueRight;
    static const litesql::FieldType Score;
    litesql::Field<float> score;
    static const litesql::FieldType AgeS;
    litesql::Field<float> ageS;
    static const litesql::FieldType AgeE;
    litesql::Field<float> ageE;
    static const litesql::FieldType Version;
    litesql::Field<int> version;
protected:
    void defaults();
public:
    ScoreRules(const litesql::Database& db);
    ScoreRules(const litesql::Database& db, const litesql::Record& rec);
    ScoreRules(const ScoreRules& obj);
    const ScoreRules& operator=(const ScoreRules& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<ScoreRules> upcast() const;
    std::auto_ptr<ScoreRules> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, ScoreRules o);
class ConfigVersion : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType CfgVersion;
    litesql::Field<std::string> cfgVersion;
protected:
    void defaults();
public:
    ConfigVersion(const litesql::Database& db);
    ConfigVersion(const litesql::Database& db, const litesql::Record& rec);
    ConfigVersion(const ConfigVersion& obj);
    const ConfigVersion& operator=(const ConfigVersion& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<ConfigVersion> upcast() const;
    std::auto_ptr<ConfigVersion> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, ConfigVersion o);
class DataSyncTime : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType TableType;
    litesql::Field<int> tableType;
    static const litesql::FieldType LastSyncTime;
    litesql::Field<std::string> lastSyncTime;
protected:
    void defaults();
public:
    DataSyncTime(const litesql::Database& db);
    DataSyncTime(const litesql::Database& db, const litesql::Record& rec);
    DataSyncTime(const DataSyncTime& obj);
    const DataSyncTime& operator=(const DataSyncTime& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<DataSyncTime> upcast() const;
    std::auto_ptr<DataSyncTime> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, DataSyncTime o);
class ClassInfo : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType ClassUniqueID;
    litesql::Field<int> classUniqueID;
    static const litesql::FieldType ClassDescribe;
    litesql::Field<std::string> classDescribe;
    static const litesql::FieldType TimeStamp;
    litesql::Field<std::string> timeStamp;
protected:
    void defaults();
public:
    ClassInfo(const litesql::Database& db);
    ClassInfo(const litesql::Database& db, const litesql::Record& rec);
    ClassInfo(const ClassInfo& obj);
    const ClassInfo& operator=(const ClassInfo& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<ClassInfo> upcast() const;
    std::auto_ptr<ClassInfo> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, ClassInfo o);
class CameraDeviceConfig : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class CamUseTypeType : public litesql::FieldType {
    public:
        static const int FaceRecognition;
        static const int PhotoGraph;
        CamUseTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class CamUseType {
    public:
        static const int FaceRecognition;
        static const int PhotoGraph;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType CamName;
    litesql::Field<std::string> camName;
protected:
    static std::vector < std::pair< std::string, std::string > > camUseType_values;
public:
    static const CameraDeviceConfig::CamUseTypeType CamUseType;
    litesql::Field<int> camUseType;
    static void initValues();
protected:
    void defaults();
public:
    CameraDeviceConfig(const litesql::Database& db);
    CameraDeviceConfig(const litesql::Database& db, const litesql::Record& rec);
    CameraDeviceConfig(const CameraDeviceConfig& obj);
    const CameraDeviceConfig& operator=(const CameraDeviceConfig& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<CameraDeviceConfig> upcast() const;
    std::auto_ptr<CameraDeviceConfig> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, CameraDeviceConfig o);
class DevHWInfoConfig : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Channel;
    litesql::Field<int> channel;
    static const litesql::FieldType MaxDevCount;
    litesql::Field<int> maxDevCount;
    static const litesql::FieldType DevNum;
    litesql::Field<int> devNum;
protected:
    void defaults();
public:
    DevHWInfoConfig(const litesql::Database& db);
    DevHWInfoConfig(const litesql::Database& db, const litesql::Record& rec);
    DevHWInfoConfig(const DevHWInfoConfig& obj);
    const DevHWInfoConfig& operator=(const DevHWInfoConfig& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<DevHWInfoConfig> upcast() const;
    std::auto_ptr<DevHWInfoConfig> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, DevHWInfoConfig o);
class DevSWInfoConfig : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType TestDev;
    litesql::Field<int> testDev;
    static const litesql::FieldType TestTimes;
    litesql::Field<int> testTimes;
    static const litesql::FieldType TestStartWaitTime;
    litesql::Field<int> testStartWaitTime;
    static const litesql::FieldType TestPageCountTime;
    litesql::Field<int> testPageCountTime;
    static const litesql::FieldType TestItem;
    litesql::Field<std::string> testItem;
protected:
    void defaults();
public:
    DevSWInfoConfig(const litesql::Database& db);
    DevSWInfoConfig(const litesql::Database& db, const litesql::Record& rec);
    DevSWInfoConfig(const DevSWInfoConfig& obj);
    const DevSWInfoConfig& operator=(const DevSWInfoConfig& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<DevSWInfoConfig> upcast() const;
    std::auto_ptr<DevSWInfoConfig> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, DevSWInfoConfig o);
class DevTestInfoConfig : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class TestModeType : public litesql::FieldType {
    public:
        static const int Sync;
        static const int Async;
        TestModeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class TestMode {
    public:
        static const int Sync;
        static const int Async;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
protected:
    static std::vector < std::pair< std::string, std::string > > testMode_values;
public:
    static const DevTestInfoConfig::TestModeType TestMode;
    litesql::Field<int> testMode;
    static const DevTestInfoConfig::TestModeType BDisplayScore;
    litesql::Field<bool> bDisplayScore;
    static const DevTestInfoConfig::TestModeType BUseRunShortStart;
    litesql::Field<bool> bUseRunShortStart;
    static const DevTestInfoConfig::TestModeType RunMidLength;
    litesql::Field<int> runMidLength;
    static const DevTestInfoConfig::TestModeType RopeDelayTime;
    litesql::Field<int> ropeDelayTime;
    static const DevTestInfoConfig::TestModeType StandingJumpStartPos;
    litesql::Field<int> standingJumpStartPos;
    static const DevTestInfoConfig::TestModeType DevConnectMode;
    litesql::Field<bool> devConnectMode;
    static const DevTestInfoConfig::TestModeType DevConnectModeAuto;
    litesql::Field<bool> devConnectModeAuto;
    static void initValues();
protected:
    void defaults();
public:
    DevTestInfoConfig(const litesql::Database& db);
    DevTestInfoConfig(const litesql::Database& db, const litesql::Record& rec);
    DevTestInfoConfig(const DevTestInfoConfig& obj);
    const DevTestInfoConfig& operator=(const DevTestInfoConfig& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<DevTestInfoConfig> upcast() const;
    std::auto_ptr<DevTestInfoConfig> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, DevTestInfoConfig o);
class CommitPopupConfig : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class DelScoreTypeType : public litesql::FieldType {
    public:
        static const int Disable;
        static const int Enable;
        DelScoreTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class DelScoreType {
    public:
        static const int Disable;
        static const int Enable;
    };
    class CommitTypeType : public litesql::FieldType {
    public:
        static const int Manual;
        static const int Auto;
        CommitTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class CommitType {
    public:
        static const int Manual;
        static const int Auto;
    };
    class SignatureExistedType : public litesql::FieldType {
    public:
        static const int NotExisted;
        static const int Existed;
        SignatureExistedType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class SignatureExisted {
    public:
        static const int NotExisted;
        static const int Existed;
    };
    class UserInfoPageExistedType : public litesql::FieldType {
    public:
        static const int Exixted;
        static const int NoExixted;
        UserInfoPageExistedType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class UserInfoPageExisted {
    public:
        static const int Exixted;
        static const int NoExixted;
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
protected:
    static std::vector < std::pair< std::string, std::string > > delScoreType_values;
public:
    static const CommitPopupConfig::DelScoreTypeType DelScoreType;
    litesql::Field<int> delScoreType;
protected:
    static std::vector < std::pair< std::string, std::string > > commitType_values;
public:
    static const CommitPopupConfig::CommitTypeType CommitType;
    litesql::Field<int> commitType;
protected:
    static std::vector < std::pair< std::string, std::string > > signatureExisted_values;
public:
    static const CommitPopupConfig::SignatureExistedType SignatureExisted;
    litesql::Field<int> signatureExisted;
    static const CommitPopupConfig::SignatureExistedType AutoCommitCountTime;
    litesql::Field<int> autoCommitCountTime;
protected:
    static std::vector < std::pair< std::string, std::string > > userInfoPageExisted_values;
public:
    static const CommitPopupConfig::UserInfoPageExistedType UserInfoPageExisted;
    litesql::Field<int> userInfoPageExisted;
    static void initValues();
protected:
    void defaults();
public:
    CommitPopupConfig(const litesql::Database& db);
    CommitPopupConfig(const litesql::Database& db, const litesql::Record& rec);
    CommitPopupConfig(const CommitPopupConfig& obj);
    const CommitPopupConfig& operator=(const CommitPopupConfig& obj);
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<CommitPopupConfig> upcast() const;
    std::auto_ptr<CommitPopupConfig> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, CommitPopupConfig o);
class STE100Database : public litesql::Database {
public:
    STE100Database(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif
