#include "ste100database.hpp"
namespace Database {
using namespace litesql;
const litesql::FieldType UserIdentityInfo::Own::Id("id_",A_field_type_integer,"UserIdentityInfo_");
const int UserIdentityInfo::RoleTypeType::SYSTEM_USER_UNKNOW(-1);
const int UserIdentityInfo::RoleTypeType::SYSTEM_USER_STUDENT(0);
const int UserIdentityInfo::RoleTypeType::SYSTEM_USER_TEACHER(1);
const int UserIdentityInfo::RoleTypeType::SYSTEM_USER_ADMIN(2);
UserIdentityInfo::RoleTypeType::RoleTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int UserIdentityInfo::RoleType::SYSTEM_USER_UNKNOW(-1);
const int UserIdentityInfo::RoleType::SYSTEM_USER_STUDENT(0);
const int UserIdentityInfo::RoleType::SYSTEM_USER_TEACHER(1);
const int UserIdentityInfo::RoleType::SYSTEM_USER_ADMIN(2);
const bool UserIdentityInfo::GenderType::MALE(true);
const bool UserIdentityInfo::GenderType::FEMALE(false);
UserIdentityInfo::GenderType::GenderType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const bool UserIdentityInfo::Gender::MALE(true);
const bool UserIdentityInfo::Gender::FEMALE(false);
const std::string UserIdentityInfo::type__("UserIdentityInfo");
const std::string UserIdentityInfo::table__("UserIdentityInfo_");
const std::string UserIdentityInfo::sequence__("UserIdentityInfo_seq");
const litesql::FieldType UserIdentityInfo::Id("id_",A_field_type_integer,table__);
const litesql::FieldType UserIdentityInfo::Type("type_",A_field_type_string,table__);
const litesql::FieldType UserIdentityInfo::CodeUniqueID("codeUniqueID_",A_field_type_bigint,table__);
std::vector < std::pair< std::string, std::string > > UserIdentityInfo::roleType_values;
const UserIdentityInfo::RoleTypeType UserIdentityInfo::RoleType("roleType_",A_field_type_integer,table__,roleType_values);
const UserIdentityInfo::RoleTypeType UserIdentityInfo::Name("name_",A_field_type_string,table__);
const UserIdentityInfo::RoleTypeType UserIdentityInfo::Brithday("brithday_",A_field_type_string,table__);
std::vector < std::pair< std::string, std::string > > UserIdentityInfo::gender_values;
const UserIdentityInfo::GenderType UserIdentityInfo::Gender("gender_",A_field_type_boolean,table__,gender_values);
const UserIdentityInfo::GenderType UserIdentityInfo::StudentID("studentID_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::IDCardNo("iDCardNo_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::IDMagCard("iDMagCard_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::NGrade("nGrade_",A_field_type_integer,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::NClass("nClass_",A_field_type_integer,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::Nation("nation_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::FaceFeature("faceFeature_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::FaceImagePath("faceImagePath_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::CampusMagcard("campusMagcard_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::TimeStamp("timeStamp_",A_field_type_string,table__);
const UserIdentityInfo::GenderType UserIdentityInfo::Stage("stage_",A_field_type_integer,table__);
void UserIdentityInfo::initValues() {
    roleType_values.clear();
    roleType_values.push_back(make_pair<std::string, std::string>("SYSTEM_USER_UNKNOW","-1"));
    roleType_values.push_back(make_pair<std::string, std::string>("SYSTEM_USER_STUDENT","0"));
    roleType_values.push_back(make_pair<std::string, std::string>("SYSTEM_USER_TEACHER","1"));
    roleType_values.push_back(make_pair<std::string, std::string>("SYSTEM_USER_ADMIN","2"));
    gender_values.clear();
    gender_values.push_back(make_pair<std::string, std::string>("MALE","true"));
    gender_values.push_back(make_pair<std::string, std::string>("FEMALE","false"));
}
void UserIdentityInfo::defaults() {
    id = 0;
    codeUniqueID = 0;
    roleType = 0;
    gender = 0;
    nGrade = 0;
    nClass = 0;
    stage = 0;
}
UserIdentityInfo::UserIdentityInfo(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), codeUniqueID(CodeUniqueID), roleType(RoleType), name(Name), brithday(Brithday), gender(Gender), studentID(StudentID), iDCardNo(IDCardNo), iDMagCard(IDMagCard), nGrade(NGrade), nClass(NClass), nation(Nation), faceFeature(FaceFeature), faceImagePath(FaceImagePath), campusMagcard(CampusMagcard), timeStamp(TimeStamp), stage(Stage) {
    defaults();
}
UserIdentityInfo::UserIdentityInfo(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), codeUniqueID(CodeUniqueID), roleType(RoleType), name(Name), brithday(Brithday), gender(Gender), studentID(StudentID), iDCardNo(IDCardNo), iDMagCard(IDMagCard), nGrade(NGrade), nClass(NClass), nation(Nation), faceFeature(FaceFeature), faceImagePath(FaceImagePath), campusMagcard(CampusMagcard), timeStamp(TimeStamp), stage(Stage) {
    defaults();
    size_t size = (rec.size() > 18) ? 18 : rec.size();
    switch(size) {
    case 18: stage = convert<const std::string&, int>(rec[17]);
        stage.setModified(false);
    case 17: timeStamp = convert<const std::string&, std::string>(rec[16]);
        timeStamp.setModified(false);
    case 16: campusMagcard = convert<const std::string&, std::string>(rec[15]);
        campusMagcard.setModified(false);
    case 15: faceImagePath = convert<const std::string&, std::string>(rec[14]);
        faceImagePath.setModified(false);
    case 14: faceFeature = convert<const std::string&, std::string>(rec[13]);
        faceFeature.setModified(false);
    case 13: nation = convert<const std::string&, std::string>(rec[12]);
        nation.setModified(false);
    case 12: nClass = convert<const std::string&, int>(rec[11]);
        nClass.setModified(false);
    case 11: nGrade = convert<const std::string&, int>(rec[10]);
        nGrade.setModified(false);
    case 10: iDMagCard = convert<const std::string&, std::string>(rec[9]);
        iDMagCard.setModified(false);
    case 9: iDCardNo = convert<const std::string&, std::string>(rec[8]);
        iDCardNo.setModified(false);
    case 8: studentID = convert<const std::string&, std::string>(rec[7]);
        studentID.setModified(false);
    case 7: gender = convert<const std::string&, bool>(rec[6]);
        gender.setModified(false);
    case 6: brithday = convert<const std::string&, std::string>(rec[5]);
        brithday.setModified(false);
    case 5: name = convert<const std::string&, std::string>(rec[4]);
        name.setModified(false);
    case 4: roleType = convert<const std::string&, int>(rec[3]);
        roleType.setModified(false);
    case 3: codeUniqueID = convert<const std::string&, litesql::bigint>(rec[2]);
        codeUniqueID.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
UserIdentityInfo::UserIdentityInfo(const UserIdentityInfo& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), codeUniqueID(obj.codeUniqueID), roleType(obj.roleType), name(obj.name), brithday(obj.brithday), gender(obj.gender), studentID(obj.studentID), iDCardNo(obj.iDCardNo), iDMagCard(obj.iDMagCard), nGrade(obj.nGrade), nClass(obj.nClass), nation(obj.nation), faceFeature(obj.faceFeature), faceImagePath(obj.faceImagePath), campusMagcard(obj.campusMagcard), timeStamp(obj.timeStamp), stage(obj.stage) {
}
const UserIdentityInfo& UserIdentityInfo::operator=(const UserIdentityInfo& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        codeUniqueID = obj.codeUniqueID;
        roleType = obj.roleType;
        name = obj.name;
        brithday = obj.brithday;
        gender = obj.gender;
        studentID = obj.studentID;
        iDCardNo = obj.iDCardNo;
        iDMagCard = obj.iDMagCard;
        nGrade = obj.nGrade;
        nClass = obj.nClass;
        nation = obj.nation;
        faceFeature = obj.faceFeature;
        faceImagePath = obj.faceImagePath;
        campusMagcard = obj.campusMagcard;
        timeStamp = obj.timeStamp;
        stage = obj.stage;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string UserIdentityInfo::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(codeUniqueID.name());
    values.push_back(codeUniqueID);
    codeUniqueID.setModified(false);
    fields.push_back(roleType.name());
    values.push_back(roleType);
    roleType.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(brithday.name());
    values.push_back(brithday);
    brithday.setModified(false);
    fields.push_back(gender.name());
    values.push_back(gender);
    gender.setModified(false);
    fields.push_back(studentID.name());
    values.push_back(studentID);
    studentID.setModified(false);
    fields.push_back(iDCardNo.name());
    values.push_back(iDCardNo);
    iDCardNo.setModified(false);
    fields.push_back(iDMagCard.name());
    values.push_back(iDMagCard);
    iDMagCard.setModified(false);
    fields.push_back(nGrade.name());
    values.push_back(nGrade);
    nGrade.setModified(false);
    fields.push_back(nClass.name());
    values.push_back(nClass);
    nClass.setModified(false);
    fields.push_back(nation.name());
    values.push_back(nation);
    nation.setModified(false);
    fields.push_back(faceFeature.name());
    values.push_back(faceFeature);
    faceFeature.setModified(false);
    fields.push_back(faceImagePath.name());
    values.push_back(faceImagePath);
    faceImagePath.setModified(false);
    fields.push_back(campusMagcard.name());
    values.push_back(campusMagcard);
    campusMagcard.setModified(false);
    fields.push_back(timeStamp.name());
    values.push_back(timeStamp);
    timeStamp.setModified(false);
    fields.push_back(stage.name());
    values.push_back(stage);
    stage.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void UserIdentityInfo::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void UserIdentityInfo::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, codeUniqueID);
    updateField(updates, table__, roleType);
    updateField(updates, table__, name);
    updateField(updates, table__, brithday);
    updateField(updates, table__, gender);
    updateField(updates, table__, studentID);
    updateField(updates, table__, iDCardNo);
    updateField(updates, table__, iDMagCard);
    updateField(updates, table__, nGrade);
    updateField(updates, table__, nClass);
    updateField(updates, table__, nation);
    updateField(updates, table__, faceFeature);
    updateField(updates, table__, faceImagePath);
    updateField(updates, table__, campusMagcard);
    updateField(updates, table__, timeStamp);
    updateField(updates, table__, stage);
}
void UserIdentityInfo::addIDUpdates(Updates& updates) {
}
void UserIdentityInfo::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(CodeUniqueID);
    ftypes.push_back(RoleType);
    ftypes.push_back(Name);
    ftypes.push_back(Brithday);
    ftypes.push_back(Gender);
    ftypes.push_back(StudentID);
    ftypes.push_back(IDCardNo);
    ftypes.push_back(IDMagCard);
    ftypes.push_back(NGrade);
    ftypes.push_back(NClass);
    ftypes.push_back(Nation);
    ftypes.push_back(FaceFeature);
    ftypes.push_back(FaceImagePath);
    ftypes.push_back(CampusMagcard);
    ftypes.push_back(TimeStamp);
    ftypes.push_back(Stage);
}
void UserIdentityInfo::delRecord() {
    deleteFromTable(table__, id);
}
void UserIdentityInfo::delRelations() {
}
void UserIdentityInfo::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void UserIdentityInfo::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<UserIdentityInfo> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool UserIdentityInfo::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<UserIdentityInfo> UserIdentityInfo::upcast() const {
    return auto_ptr<UserIdentityInfo>(new UserIdentityInfo(*this));
}
std::auto_ptr<UserIdentityInfo> UserIdentityInfo::upcastCopy() const {
    UserIdentityInfo* np = new UserIdentityInfo(*this);
    np->id = id;
    np->type = type;
    np->codeUniqueID = codeUniqueID;
    np->roleType = roleType;
    np->name = name;
    np->brithday = brithday;
    np->gender = gender;
    np->studentID = studentID;
    np->iDCardNo = iDCardNo;
    np->iDMagCard = iDMagCard;
    np->nGrade = nGrade;
    np->nClass = nClass;
    np->nation = nation;
    np->faceFeature = faceFeature;
    np->faceImagePath = faceImagePath;
    np->campusMagcard = campusMagcard;
    np->timeStamp = timeStamp;
    np->stage = stage;
    np->inDatabase = inDatabase;
    return auto_ptr<UserIdentityInfo>(np);
}
std::ostream & operator<<(std::ostream& os, UserIdentityInfo o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.codeUniqueID.name() << " = " << o.codeUniqueID << std::endl;
    os << o.roleType.name() << " = " << o.roleType << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.brithday.name() << " = " << o.brithday << std::endl;
    os << o.gender.name() << " = " << o.gender << std::endl;
    os << o.studentID.name() << " = " << o.studentID << std::endl;
    os << o.iDCardNo.name() << " = " << o.iDCardNo << std::endl;
    os << o.iDMagCard.name() << " = " << o.iDMagCard << std::endl;
    os << o.nGrade.name() << " = " << o.nGrade << std::endl;
    os << o.nClass.name() << " = " << o.nClass << std::endl;
    os << o.nation.name() << " = " << o.nation << std::endl;
    os << o.faceFeature.name() << " = " << o.faceFeature << std::endl;
    os << o.faceImagePath.name() << " = " << o.faceImagePath << std::endl;
    os << o.campusMagcard.name() << " = " << o.campusMagcard << std::endl;
    os << o.timeStamp.name() << " = " << o.timeStamp << std::endl;
    os << o.stage.name() << " = " << o.stage << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType TestItemInfo::Own::Id("id_",A_field_type_integer,"TestItemInfo_");
const std::string TestItemInfo::type__("TestItemInfo");
const std::string TestItemInfo::table__("TestItemInfo_");
const std::string TestItemInfo::sequence__("TestItemInfo_seq");
const litesql::FieldType TestItemInfo::Id("id_",A_field_type_integer,table__);
const litesql::FieldType TestItemInfo::Type("type_",A_field_type_string,table__);
const litesql::FieldType TestItemInfo::TestItem("testItem_",A_field_type_integer,table__);
const litesql::FieldType TestItemInfo::TestItemDescribe("testItemDescribe_",A_field_type_string,table__);
const litesql::FieldType TestItemInfo::TestItemUnit("testItemUnit_",A_field_type_string,table__);
const litesql::FieldType TestItemInfo::TestItemUntiDescribe("testItemUntiDescribe_",A_field_type_string,table__);
void TestItemInfo::defaults() {
    id = 0;
    testItem = 0;
}
TestItemInfo::TestItemInfo(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), testItem(TestItem), testItemDescribe(TestItemDescribe), testItemUnit(TestItemUnit), testItemUntiDescribe(TestItemUntiDescribe) {
    defaults();
}
TestItemInfo::TestItemInfo(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), testItem(TestItem), testItemDescribe(TestItemDescribe), testItemUnit(TestItemUnit), testItemUntiDescribe(TestItemUntiDescribe) {
    defaults();
    size_t size = (rec.size() > 6) ? 6 : rec.size();
    switch(size) {
    case 6: testItemUntiDescribe = convert<const std::string&, std::string>(rec[5]);
        testItemUntiDescribe.setModified(false);
    case 5: testItemUnit = convert<const std::string&, std::string>(rec[4]);
        testItemUnit.setModified(false);
    case 4: testItemDescribe = convert<const std::string&, std::string>(rec[3]);
        testItemDescribe.setModified(false);
    case 3: testItem = convert<const std::string&, int>(rec[2]);
        testItem.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
TestItemInfo::TestItemInfo(const TestItemInfo& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), testItem(obj.testItem), testItemDescribe(obj.testItemDescribe), testItemUnit(obj.testItemUnit), testItemUntiDescribe(obj.testItemUntiDescribe) {
}
const TestItemInfo& TestItemInfo::operator=(const TestItemInfo& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        testItem = obj.testItem;
        testItemDescribe = obj.testItemDescribe;
        testItemUnit = obj.testItemUnit;
        testItemUntiDescribe = obj.testItemUntiDescribe;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string TestItemInfo::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(testItem.name());
    values.push_back(testItem);
    testItem.setModified(false);
    fields.push_back(testItemDescribe.name());
    values.push_back(testItemDescribe);
    testItemDescribe.setModified(false);
    fields.push_back(testItemUnit.name());
    values.push_back(testItemUnit);
    testItemUnit.setModified(false);
    fields.push_back(testItemUntiDescribe.name());
    values.push_back(testItemUntiDescribe);
    testItemUntiDescribe.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void TestItemInfo::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void TestItemInfo::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, testItem);
    updateField(updates, table__, testItemDescribe);
    updateField(updates, table__, testItemUnit);
    updateField(updates, table__, testItemUntiDescribe);
}
void TestItemInfo::addIDUpdates(Updates& updates) {
}
void TestItemInfo::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(TestItem);
    ftypes.push_back(TestItemDescribe);
    ftypes.push_back(TestItemUnit);
    ftypes.push_back(TestItemUntiDescribe);
}
void TestItemInfo::delRecord() {
    deleteFromTable(table__, id);
}
void TestItemInfo::delRelations() {
}
void TestItemInfo::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void TestItemInfo::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<TestItemInfo> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool TestItemInfo::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<TestItemInfo> TestItemInfo::upcast() const {
    return auto_ptr<TestItemInfo>(new TestItemInfo(*this));
}
std::auto_ptr<TestItemInfo> TestItemInfo::upcastCopy() const {
    TestItemInfo* np = new TestItemInfo(*this);
    np->id = id;
    np->type = type;
    np->testItem = testItem;
    np->testItemDescribe = testItemDescribe;
    np->testItemUnit = testItemUnit;
    np->testItemUntiDescribe = testItemUntiDescribe;
    np->inDatabase = inDatabase;
    return auto_ptr<TestItemInfo>(np);
}
std::ostream & operator<<(std::ostream& os, TestItemInfo o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.testItem.name() << " = " << o.testItem << std::endl;
    os << o.testItemDescribe.name() << " = " << o.testItemDescribe << std::endl;
    os << o.testItemUnit.name() << " = " << o.testItemUnit << std::endl;
    os << o.testItemUntiDescribe.name() << " = " << o.testItemUntiDescribe << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType TestItemResult::Own::Id("id_",A_field_type_integer,"TestItemResult_");
const std::string TestItemResult::type__("TestItemResult");
const std::string TestItemResult::table__("TestItemResult_");
const std::string TestItemResult::sequence__("TestItemResult_seq");
const litesql::FieldType TestItemResult::Id("id_",A_field_type_integer,table__);
const litesql::FieldType TestItemResult::Type("type_",A_field_type_string,table__);
const litesql::FieldType TestItemResult::CodeUniqueID("codeUniqueID_",A_field_type_bigint,table__);
const litesql::FieldType TestItemResult::TestItem("testItem_",A_field_type_integer,table__);
const litesql::FieldType TestItemResult::TestNature("testNature_",A_field_type_integer,table__);
const litesql::FieldType TestItemResult::TestTime("testTime_",A_field_type_string,table__);
const litesql::FieldType TestItemResult::TestMulitResult("testMulitResult_",A_field_type_string,table__);
const litesql::FieldType TestItemResult::ProcessDataPath("processDataPath_",A_field_type_string,table__);
const litesql::FieldType TestItemResult::Confidence("confidence_",A_field_type_string,table__);
const litesql::FieldType TestItemResult::ResultSignature("resultSignature_",A_field_type_string,table__);
const litesql::FieldType TestItemResult::SyncServer("syncServer_",A_field_type_boolean,table__);
const litesql::FieldType TestItemResult::DeviceID("deviceID_",A_field_type_string,table__);
void TestItemResult::defaults() {
    id = 0;
    codeUniqueID = 0;
    testItem = 0;
    testNature = 0;
    syncServer = false;
}
TestItemResult::TestItemResult(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), codeUniqueID(CodeUniqueID), testItem(TestItem), testNature(TestNature), testTime(TestTime), testMulitResult(TestMulitResult), processDataPath(ProcessDataPath), confidence(Confidence), resultSignature(ResultSignature), syncServer(SyncServer), deviceID(DeviceID) {
    defaults();
}
TestItemResult::TestItemResult(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), codeUniqueID(CodeUniqueID), testItem(TestItem), testNature(TestNature), testTime(TestTime), testMulitResult(TestMulitResult), processDataPath(ProcessDataPath), confidence(Confidence), resultSignature(ResultSignature), syncServer(SyncServer), deviceID(DeviceID) {
    defaults();
    size_t size = (rec.size() > 12) ? 12 : rec.size();
    switch(size) {
    case 12: deviceID = convert<const std::string&, std::string>(rec[11]);
        deviceID.setModified(false);
    case 11: syncServer = convert<const std::string&, bool>(rec[10]);
        syncServer.setModified(false);
    case 10: resultSignature = convert<const std::string&, std::string>(rec[9]);
        resultSignature.setModified(false);
    case 9: confidence = convert<const std::string&, std::string>(rec[8]);
        confidence.setModified(false);
    case 8: processDataPath = convert<const std::string&, std::string>(rec[7]);
        processDataPath.setModified(false);
    case 7: testMulitResult = convert<const std::string&, std::string>(rec[6]);
        testMulitResult.setModified(false);
    case 6: testTime = convert<const std::string&, std::string>(rec[5]);
        testTime.setModified(false);
    case 5: testNature = convert<const std::string&, int>(rec[4]);
        testNature.setModified(false);
    case 4: testItem = convert<const std::string&, int>(rec[3]);
        testItem.setModified(false);
    case 3: codeUniqueID = convert<const std::string&, litesql::bigint>(rec[2]);
        codeUniqueID.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
TestItemResult::TestItemResult(const TestItemResult& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), codeUniqueID(obj.codeUniqueID), testItem(obj.testItem), testNature(obj.testNature), testTime(obj.testTime), testMulitResult(obj.testMulitResult), processDataPath(obj.processDataPath), confidence(obj.confidence), resultSignature(obj.resultSignature), syncServer(obj.syncServer), deviceID(obj.deviceID) {
}
const TestItemResult& TestItemResult::operator=(const TestItemResult& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        codeUniqueID = obj.codeUniqueID;
        testItem = obj.testItem;
        testNature = obj.testNature;
        testTime = obj.testTime;
        testMulitResult = obj.testMulitResult;
        processDataPath = obj.processDataPath;
        confidence = obj.confidence;
        resultSignature = obj.resultSignature;
        syncServer = obj.syncServer;
        deviceID = obj.deviceID;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string TestItemResult::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(codeUniqueID.name());
    values.push_back(codeUniqueID);
    codeUniqueID.setModified(false);
    fields.push_back(testItem.name());
    values.push_back(testItem);
    testItem.setModified(false);
    fields.push_back(testNature.name());
    values.push_back(testNature);
    testNature.setModified(false);
    fields.push_back(testTime.name());
    values.push_back(testTime);
    testTime.setModified(false);
    fields.push_back(testMulitResult.name());
    values.push_back(testMulitResult);
    testMulitResult.setModified(false);
    fields.push_back(processDataPath.name());
    values.push_back(processDataPath);
    processDataPath.setModified(false);
    fields.push_back(confidence.name());
    values.push_back(confidence);
    confidence.setModified(false);
    fields.push_back(resultSignature.name());
    values.push_back(resultSignature);
    resultSignature.setModified(false);
    fields.push_back(syncServer.name());
    values.push_back(syncServer);
    syncServer.setModified(false);
    fields.push_back(deviceID.name());
    values.push_back(deviceID);
    deviceID.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void TestItemResult::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void TestItemResult::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, codeUniqueID);
    updateField(updates, table__, testItem);
    updateField(updates, table__, testNature);
    updateField(updates, table__, testTime);
    updateField(updates, table__, testMulitResult);
    updateField(updates, table__, processDataPath);
    updateField(updates, table__, confidence);
    updateField(updates, table__, resultSignature);
    updateField(updates, table__, syncServer);
    updateField(updates, table__, deviceID);
}
void TestItemResult::addIDUpdates(Updates& updates) {
}
void TestItemResult::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(CodeUniqueID);
    ftypes.push_back(TestItem);
    ftypes.push_back(TestNature);
    ftypes.push_back(TestTime);
    ftypes.push_back(TestMulitResult);
    ftypes.push_back(ProcessDataPath);
    ftypes.push_back(Confidence);
    ftypes.push_back(ResultSignature);
    ftypes.push_back(SyncServer);
    ftypes.push_back(DeviceID);
}
void TestItemResult::delRecord() {
    deleteFromTable(table__, id);
}
void TestItemResult::delRelations() {
}
void TestItemResult::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void TestItemResult::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<TestItemResult> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool TestItemResult::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<TestItemResult> TestItemResult::upcast() const {
    return auto_ptr<TestItemResult>(new TestItemResult(*this));
}
std::auto_ptr<TestItemResult> TestItemResult::upcastCopy() const {
    TestItemResult* np = new TestItemResult(*this);
    np->id = id;
    np->type = type;
    np->codeUniqueID = codeUniqueID;
    np->testItem = testItem;
    np->testNature = testNature;
    np->testTime = testTime;
    np->testMulitResult = testMulitResult;
    np->processDataPath = processDataPath;
    np->confidence = confidence;
    np->resultSignature = resultSignature;
    np->syncServer = syncServer;
    np->deviceID = deviceID;
    np->inDatabase = inDatabase;
    return auto_ptr<TestItemResult>(np);
}
std::ostream & operator<<(std::ostream& os, TestItemResult o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.codeUniqueID.name() << " = " << o.codeUniqueID << std::endl;
    os << o.testItem.name() << " = " << o.testItem << std::endl;
    os << o.testNature.name() << " = " << o.testNature << std::endl;
    os << o.testTime.name() << " = " << o.testTime << std::endl;
    os << o.testMulitResult.name() << " = " << o.testMulitResult << std::endl;
    os << o.processDataPath.name() << " = " << o.processDataPath << std::endl;
    os << o.confidence.name() << " = " << o.confidence << std::endl;
    os << o.resultSignature.name() << " = " << o.resultSignature << std::endl;
    os << o.syncServer.name() << " = " << o.syncServer << std::endl;
    os << o.deviceID.name() << " = " << o.deviceID << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ScoreRules::Own::Id("id_",A_field_type_integer,"ScoreRules_");
const std::string ScoreRules::type__("ScoreRules");
const std::string ScoreRules::table__("ScoreRules_");
const std::string ScoreRules::sequence__("ScoreRules_seq");
const litesql::FieldType ScoreRules::Id("id_",A_field_type_integer,table__);
const litesql::FieldType ScoreRules::Type("type_",A_field_type_string,table__);
const litesql::FieldType ScoreRules::TestItem("testItem_",A_field_type_integer,table__);
const litesql::FieldType ScoreRules::Grade("grade_",A_field_type_integer,table__);
const litesql::FieldType ScoreRules::Gender("gender_",A_field_type_boolean,table__);
const litesql::FieldType ScoreRules::ValueLeft("valueLeft_",A_field_type_float,table__);
const litesql::FieldType ScoreRules::ValueRight("valueRight_",A_field_type_float,table__);
const litesql::FieldType ScoreRules::Score("score_",A_field_type_float,table__);
const litesql::FieldType ScoreRules::AgeS("ageS_",A_field_type_float,table__);
const litesql::FieldType ScoreRules::AgeE("ageE_",A_field_type_float,table__);
const litesql::FieldType ScoreRules::Version("version_",A_field_type_integer,table__);
void ScoreRules::defaults() {
    id = 0;
    testItem = 0;
    grade = 0;
    gender = 0;
    valueLeft = 0.0f;
    valueRight = 0.0f;
    score = 0.0f;
    ageS = 0.0f;
    ageE = 0.0f;
    version = 1;
}
ScoreRules::ScoreRules(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), testItem(TestItem), grade(Grade), gender(Gender), valueLeft(ValueLeft), valueRight(ValueRight), score(Score), ageS(AgeS), ageE(AgeE), version(Version) {
    defaults();
}
ScoreRules::ScoreRules(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), testItem(TestItem), grade(Grade), gender(Gender), valueLeft(ValueLeft), valueRight(ValueRight), score(Score), ageS(AgeS), ageE(AgeE), version(Version) {
    defaults();
    size_t size = (rec.size() > 11) ? 11 : rec.size();
    switch(size) {
    case 11: version = convert<const std::string&, int>(rec[10]);
        version.setModified(false);
    case 10: ageE = convert<const std::string&, float>(rec[9]);
        ageE.setModified(false);
    case 9: ageS = convert<const std::string&, float>(rec[8]);
        ageS.setModified(false);
    case 8: score = convert<const std::string&, float>(rec[7]);
        score.setModified(false);
    case 7: valueRight = convert<const std::string&, float>(rec[6]);
        valueRight.setModified(false);
    case 6: valueLeft = convert<const std::string&, float>(rec[5]);
        valueLeft.setModified(false);
    case 5: gender = convert<const std::string&, bool>(rec[4]);
        gender.setModified(false);
    case 4: grade = convert<const std::string&, int>(rec[3]);
        grade.setModified(false);
    case 3: testItem = convert<const std::string&, int>(rec[2]);
        testItem.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ScoreRules::ScoreRules(const ScoreRules& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), testItem(obj.testItem), grade(obj.grade), gender(obj.gender), valueLeft(obj.valueLeft), valueRight(obj.valueRight), score(obj.score), ageS(obj.ageS), ageE(obj.ageE), version(obj.version) {
}
const ScoreRules& ScoreRules::operator=(const ScoreRules& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        testItem = obj.testItem;
        grade = obj.grade;
        gender = obj.gender;
        valueLeft = obj.valueLeft;
        valueRight = obj.valueRight;
        score = obj.score;
        ageS = obj.ageS;
        ageE = obj.ageE;
        version = obj.version;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string ScoreRules::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(testItem.name());
    values.push_back(testItem);
    testItem.setModified(false);
    fields.push_back(grade.name());
    values.push_back(grade);
    grade.setModified(false);
    fields.push_back(gender.name());
    values.push_back(gender);
    gender.setModified(false);
    fields.push_back(valueLeft.name());
    values.push_back(valueLeft);
    valueLeft.setModified(false);
    fields.push_back(valueRight.name());
    values.push_back(valueRight);
    valueRight.setModified(false);
    fields.push_back(score.name());
    values.push_back(score);
    score.setModified(false);
    fields.push_back(ageS.name());
    values.push_back(ageS);
    ageS.setModified(false);
    fields.push_back(ageE.name());
    values.push_back(ageE);
    ageE.setModified(false);
    fields.push_back(version.name());
    values.push_back(version);
    version.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ScoreRules::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ScoreRules::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, testItem);
    updateField(updates, table__, grade);
    updateField(updates, table__, gender);
    updateField(updates, table__, valueLeft);
    updateField(updates, table__, valueRight);
    updateField(updates, table__, score);
    updateField(updates, table__, ageS);
    updateField(updates, table__, ageE);
    updateField(updates, table__, version);
}
void ScoreRules::addIDUpdates(Updates& updates) {
}
void ScoreRules::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(TestItem);
    ftypes.push_back(Grade);
    ftypes.push_back(Gender);
    ftypes.push_back(ValueLeft);
    ftypes.push_back(ValueRight);
    ftypes.push_back(Score);
    ftypes.push_back(AgeS);
    ftypes.push_back(AgeE);
    ftypes.push_back(Version);
}
void ScoreRules::delRecord() {
    deleteFromTable(table__, id);
}
void ScoreRules::delRelations() {
}
void ScoreRules::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void ScoreRules::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<ScoreRules> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ScoreRules::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<ScoreRules> ScoreRules::upcast() const {
    return auto_ptr<ScoreRules>(new ScoreRules(*this));
}
std::auto_ptr<ScoreRules> ScoreRules::upcastCopy() const {
    ScoreRules* np = new ScoreRules(*this);
    np->id = id;
    np->type = type;
    np->testItem = testItem;
    np->grade = grade;
    np->gender = gender;
    np->valueLeft = valueLeft;
    np->valueRight = valueRight;
    np->score = score;
    np->ageS = ageS;
    np->ageE = ageE;
    np->version = version;
    np->inDatabase = inDatabase;
    return auto_ptr<ScoreRules>(np);
}
std::ostream & operator<<(std::ostream& os, ScoreRules o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.testItem.name() << " = " << o.testItem << std::endl;
    os << o.grade.name() << " = " << o.grade << std::endl;
    os << o.gender.name() << " = " << o.gender << std::endl;
    os << o.valueLeft.name() << " = " << o.valueLeft << std::endl;
    os << o.valueRight.name() << " = " << o.valueRight << std::endl;
    os << o.score.name() << " = " << o.score << std::endl;
    os << o.ageS.name() << " = " << o.ageS << std::endl;
    os << o.ageE.name() << " = " << o.ageE << std::endl;
    os << o.version.name() << " = " << o.version << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ConfigVersion::Own::Id("id_",A_field_type_integer,"ConfigVersion_");
const std::string ConfigVersion::type__("ConfigVersion");
const std::string ConfigVersion::table__("ConfigVersion_");
const std::string ConfigVersion::sequence__("ConfigVersion_seq");
const litesql::FieldType ConfigVersion::Id("id_",A_field_type_integer,table__);
const litesql::FieldType ConfigVersion::Type("type_",A_field_type_string,table__);
const litesql::FieldType ConfigVersion::CfgVersion("cfgVersion_",A_field_type_string,table__);
void ConfigVersion::defaults() {
    id = 0;
}
ConfigVersion::ConfigVersion(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), cfgVersion(CfgVersion) {
    defaults();
}
ConfigVersion::ConfigVersion(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), cfgVersion(CfgVersion) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: cfgVersion = convert<const std::string&, std::string>(rec[2]);
        cfgVersion.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ConfigVersion::ConfigVersion(const ConfigVersion& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), cfgVersion(obj.cfgVersion) {
}
const ConfigVersion& ConfigVersion::operator=(const ConfigVersion& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        cfgVersion = obj.cfgVersion;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string ConfigVersion::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(cfgVersion.name());
    values.push_back(cfgVersion);
    cfgVersion.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ConfigVersion::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ConfigVersion::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, cfgVersion);
}
void ConfigVersion::addIDUpdates(Updates& updates) {
}
void ConfigVersion::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(CfgVersion);
}
void ConfigVersion::delRecord() {
    deleteFromTable(table__, id);
}
void ConfigVersion::delRelations() {
}
void ConfigVersion::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void ConfigVersion::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<ConfigVersion> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ConfigVersion::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<ConfigVersion> ConfigVersion::upcast() const {
    return auto_ptr<ConfigVersion>(new ConfigVersion(*this));
}
std::auto_ptr<ConfigVersion> ConfigVersion::upcastCopy() const {
    ConfigVersion* np = new ConfigVersion(*this);
    np->id = id;
    np->type = type;
    np->cfgVersion = cfgVersion;
    np->inDatabase = inDatabase;
    return auto_ptr<ConfigVersion>(np);
}
std::ostream & operator<<(std::ostream& os, ConfigVersion o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.cfgVersion.name() << " = " << o.cfgVersion << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType DataSyncTime::Own::Id("id_",A_field_type_integer,"DataSyncTime_");
const std::string DataSyncTime::type__("DataSyncTime");
const std::string DataSyncTime::table__("DataSyncTime_");
const std::string DataSyncTime::sequence__("DataSyncTime_seq");
const litesql::FieldType DataSyncTime::Id("id_",A_field_type_integer,table__);
const litesql::FieldType DataSyncTime::Type("type_",A_field_type_string,table__);
const litesql::FieldType DataSyncTime::TableType("tableType_",A_field_type_integer,table__);
const litesql::FieldType DataSyncTime::LastSyncTime("lastSyncTime_",A_field_type_string,table__);
void DataSyncTime::defaults() {
    id = 0;
    tableType = 0;
}
DataSyncTime::DataSyncTime(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), tableType(TableType), lastSyncTime(LastSyncTime) {
    defaults();
}
DataSyncTime::DataSyncTime(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), tableType(TableType), lastSyncTime(LastSyncTime) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: lastSyncTime = convert<const std::string&, std::string>(rec[3]);
        lastSyncTime.setModified(false);
    case 3: tableType = convert<const std::string&, int>(rec[2]);
        tableType.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
DataSyncTime::DataSyncTime(const DataSyncTime& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), tableType(obj.tableType), lastSyncTime(obj.lastSyncTime) {
}
const DataSyncTime& DataSyncTime::operator=(const DataSyncTime& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        tableType = obj.tableType;
        lastSyncTime = obj.lastSyncTime;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string DataSyncTime::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(tableType.name());
    values.push_back(tableType);
    tableType.setModified(false);
    fields.push_back(lastSyncTime.name());
    values.push_back(lastSyncTime);
    lastSyncTime.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void DataSyncTime::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void DataSyncTime::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, tableType);
    updateField(updates, table__, lastSyncTime);
}
void DataSyncTime::addIDUpdates(Updates& updates) {
}
void DataSyncTime::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(TableType);
    ftypes.push_back(LastSyncTime);
}
void DataSyncTime::delRecord() {
    deleteFromTable(table__, id);
}
void DataSyncTime::delRelations() {
}
void DataSyncTime::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void DataSyncTime::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<DataSyncTime> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool DataSyncTime::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<DataSyncTime> DataSyncTime::upcast() const {
    return auto_ptr<DataSyncTime>(new DataSyncTime(*this));
}
std::auto_ptr<DataSyncTime> DataSyncTime::upcastCopy() const {
    DataSyncTime* np = new DataSyncTime(*this);
    np->id = id;
    np->type = type;
    np->tableType = tableType;
    np->lastSyncTime = lastSyncTime;
    np->inDatabase = inDatabase;
    return auto_ptr<DataSyncTime>(np);
}
std::ostream & operator<<(std::ostream& os, DataSyncTime o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.tableType.name() << " = " << o.tableType << std::endl;
    os << o.lastSyncTime.name() << " = " << o.lastSyncTime << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ClassInfo::Own::Id("id_",A_field_type_integer,"ClassInfo_");
const std::string ClassInfo::type__("ClassInfo");
const std::string ClassInfo::table__("ClassInfo_");
const std::string ClassInfo::sequence__("ClassInfo_seq");
const litesql::FieldType ClassInfo::Id("id_",A_field_type_integer,table__);
const litesql::FieldType ClassInfo::Type("type_",A_field_type_string,table__);
const litesql::FieldType ClassInfo::ClassUniqueID("classUniqueID_",A_field_type_integer,table__);
const litesql::FieldType ClassInfo::ClassDescribe("classDescribe_",A_field_type_string,table__);
const litesql::FieldType ClassInfo::TimeStamp("timeStamp_",A_field_type_string,table__);
void ClassInfo::defaults() {
    id = 0;
    classUniqueID = 0;
}
ClassInfo::ClassInfo(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), classUniqueID(ClassUniqueID), classDescribe(ClassDescribe), timeStamp(TimeStamp) {
    defaults();
}
ClassInfo::ClassInfo(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), classUniqueID(ClassUniqueID), classDescribe(ClassDescribe), timeStamp(TimeStamp) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: timeStamp = convert<const std::string&, std::string>(rec[4]);
        timeStamp.setModified(false);
    case 4: classDescribe = convert<const std::string&, std::string>(rec[3]);
        classDescribe.setModified(false);
    case 3: classUniqueID = convert<const std::string&, int>(rec[2]);
        classUniqueID.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
ClassInfo::ClassInfo(const ClassInfo& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), classUniqueID(obj.classUniqueID), classDescribe(obj.classDescribe), timeStamp(obj.timeStamp) {
}
const ClassInfo& ClassInfo::operator=(const ClassInfo& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        classUniqueID = obj.classUniqueID;
        classDescribe = obj.classDescribe;
        timeStamp = obj.timeStamp;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string ClassInfo::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(classUniqueID.name());
    values.push_back(classUniqueID);
    classUniqueID.setModified(false);
    fields.push_back(classDescribe.name());
    values.push_back(classDescribe);
    classDescribe.setModified(false);
    fields.push_back(timeStamp.name());
    values.push_back(timeStamp);
    timeStamp.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void ClassInfo::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ClassInfo::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, classUniqueID);
    updateField(updates, table__, classDescribe);
    updateField(updates, table__, timeStamp);
}
void ClassInfo::addIDUpdates(Updates& updates) {
}
void ClassInfo::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(ClassUniqueID);
    ftypes.push_back(ClassDescribe);
    ftypes.push_back(TimeStamp);
}
void ClassInfo::delRecord() {
    deleteFromTable(table__, id);
}
void ClassInfo::delRelations() {
}
void ClassInfo::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void ClassInfo::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<ClassInfo> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ClassInfo::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<ClassInfo> ClassInfo::upcast() const {
    return auto_ptr<ClassInfo>(new ClassInfo(*this));
}
std::auto_ptr<ClassInfo> ClassInfo::upcastCopy() const {
    ClassInfo* np = new ClassInfo(*this);
    np->id = id;
    np->type = type;
    np->classUniqueID = classUniqueID;
    np->classDescribe = classDescribe;
    np->timeStamp = timeStamp;
    np->inDatabase = inDatabase;
    return auto_ptr<ClassInfo>(np);
}
std::ostream & operator<<(std::ostream& os, ClassInfo o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.classUniqueID.name() << " = " << o.classUniqueID << std::endl;
    os << o.classDescribe.name() << " = " << o.classDescribe << std::endl;
    os << o.timeStamp.name() << " = " << o.timeStamp << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType CameraDeviceConfig::Own::Id("id_",A_field_type_integer,"CameraDeviceConfig_");
const int CameraDeviceConfig::CamUseTypeType::FaceRecognition(0);
const int CameraDeviceConfig::CamUseTypeType::PhotoGraph(1);
CameraDeviceConfig::CamUseTypeType::CamUseTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int CameraDeviceConfig::CamUseType::FaceRecognition(0);
const int CameraDeviceConfig::CamUseType::PhotoGraph(1);
const std::string CameraDeviceConfig::type__("CameraDeviceConfig");
const std::string CameraDeviceConfig::table__("CameraDeviceConfig_");
const std::string CameraDeviceConfig::sequence__("CameraDeviceConfig_seq");
const litesql::FieldType CameraDeviceConfig::Id("id_",A_field_type_integer,table__);
const litesql::FieldType CameraDeviceConfig::Type("type_",A_field_type_string,table__);
const litesql::FieldType CameraDeviceConfig::CamName("camName_",A_field_type_string,table__);
std::vector < std::pair< std::string, std::string > > CameraDeviceConfig::camUseType_values;
const CameraDeviceConfig::CamUseTypeType CameraDeviceConfig::CamUseType("camUseType_",A_field_type_integer,table__,camUseType_values);
void CameraDeviceConfig::initValues() {
    camUseType_values.clear();
    camUseType_values.push_back(make_pair<std::string, std::string>("FaceRecognition","0"));
    camUseType_values.push_back(make_pair<std::string, std::string>("PhotoGraph","1"));
}
void CameraDeviceConfig::defaults() {
    id = 0;
    camUseType = 0;
}
CameraDeviceConfig::CameraDeviceConfig(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), camName(CamName), camUseType(CamUseType) {
    defaults();
}
CameraDeviceConfig::CameraDeviceConfig(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), camName(CamName), camUseType(CamUseType) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: camUseType = convert<const std::string&, int>(rec[3]);
        camUseType.setModified(false);
    case 3: camName = convert<const std::string&, std::string>(rec[2]);
        camName.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
CameraDeviceConfig::CameraDeviceConfig(const CameraDeviceConfig& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), camName(obj.camName), camUseType(obj.camUseType) {
}
const CameraDeviceConfig& CameraDeviceConfig::operator=(const CameraDeviceConfig& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        camName = obj.camName;
        camUseType = obj.camUseType;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string CameraDeviceConfig::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(camName.name());
    values.push_back(camName);
    camName.setModified(false);
    fields.push_back(camUseType.name());
    values.push_back(camUseType);
    camUseType.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void CameraDeviceConfig::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void CameraDeviceConfig::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, camName);
    updateField(updates, table__, camUseType);
}
void CameraDeviceConfig::addIDUpdates(Updates& updates) {
}
void CameraDeviceConfig::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(CamName);
    ftypes.push_back(CamUseType);
}
void CameraDeviceConfig::delRecord() {
    deleteFromTable(table__, id);
}
void CameraDeviceConfig::delRelations() {
}
void CameraDeviceConfig::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void CameraDeviceConfig::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<CameraDeviceConfig> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool CameraDeviceConfig::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<CameraDeviceConfig> CameraDeviceConfig::upcast() const {
    return auto_ptr<CameraDeviceConfig>(new CameraDeviceConfig(*this));
}
std::auto_ptr<CameraDeviceConfig> CameraDeviceConfig::upcastCopy() const {
    CameraDeviceConfig* np = new CameraDeviceConfig(*this);
    np->id = id;
    np->type = type;
    np->camName = camName;
    np->camUseType = camUseType;
    np->inDatabase = inDatabase;
    return auto_ptr<CameraDeviceConfig>(np);
}
std::ostream & operator<<(std::ostream& os, CameraDeviceConfig o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.camName.name() << " = " << o.camName << std::endl;
    os << o.camUseType.name() << " = " << o.camUseType << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType DevHWInfoConfig::Own::Id("id_",A_field_type_integer,"DevHWInfoConfig_");
const std::string DevHWInfoConfig::type__("DevHWInfoConfig");
const std::string DevHWInfoConfig::table__("DevHWInfoConfig_");
const std::string DevHWInfoConfig::sequence__("DevHWInfoConfig_seq");
const litesql::FieldType DevHWInfoConfig::Id("id_",A_field_type_integer,table__);
const litesql::FieldType DevHWInfoConfig::Type("type_",A_field_type_string,table__);
const litesql::FieldType DevHWInfoConfig::Channel("channel_",A_field_type_integer,table__);
const litesql::FieldType DevHWInfoConfig::MaxDevCount("maxDevCount_",A_field_type_integer,table__);
const litesql::FieldType DevHWInfoConfig::DevNum("devNum_",A_field_type_integer,table__);
void DevHWInfoConfig::defaults() {
    id = 0;
    channel = 0;
    maxDevCount = 0;
    devNum = 0;
}
DevHWInfoConfig::DevHWInfoConfig(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), channel(Channel), maxDevCount(MaxDevCount), devNum(DevNum) {
    defaults();
}
DevHWInfoConfig::DevHWInfoConfig(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), channel(Channel), maxDevCount(MaxDevCount), devNum(DevNum) {
    defaults();
    size_t size = (rec.size() > 5) ? 5 : rec.size();
    switch(size) {
    case 5: devNum = convert<const std::string&, int>(rec[4]);
        devNum.setModified(false);
    case 4: maxDevCount = convert<const std::string&, int>(rec[3]);
        maxDevCount.setModified(false);
    case 3: channel = convert<const std::string&, int>(rec[2]);
        channel.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
DevHWInfoConfig::DevHWInfoConfig(const DevHWInfoConfig& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), channel(obj.channel), maxDevCount(obj.maxDevCount), devNum(obj.devNum) {
}
const DevHWInfoConfig& DevHWInfoConfig::operator=(const DevHWInfoConfig& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        channel = obj.channel;
        maxDevCount = obj.maxDevCount;
        devNum = obj.devNum;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string DevHWInfoConfig::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(channel.name());
    values.push_back(channel);
    channel.setModified(false);
    fields.push_back(maxDevCount.name());
    values.push_back(maxDevCount);
    maxDevCount.setModified(false);
    fields.push_back(devNum.name());
    values.push_back(devNum);
    devNum.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void DevHWInfoConfig::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void DevHWInfoConfig::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, channel);
    updateField(updates, table__, maxDevCount);
    updateField(updates, table__, devNum);
}
void DevHWInfoConfig::addIDUpdates(Updates& updates) {
}
void DevHWInfoConfig::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Channel);
    ftypes.push_back(MaxDevCount);
    ftypes.push_back(DevNum);
}
void DevHWInfoConfig::delRecord() {
    deleteFromTable(table__, id);
}
void DevHWInfoConfig::delRelations() {
}
void DevHWInfoConfig::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void DevHWInfoConfig::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<DevHWInfoConfig> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool DevHWInfoConfig::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<DevHWInfoConfig> DevHWInfoConfig::upcast() const {
    return auto_ptr<DevHWInfoConfig>(new DevHWInfoConfig(*this));
}
std::auto_ptr<DevHWInfoConfig> DevHWInfoConfig::upcastCopy() const {
    DevHWInfoConfig* np = new DevHWInfoConfig(*this);
    np->id = id;
    np->type = type;
    np->channel = channel;
    np->maxDevCount = maxDevCount;
    np->devNum = devNum;
    np->inDatabase = inDatabase;
    return auto_ptr<DevHWInfoConfig>(np);
}
std::ostream & operator<<(std::ostream& os, DevHWInfoConfig o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.channel.name() << " = " << o.channel << std::endl;
    os << o.maxDevCount.name() << " = " << o.maxDevCount << std::endl;
    os << o.devNum.name() << " = " << o.devNum << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType DevSWInfoConfig::Own::Id("id_",A_field_type_integer,"DevSWInfoConfig_");
const std::string DevSWInfoConfig::type__("DevSWInfoConfig");
const std::string DevSWInfoConfig::table__("DevSWInfoConfig_");
const std::string DevSWInfoConfig::sequence__("DevSWInfoConfig_seq");
const litesql::FieldType DevSWInfoConfig::Id("id_",A_field_type_integer,table__);
const litesql::FieldType DevSWInfoConfig::Type("type_",A_field_type_string,table__);
const litesql::FieldType DevSWInfoConfig::TestDev("testDev_",A_field_type_integer,table__);
const litesql::FieldType DevSWInfoConfig::TestTimes("testTimes_",A_field_type_integer,table__);
const litesql::FieldType DevSWInfoConfig::TestStartWaitTime("testStartWaitTime_",A_field_type_integer,table__);
const litesql::FieldType DevSWInfoConfig::TestPageCountTime("testPageCountTime_",A_field_type_integer,table__);
const litesql::FieldType DevSWInfoConfig::TestItem("testItem_",A_field_type_string,table__);
void DevSWInfoConfig::defaults() {
    id = 0;
    testDev = 0;
    testTimes = 0;
    testStartWaitTime = 0;
    testPageCountTime = 0;
}
DevSWInfoConfig::DevSWInfoConfig(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), testDev(TestDev), testTimes(TestTimes), testStartWaitTime(TestStartWaitTime), testPageCountTime(TestPageCountTime), testItem(TestItem) {
    defaults();
}
DevSWInfoConfig::DevSWInfoConfig(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), testDev(TestDev), testTimes(TestTimes), testStartWaitTime(TestStartWaitTime), testPageCountTime(TestPageCountTime), testItem(TestItem) {
    defaults();
    size_t size = (rec.size() > 7) ? 7 : rec.size();
    switch(size) {
    case 7: testItem = convert<const std::string&, std::string>(rec[6]);
        testItem.setModified(false);
    case 6: testPageCountTime = convert<const std::string&, int>(rec[5]);
        testPageCountTime.setModified(false);
    case 5: testStartWaitTime = convert<const std::string&, int>(rec[4]);
        testStartWaitTime.setModified(false);
    case 4: testTimes = convert<const std::string&, int>(rec[3]);
        testTimes.setModified(false);
    case 3: testDev = convert<const std::string&, int>(rec[2]);
        testDev.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
DevSWInfoConfig::DevSWInfoConfig(const DevSWInfoConfig& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), testDev(obj.testDev), testTimes(obj.testTimes), testStartWaitTime(obj.testStartWaitTime), testPageCountTime(obj.testPageCountTime), testItem(obj.testItem) {
}
const DevSWInfoConfig& DevSWInfoConfig::operator=(const DevSWInfoConfig& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        testDev = obj.testDev;
        testTimes = obj.testTimes;
        testStartWaitTime = obj.testStartWaitTime;
        testPageCountTime = obj.testPageCountTime;
        testItem = obj.testItem;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string DevSWInfoConfig::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(testDev.name());
    values.push_back(testDev);
    testDev.setModified(false);
    fields.push_back(testTimes.name());
    values.push_back(testTimes);
    testTimes.setModified(false);
    fields.push_back(testStartWaitTime.name());
    values.push_back(testStartWaitTime);
    testStartWaitTime.setModified(false);
    fields.push_back(testPageCountTime.name());
    values.push_back(testPageCountTime);
    testPageCountTime.setModified(false);
    fields.push_back(testItem.name());
    values.push_back(testItem);
    testItem.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void DevSWInfoConfig::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void DevSWInfoConfig::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, testDev);
    updateField(updates, table__, testTimes);
    updateField(updates, table__, testStartWaitTime);
    updateField(updates, table__, testPageCountTime);
    updateField(updates, table__, testItem);
}
void DevSWInfoConfig::addIDUpdates(Updates& updates) {
}
void DevSWInfoConfig::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(TestDev);
    ftypes.push_back(TestTimes);
    ftypes.push_back(TestStartWaitTime);
    ftypes.push_back(TestPageCountTime);
    ftypes.push_back(TestItem);
}
void DevSWInfoConfig::delRecord() {
    deleteFromTable(table__, id);
}
void DevSWInfoConfig::delRelations() {
}
void DevSWInfoConfig::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void DevSWInfoConfig::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<DevSWInfoConfig> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool DevSWInfoConfig::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<DevSWInfoConfig> DevSWInfoConfig::upcast() const {
    return auto_ptr<DevSWInfoConfig>(new DevSWInfoConfig(*this));
}
std::auto_ptr<DevSWInfoConfig> DevSWInfoConfig::upcastCopy() const {
    DevSWInfoConfig* np = new DevSWInfoConfig(*this);
    np->id = id;
    np->type = type;
    np->testDev = testDev;
    np->testTimes = testTimes;
    np->testStartWaitTime = testStartWaitTime;
    np->testPageCountTime = testPageCountTime;
    np->testItem = testItem;
    np->inDatabase = inDatabase;
    return auto_ptr<DevSWInfoConfig>(np);
}
std::ostream & operator<<(std::ostream& os, DevSWInfoConfig o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.testDev.name() << " = " << o.testDev << std::endl;
    os << o.testTimes.name() << " = " << o.testTimes << std::endl;
    os << o.testStartWaitTime.name() << " = " << o.testStartWaitTime << std::endl;
    os << o.testPageCountTime.name() << " = " << o.testPageCountTime << std::endl;
    os << o.testItem.name() << " = " << o.testItem << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType DevTestInfoConfig::Own::Id("id_",A_field_type_integer,"DevTestInfoConfig_");
const int DevTestInfoConfig::TestModeType::Sync(0);
const int DevTestInfoConfig::TestModeType::Async(1);
DevTestInfoConfig::TestModeType::TestModeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int DevTestInfoConfig::TestMode::Sync(0);
const int DevTestInfoConfig::TestMode::Async(1);
const std::string DevTestInfoConfig::type__("DevTestInfoConfig");
const std::string DevTestInfoConfig::table__("DevTestInfoConfig_");
const std::string DevTestInfoConfig::sequence__("DevTestInfoConfig_seq");
const litesql::FieldType DevTestInfoConfig::Id("id_",A_field_type_integer,table__);
const litesql::FieldType DevTestInfoConfig::Type("type_",A_field_type_string,table__);
std::vector < std::pair< std::string, std::string > > DevTestInfoConfig::testMode_values;
const DevTestInfoConfig::TestModeType DevTestInfoConfig::TestMode("testMode_",A_field_type_integer,table__,testMode_values);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::BDisplayScore("bDisplayScore_",A_field_type_boolean,table__);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::BUseRunShortStart("bUseRunShortStart_",A_field_type_boolean,table__);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::RunMidLength("runMidLength_",A_field_type_integer,table__);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::RopeDelayTime("ropeDelayTime_",A_field_type_integer,table__);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::StandingJumpStartPos("standingJumpStartPos_",A_field_type_integer,table__);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::DevConnectMode("devConnectMode_",A_field_type_boolean,table__);
const DevTestInfoConfig::TestModeType DevTestInfoConfig::DevConnectModeAuto("devConnectModeAuto_",A_field_type_boolean,table__);
void DevTestInfoConfig::initValues() {
    testMode_values.clear();
    testMode_values.push_back(make_pair<std::string, std::string>("Sync","0"));
    testMode_values.push_back(make_pair<std::string, std::string>("Async","1"));
}
void DevTestInfoConfig::defaults() {
    id = 0;
    testMode = 0;
    bDisplayScore = 0;
    bUseRunShortStart = true;
    runMidLength = 0;
    ropeDelayTime = 0;
    standingJumpStartPos = 40;
    devConnectMode = false;
    devConnectModeAuto = true;
}
DevTestInfoConfig::DevTestInfoConfig(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), testMode(TestMode), bDisplayScore(BDisplayScore), bUseRunShortStart(BUseRunShortStart), runMidLength(RunMidLength), ropeDelayTime(RopeDelayTime), standingJumpStartPos(StandingJumpStartPos), devConnectMode(DevConnectMode), devConnectModeAuto(DevConnectModeAuto) {
    defaults();
}
DevTestInfoConfig::DevTestInfoConfig(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), testMode(TestMode), bDisplayScore(BDisplayScore), bUseRunShortStart(BUseRunShortStart), runMidLength(RunMidLength), ropeDelayTime(RopeDelayTime), standingJumpStartPos(StandingJumpStartPos), devConnectMode(DevConnectMode), devConnectModeAuto(DevConnectModeAuto) {
    defaults();
    size_t size = (rec.size() > 10) ? 10 : rec.size();
    switch(size) {
    case 10: devConnectModeAuto = convert<const std::string&, bool>(rec[9]);
        devConnectModeAuto.setModified(false);
    case 9: devConnectMode = convert<const std::string&, bool>(rec[8]);
        devConnectMode.setModified(false);
    case 8: standingJumpStartPos = convert<const std::string&, int>(rec[7]);
        standingJumpStartPos.setModified(false);
    case 7: ropeDelayTime = convert<const std::string&, int>(rec[6]);
        ropeDelayTime.setModified(false);
    case 6: runMidLength = convert<const std::string&, int>(rec[5]);
        runMidLength.setModified(false);
    case 5: bUseRunShortStart = convert<const std::string&, bool>(rec[4]);
        bUseRunShortStart.setModified(false);
    case 4: bDisplayScore = convert<const std::string&, bool>(rec[3]);
        bDisplayScore.setModified(false);
    case 3: testMode = convert<const std::string&, int>(rec[2]);
        testMode.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
DevTestInfoConfig::DevTestInfoConfig(const DevTestInfoConfig& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), testMode(obj.testMode), bDisplayScore(obj.bDisplayScore), bUseRunShortStart(obj.bUseRunShortStart), runMidLength(obj.runMidLength), ropeDelayTime(obj.ropeDelayTime), standingJumpStartPos(obj.standingJumpStartPos), devConnectMode(obj.devConnectMode), devConnectModeAuto(obj.devConnectModeAuto) {
}
const DevTestInfoConfig& DevTestInfoConfig::operator=(const DevTestInfoConfig& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        testMode = obj.testMode;
        bDisplayScore = obj.bDisplayScore;
        bUseRunShortStart = obj.bUseRunShortStart;
        runMidLength = obj.runMidLength;
        ropeDelayTime = obj.ropeDelayTime;
        standingJumpStartPos = obj.standingJumpStartPos;
        devConnectMode = obj.devConnectMode;
        devConnectModeAuto = obj.devConnectModeAuto;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string DevTestInfoConfig::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(testMode.name());
    values.push_back(testMode);
    testMode.setModified(false);
    fields.push_back(bDisplayScore.name());
    values.push_back(bDisplayScore);
    bDisplayScore.setModified(false);
    fields.push_back(bUseRunShortStart.name());
    values.push_back(bUseRunShortStart);
    bUseRunShortStart.setModified(false);
    fields.push_back(runMidLength.name());
    values.push_back(runMidLength);
    runMidLength.setModified(false);
    fields.push_back(ropeDelayTime.name());
    values.push_back(ropeDelayTime);
    ropeDelayTime.setModified(false);
    fields.push_back(standingJumpStartPos.name());
    values.push_back(standingJumpStartPos);
    standingJumpStartPos.setModified(false);
    fields.push_back(devConnectMode.name());
    values.push_back(devConnectMode);
    devConnectMode.setModified(false);
    fields.push_back(devConnectModeAuto.name());
    values.push_back(devConnectModeAuto);
    devConnectModeAuto.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void DevTestInfoConfig::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void DevTestInfoConfig::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, testMode);
    updateField(updates, table__, bDisplayScore);
    updateField(updates, table__, bUseRunShortStart);
    updateField(updates, table__, runMidLength);
    updateField(updates, table__, ropeDelayTime);
    updateField(updates, table__, standingJumpStartPos);
    updateField(updates, table__, devConnectMode);
    updateField(updates, table__, devConnectModeAuto);
}
void DevTestInfoConfig::addIDUpdates(Updates& updates) {
}
void DevTestInfoConfig::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(TestMode);
    ftypes.push_back(BDisplayScore);
    ftypes.push_back(BUseRunShortStart);
    ftypes.push_back(RunMidLength);
    ftypes.push_back(RopeDelayTime);
    ftypes.push_back(StandingJumpStartPos);
    ftypes.push_back(DevConnectMode);
    ftypes.push_back(DevConnectModeAuto);
}
void DevTestInfoConfig::delRecord() {
    deleteFromTable(table__, id);
}
void DevTestInfoConfig::delRelations() {
}
void DevTestInfoConfig::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void DevTestInfoConfig::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<DevTestInfoConfig> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool DevTestInfoConfig::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<DevTestInfoConfig> DevTestInfoConfig::upcast() const {
    return auto_ptr<DevTestInfoConfig>(new DevTestInfoConfig(*this));
}
std::auto_ptr<DevTestInfoConfig> DevTestInfoConfig::upcastCopy() const {
    DevTestInfoConfig* np = new DevTestInfoConfig(*this);
    np->id = id;
    np->type = type;
    np->testMode = testMode;
    np->bDisplayScore = bDisplayScore;
    np->bUseRunShortStart = bUseRunShortStart;
    np->runMidLength = runMidLength;
    np->ropeDelayTime = ropeDelayTime;
    np->standingJumpStartPos = standingJumpStartPos;
    np->devConnectMode = devConnectMode;
    np->devConnectModeAuto = devConnectModeAuto;
    np->inDatabase = inDatabase;
    return auto_ptr<DevTestInfoConfig>(np);
}
std::ostream & operator<<(std::ostream& os, DevTestInfoConfig o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.testMode.name() << " = " << o.testMode << std::endl;
    os << o.bDisplayScore.name() << " = " << o.bDisplayScore << std::endl;
    os << o.bUseRunShortStart.name() << " = " << o.bUseRunShortStart << std::endl;
    os << o.runMidLength.name() << " = " << o.runMidLength << std::endl;
    os << o.ropeDelayTime.name() << " = " << o.ropeDelayTime << std::endl;
    os << o.standingJumpStartPos.name() << " = " << o.standingJumpStartPos << std::endl;
    os << o.devConnectMode.name() << " = " << o.devConnectMode << std::endl;
    os << o.devConnectModeAuto.name() << " = " << o.devConnectModeAuto << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType CommitPopupConfig::Own::Id("id_",A_field_type_integer,"CommitPopupConfig_");
const int CommitPopupConfig::DelScoreTypeType::Disable(0);
const int CommitPopupConfig::DelScoreTypeType::Enable(1);
CommitPopupConfig::DelScoreTypeType::DelScoreTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int CommitPopupConfig::DelScoreType::Disable(0);
const int CommitPopupConfig::DelScoreType::Enable(1);
const int CommitPopupConfig::CommitTypeType::Manual(0);
const int CommitPopupConfig::CommitTypeType::Auto(1);
CommitPopupConfig::CommitTypeType::CommitTypeType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int CommitPopupConfig::CommitType::Manual(0);
const int CommitPopupConfig::CommitType::Auto(1);
const int CommitPopupConfig::SignatureExistedType::NotExisted(0);
const int CommitPopupConfig::SignatureExistedType::Existed(1);
CommitPopupConfig::SignatureExistedType::SignatureExistedType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int CommitPopupConfig::SignatureExisted::NotExisted(0);
const int CommitPopupConfig::SignatureExisted::Existed(1);
const int CommitPopupConfig::UserInfoPageExistedType::Exixted(1);
const int CommitPopupConfig::UserInfoPageExistedType::NoExixted(0);
CommitPopupConfig::UserInfoPageExistedType::UserInfoPageExistedType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int CommitPopupConfig::UserInfoPageExisted::Exixted(1);
const int CommitPopupConfig::UserInfoPageExisted::NoExixted(0);
const std::string CommitPopupConfig::type__("CommitPopupConfig");
const std::string CommitPopupConfig::table__("CommitPopupConfig_");
const std::string CommitPopupConfig::sequence__("CommitPopupConfig_seq");
const litesql::FieldType CommitPopupConfig::Id("id_",A_field_type_integer,table__);
const litesql::FieldType CommitPopupConfig::Type("type_",A_field_type_string,table__);
std::vector < std::pair< std::string, std::string > > CommitPopupConfig::delScoreType_values;
const CommitPopupConfig::DelScoreTypeType CommitPopupConfig::DelScoreType("delScoreType_",A_field_type_integer,table__,delScoreType_values);
std::vector < std::pair< std::string, std::string > > CommitPopupConfig::commitType_values;
const CommitPopupConfig::CommitTypeType CommitPopupConfig::CommitType("commitType_",A_field_type_integer,table__,commitType_values);
std::vector < std::pair< std::string, std::string > > CommitPopupConfig::signatureExisted_values;
const CommitPopupConfig::SignatureExistedType CommitPopupConfig::SignatureExisted("signatureExisted_",A_field_type_integer,table__,signatureExisted_values);
const CommitPopupConfig::SignatureExistedType CommitPopupConfig::AutoCommitCountTime("autoCommitCountTime_",A_field_type_integer,table__);
std::vector < std::pair< std::string, std::string > > CommitPopupConfig::userInfoPageExisted_values;
const CommitPopupConfig::UserInfoPageExistedType CommitPopupConfig::UserInfoPageExisted("userInfoPageExisted_",A_field_type_integer,table__,userInfoPageExisted_values);
void CommitPopupConfig::initValues() {
    delScoreType_values.clear();
    delScoreType_values.push_back(make_pair<std::string, std::string>("Disable","0"));
    delScoreType_values.push_back(make_pair<std::string, std::string>("Enable","1"));
    commitType_values.clear();
    commitType_values.push_back(make_pair<std::string, std::string>("Manual","0"));
    commitType_values.push_back(make_pair<std::string, std::string>("Auto","1"));
    signatureExisted_values.clear();
    signatureExisted_values.push_back(make_pair<std::string, std::string>("NotExisted","0"));
    signatureExisted_values.push_back(make_pair<std::string, std::string>("Existed","1"));
    userInfoPageExisted_values.clear();
    userInfoPageExisted_values.push_back(make_pair<std::string, std::string>("Exixted","1"));
    userInfoPageExisted_values.push_back(make_pair<std::string, std::string>("NoExixted","0"));
}
void CommitPopupConfig::defaults() {
    id = 0;
    delScoreType = 0;
    commitType = 0;
    signatureExisted = 0;
    autoCommitCountTime = 0;
    userInfoPageExisted = 1;
}
CommitPopupConfig::CommitPopupConfig(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), delScoreType(DelScoreType), commitType(CommitType), signatureExisted(SignatureExisted), autoCommitCountTime(AutoCommitCountTime), userInfoPageExisted(UserInfoPageExisted) {
    defaults();
}
CommitPopupConfig::CommitPopupConfig(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), delScoreType(DelScoreType), commitType(CommitType), signatureExisted(SignatureExisted), autoCommitCountTime(AutoCommitCountTime), userInfoPageExisted(UserInfoPageExisted) {
    defaults();
    size_t size = (rec.size() > 7) ? 7 : rec.size();
    switch(size) {
    case 7: userInfoPageExisted = convert<const std::string&, int>(rec[6]);
        userInfoPageExisted.setModified(false);
    case 6: autoCommitCountTime = convert<const std::string&, int>(rec[5]);
        autoCommitCountTime.setModified(false);
    case 5: signatureExisted = convert<const std::string&, int>(rec[4]);
        signatureExisted.setModified(false);
    case 4: commitType = convert<const std::string&, int>(rec[3]);
        commitType.setModified(false);
    case 3: delScoreType = convert<const std::string&, int>(rec[2]);
        delScoreType.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
CommitPopupConfig::CommitPopupConfig(const CommitPopupConfig& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), delScoreType(obj.delScoreType), commitType(obj.commitType), signatureExisted(obj.signatureExisted), autoCommitCountTime(obj.autoCommitCountTime), userInfoPageExisted(obj.userInfoPageExisted) {
}
const CommitPopupConfig& CommitPopupConfig::operator=(const CommitPopupConfig& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        delScoreType = obj.delScoreType;
        commitType = obj.commitType;
        signatureExisted = obj.signatureExisted;
        autoCommitCountTime = obj.autoCommitCountTime;
        userInfoPageExisted = obj.userInfoPageExisted;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
std::string CommitPopupConfig::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(delScoreType.name());
    values.push_back(delScoreType);
    delScoreType.setModified(false);
    fields.push_back(commitType.name());
    values.push_back(commitType);
    commitType.setModified(false);
    fields.push_back(signatureExisted.name());
    values.push_back(signatureExisted);
    signatureExisted.setModified(false);
    fields.push_back(autoCommitCountTime.name());
    values.push_back(autoCommitCountTime);
    autoCommitCountTime.setModified(false);
    fields.push_back(userInfoPageExisted.name());
    values.push_back(userInfoPageExisted);
    userInfoPageExisted.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void CommitPopupConfig::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void CommitPopupConfig::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, delScoreType);
    updateField(updates, table__, commitType);
    updateField(updates, table__, signatureExisted);
    updateField(updates, table__, autoCommitCountTime);
    updateField(updates, table__, userInfoPageExisted);
}
void CommitPopupConfig::addIDUpdates(Updates& updates) {
}
void CommitPopupConfig::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(DelScoreType);
    ftypes.push_back(CommitType);
    ftypes.push_back(SignatureExisted);
    ftypes.push_back(AutoCommitCountTime);
    ftypes.push_back(UserInfoPageExisted);
}
void CommitPopupConfig::delRecord() {
    deleteFromTable(table__, id);
}
void CommitPopupConfig::delRelations() {
}
void CommitPopupConfig::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void CommitPopupConfig::del() {
    if (!typeIsCorrect()) {
        std::auto_ptr<CommitPopupConfig> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool CommitPopupConfig::typeIsCorrect() const {
    return type == type__;
}
std::auto_ptr<CommitPopupConfig> CommitPopupConfig::upcast() const {
    return auto_ptr<CommitPopupConfig>(new CommitPopupConfig(*this));
}
std::auto_ptr<CommitPopupConfig> CommitPopupConfig::upcastCopy() const {
    CommitPopupConfig* np = new CommitPopupConfig(*this);
    np->id = id;
    np->type = type;
    np->delScoreType = delScoreType;
    np->commitType = commitType;
    np->signatureExisted = signatureExisted;
    np->autoCommitCountTime = autoCommitCountTime;
    np->userInfoPageExisted = userInfoPageExisted;
    np->inDatabase = inDatabase;
    return auto_ptr<CommitPopupConfig>(np);
}
std::ostream & operator<<(std::ostream& os, CommitPopupConfig o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.delScoreType.name() << " = " << o.delScoreType << std::endl;
    os << o.commitType.name() << " = " << o.commitType << std::endl;
    os << o.signatureExisted.name() << " = " << o.signatureExisted << std::endl;
    os << o.autoCommitCountTime.name() << " = " << o.autoCommitCountTime << std::endl;
    os << o.userInfoPageExisted.name() << " = " << o.userInfoPageExisted << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
STE100Database::STE100Database(std::string backendType, std::string connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> STE100Database::getSchema() const {
    vector<Database::SchemaItem> res;
    string TEXT = backend->getSQLType(A_field_type_string);
    string rowIdType = backend->getRowIDType();
    res.push_back(Database::SchemaItem("schema_","table","CREATE TABLE schema_ (name_ "+TEXT+", type_ "+TEXT+", sql_ "+TEXT+")"));
    if (backend->supportsSequences()) {
        res.push_back(Database::SchemaItem("UserIdentityInfo_seq","sequence",backend->getCreateSequenceSQL("UserIdentityInfo_seq")));
        res.push_back(Database::SchemaItem("TestItemInfo_seq","sequence",backend->getCreateSequenceSQL("TestItemInfo_seq")));
        res.push_back(Database::SchemaItem("TestItemResult_seq","sequence",backend->getCreateSequenceSQL("TestItemResult_seq")));
        res.push_back(Database::SchemaItem("ScoreRules_seq","sequence",backend->getCreateSequenceSQL("ScoreRules_seq")));
        res.push_back(Database::SchemaItem("ConfigVersion_seq","sequence",backend->getCreateSequenceSQL("ConfigVersion_seq")));
        res.push_back(Database::SchemaItem("DataSyncTime_seq","sequence",backend->getCreateSequenceSQL("DataSyncTime_seq")));
        res.push_back(Database::SchemaItem("ClassInfo_seq","sequence",backend->getCreateSequenceSQL("ClassInfo_seq")));
        res.push_back(Database::SchemaItem("CameraDeviceConfig_seq","sequence",backend->getCreateSequenceSQL("CameraDeviceConfig_seq")));
        res.push_back(Database::SchemaItem("DevHWInfoConfig_seq","sequence",backend->getCreateSequenceSQL("DevHWInfoConfig_seq")));
        res.push_back(Database::SchemaItem("DevSWInfoConfig_seq","sequence",backend->getCreateSequenceSQL("DevSWInfoConfig_seq")));
        res.push_back(Database::SchemaItem("DevTestInfoConfig_seq","sequence",backend->getCreateSequenceSQL("DevTestInfoConfig_seq")));
        res.push_back(Database::SchemaItem("CommitPopupConfig_seq","sequence",backend->getCreateSequenceSQL("CommitPopupConfig_seq")));
    }
    res.push_back(Database::SchemaItem("UserIdentityInfo_","table","CREATE TABLE UserIdentityInfo_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",codeUniqueID_ " + backend->getSQLType(A_field_type_bigint,"") + " UNIQUE" +",roleType_ " + backend->getSQLType(A_field_type_integer,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"") + "" +",brithday_ " + backend->getSQLType(A_field_type_string,"") + "" +",gender_ " + backend->getSQLType(A_field_type_boolean,"") + "" +",studentID_ " + backend->getSQLType(A_field_type_string,"") + "" +",iDCardNo_ " + backend->getSQLType(A_field_type_string,"") + "" +",iDMagCard_ " + backend->getSQLType(A_field_type_string,"") + "" +",nGrade_ " + backend->getSQLType(A_field_type_integer,"") + "" +",nClass_ " + backend->getSQLType(A_field_type_integer,"") + "" +",nation_ " + backend->getSQLType(A_field_type_string,"") + "" +",faceFeature_ " + backend->getSQLType(A_field_type_string,"") + "" +",faceImagePath_ " + backend->getSQLType(A_field_type_string,"") + "" +",campusMagcard_ " + backend->getSQLType(A_field_type_string,"") + "" +",timeStamp_ " + backend->getSQLType(A_field_type_string,"") + "" +",stage_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("TestItemInfo_","table","CREATE TABLE TestItemInfo_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",testItem_ " + backend->getSQLType(A_field_type_integer,"") + " UNIQUE" +",testItemDescribe_ " + backend->getSQLType(A_field_type_string,"") + "" +",testItemUnit_ " + backend->getSQLType(A_field_type_string,"") + "" +",testItemUntiDescribe_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("TestItemResult_","table","CREATE TABLE TestItemResult_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",codeUniqueID_ " + backend->getSQLType(A_field_type_bigint,"") + "" +",testItem_ " + backend->getSQLType(A_field_type_integer,"") + "" +",testNature_ " + backend->getSQLType(A_field_type_integer,"") + "" +",testTime_ " + backend->getSQLType(A_field_type_string,"") + "" +",testMulitResult_ " + backend->getSQLType(A_field_type_string,"") + "" +",processDataPath_ " + backend->getSQLType(A_field_type_string,"") + "" +",confidence_ " + backend->getSQLType(A_field_type_string,"") + "" +",resultSignature_ " + backend->getSQLType(A_field_type_string,"") + "" +",syncServer_ " + backend->getSQLType(A_field_type_boolean,"") + "" +",deviceID_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("ScoreRules_","table","CREATE TABLE ScoreRules_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",testItem_ " + backend->getSQLType(A_field_type_integer,"") + "" +",grade_ " + backend->getSQLType(A_field_type_integer,"") + "" +",gender_ " + backend->getSQLType(A_field_type_boolean,"") + "" +",valueLeft_ " + backend->getSQLType(A_field_type_float,"") + "" +",valueRight_ " + backend->getSQLType(A_field_type_float,"") + "" +",score_ " + backend->getSQLType(A_field_type_float,"") + "" +",ageS_ " + backend->getSQLType(A_field_type_float,"") + "" +",ageE_ " + backend->getSQLType(A_field_type_float,"") + "" +",version_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("ConfigVersion_","table","CREATE TABLE ConfigVersion_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",cfgVersion_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("DataSyncTime_","table","CREATE TABLE DataSyncTime_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",tableType_ " + backend->getSQLType(A_field_type_integer,"") + " UNIQUE" +",lastSyncTime_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("ClassInfo_","table","CREATE TABLE ClassInfo_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",classUniqueID_ " + backend->getSQLType(A_field_type_integer,"") + " UNIQUE" +",classDescribe_ " + backend->getSQLType(A_field_type_string,"") + "" +",timeStamp_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("CameraDeviceConfig_","table","CREATE TABLE CameraDeviceConfig_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",camName_ " + backend->getSQLType(A_field_type_string,"") + "" +",camUseType_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("DevHWInfoConfig_","table","CREATE TABLE DevHWInfoConfig_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",channel_ " + backend->getSQLType(A_field_type_integer,"") + "" +",maxDevCount_ " + backend->getSQLType(A_field_type_integer,"") + "" +",devNum_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("DevSWInfoConfig_","table","CREATE TABLE DevSWInfoConfig_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",testDev_ " + backend->getSQLType(A_field_type_integer,"") + "" +",testTimes_ " + backend->getSQLType(A_field_type_integer,"") + "" +",testStartWaitTime_ " + backend->getSQLType(A_field_type_integer,"") + "" +",testPageCountTime_ " + backend->getSQLType(A_field_type_integer,"") + "" +",testItem_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("DevTestInfoConfig_","table","CREATE TABLE DevTestInfoConfig_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",testMode_ " + backend->getSQLType(A_field_type_integer,"") + "" +",bDisplayScore_ " + backend->getSQLType(A_field_type_boolean,"") + "" +",bUseRunShortStart_ " + backend->getSQLType(A_field_type_boolean,"") + "" +",runMidLength_ " + backend->getSQLType(A_field_type_integer,"") + "" +",ropeDelayTime_ " + backend->getSQLType(A_field_type_integer,"") + "" +",standingJumpStartPos_ " + backend->getSQLType(A_field_type_integer,"") + "" +",devConnectMode_ " + backend->getSQLType(A_field_type_boolean,"") + "" +",devConnectModeAuto_ " + backend->getSQLType(A_field_type_boolean,"") + "" +")"));
    res.push_back(Database::SchemaItem("CommitPopupConfig_","table","CREATE TABLE CommitPopupConfig_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",delScoreType_ " + backend->getSQLType(A_field_type_integer,"") + "" +",commitType_ " + backend->getSQLType(A_field_type_integer,"") + "" +",signatureExisted_ " + backend->getSQLType(A_field_type_integer,"") + "" +",autoCommitCountTime_ " + backend->getSQLType(A_field_type_integer,"") + "" +",userInfoPageExisted_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("UserIdentityInfo_id_idx","index","CREATE INDEX UserIdentityInfo_id_idx ON UserIdentityInfo_ (id_)"));
    res.push_back(Database::SchemaItem("TestItemInfo_id_idx","index","CREATE INDEX TestItemInfo_id_idx ON TestItemInfo_ (id_)"));
    res.push_back(Database::SchemaItem("TestItemResult_id_idx","index","CREATE INDEX TestItemResult_id_idx ON TestItemResult_ (id_)"));
    res.push_back(Database::SchemaItem("ScoreRules_id_idx","index","CREATE INDEX ScoreRules_id_idx ON ScoreRules_ (id_)"));
    res.push_back(Database::SchemaItem("ConfigVersion_id_idx","index","CREATE INDEX ConfigVersion_id_idx ON ConfigVersion_ (id_)"));
    res.push_back(Database::SchemaItem("DataSyncTime_id_idx","index","CREATE INDEX DataSyncTime_id_idx ON DataSyncTime_ (id_)"));
    res.push_back(Database::SchemaItem("ClassInfo_id_idx","index","CREATE INDEX ClassInfo_id_idx ON ClassInfo_ (id_)"));
    res.push_back(Database::SchemaItem("CameraDeviceConfig_id_idx","index","CREATE INDEX CameraDeviceConfig_id_idx ON CameraDeviceConfig_ (id_)"));
    res.push_back(Database::SchemaItem("DevHWInfoConfig_id_idx","index","CREATE INDEX DevHWInfoConfig_id_idx ON DevHWInfoConfig_ (id_)"));
    res.push_back(Database::SchemaItem("DevSWInfoConfig_id_idx","index","CREATE INDEX DevSWInfoConfig_id_idx ON DevSWInfoConfig_ (id_)"));
    res.push_back(Database::SchemaItem("DevTestInfoConfig_id_idx","index","CREATE INDEX DevTestInfoConfig_id_idx ON DevTestInfoConfig_ (id_)"));
    res.push_back(Database::SchemaItem("CommitPopupConfig_id_idx","index","CREATE INDEX CommitPopupConfig_id_idx ON CommitPopupConfig_ (id_)"));
    return res;
}
void STE100Database::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
    UserIdentityInfo::initValues();
    CameraDeviceConfig::initValues();
    DevTestInfoConfig::initValues();
    CommitPopupConfig::initValues();
}
}
