#include "CGrade.h"
using namespace std;
//从小学
static int SCORE_GRADE_NO[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
//某一阶段年级对应的数字，最大到6年级
static int GRADE_NO[] = {1, 2, 3, 4, 5, 6};
//不同学校阶段对应的毕业年级
static int GRADUATE_GRADE_NO[] = {3, 4, 5, 6};
//幼儿园年级名称
static string KINDERGARTEN_GRADE_NAME[] = {"小班", "中班", "大班"};
//国标小学年级名称，兼容5年制小学的
static string STANDARD_PRIMARY_GRADE_NAME[] = {"小学一年级", "小学二年级", "小学三年级", "小学四年级", "小学五年级", "小学六年级"};
//包含国标的初中年级名称
static string STANDARD_JUNIOR_GRADE_NAME[] = {"初中一年级", "初中二年级", "初中三年级", "初中四年级"};
//国标高中年级名称
static string STANDARD_SENIOR_GRADE_NAME[] = {"高中一年级", "高中二年级", "高中三年级"};
//包含国标的大学年级名称
static string STANDARD_UNIVERSITY_GRADE_NAME[] = {"大学一年级", "大学二年级", "大学三年级", "大学四年级", "大学五年级"};
//九义校年级名称
static string NINE_YEAR_GRADE_NAME[] = {"一年级", "二年级", "三年级", "四年级", "五年级", "六年级", "七年级", "八年级", "九年级"};
//学期相关 固定时间点码值
static string SEMESTER_ONE_BEGIN_TIME = "-09-01 00:00:00";
static string SEMESTER_ONE_END_TIME = "-02-13 59:59:59";
static string SEMESTER_TWO_BEGIN_TIME = "-02-14 00:00:00";
static string SEMESTER_TWO_END_TIME = "-08-31 23:59:59";
//学校类型码值
/**
 * 幼儿园
 */
static int TYPE_YEY = 1;
/**
 * 小学
 */
static int TYPE_XX = 2;
/**
 * 初中
 */
static int TYPE_CZ = 3;
/**
 * 高中
 */
static int TYPE_GZ = 4;
/**
 * 大学
 */
static int TYPE_DX = 5;
/**
 * 九义校
 */
static int TYPE_JYX = 6;
/**
 * 高完中
 */
static int TYPE_GWZ = 7;
/**
 * 5年制小学
 */
static int TYPE_XX_5 = 8;
/**
 * 4年制高中
 */
static int TYPE_CZ_4 = 9;
/**
 * 5年制大学
 */
static int TYPE_DX_5 = 10;

//班级对应的stage，注意一个学校可能有多个stage 比如 九义校 有小学阶段，和初中阶段。此版本将学校阶段严格拆分成了：幼儿园小学，初中，高中，大学，阶段
//幼儿园
static int STAGE_YEY = 1;
//小学
static int STAGE_XX = 2;
//初中
static int STAGE_CZ = 3;
//高中
static int STAGE_GZ = 4;
//大学
static int STAGE_DX = 5;
//九义校的小学阶段
static int STAGE_JYX_XX = 6;
//九义校的初中阶段
static int STAGE_JYX_CZ = 7;
//高完中的初中阶段
static int STAGE_GWZ_CZ = 8;
//高完中的高中阶段
static int STAGE_GWZ_GZ = 9;
//5年制小学
static int STAGE_XX_5 = 10;
//4年制初中
static int STAGE_CZ_4 = 11;
//5年制大学
static int STAGE_DX_5 = 12;

CGrade::CGrade()
{
    initMap();
}

std::list<GradeVo> CGrade::getConcreteClazzGradeVoByStageAndEntranceYear(const int &stage, const int &year){
    std::list<GradeVo> listRet;
    auto itFind = gradeInfoMap.find(stage);
    if(itFind == gradeInfoMap.end())
    {
        return listRet;
    }

    int yearTemp = year;

    GradeVo gradeVo;
    for(auto &gradeInfo : itFind->second)
    {
        gradeVo.gradeNo = gradeInfo.gradeNo;
        gradeVo.scoreGradeNo = gradeInfo.scoreGradeNo;
        gradeVo.gradeName = gradeInfo.gradeName;
        gradeVo.year = yearTemp--;
        gradeVo.stage = gradeInfo.stage;
        listRet.push_back(gradeVo);
    }
    return listRet;
}

int CGrade::calculateGradeOneYear() {
    QDateTime time = QDateTime::currentDateTime();
    QDate date = time.date();
    int yearOfIncomingTime = date.year();
    std::string semesterTwoBeginPointStr = to_string(yearOfIncomingTime) + SEMESTER_TWO_BEGIN_TIME;
    std::string semesterTwoEndPointStr = to_string(yearOfIncomingTime) + SEMESTER_TWO_END_TIME;

    static QString csTimeFormat("yyyy-MM-dd hh:mm:ss");
    QDateTime dateTimeBegin = QDateTime::fromString(QString::fromStdString(semesterTwoEndPointStr), csTimeFormat);
    QDateTime dateTimeEnd = QDateTime::fromString(QString::fromStdString(semesterTwoEndPointStr), csTimeFormat);

    //计算出1年级的入学年级
    int realYearOfGradeOne;
    //传入的时间介于某年的第二学期之间 ，因为第二学期是不会跨年的 年不会发生变化，第一学期可能会跨年，年会变化，else中单独处理
    if (time >= dateTimeBegin && time <= dateTimeEnd) {
        realYearOfGradeOne = yearOfIncomingTime - 1; //传入的时间是第二学期，那么对应的入学年份减1
    } else { //是第1学期，判断传入的时间是否跨年了
        int monthValue = date.month();
        if (9 <= monthValue && monthValue <= 12) { //传入的时间未跨年的时候
            realYearOfGradeOne = yearOfIncomingTime;
        } else { //跨年了，则需要减1
            realYearOfGradeOne = yearOfIncomingTime - 1;
        }
    }
    return realYearOfGradeOne;
}

GradeVo CGrade::getGradeByStageAndYearAndDate(QDate date, const int &stage, const int &year) {
    GradeVo gradeVo;
    gradeVo.gradeNo = 0;
    gradeVo.scoreGradeNo = 0;
    gradeVo.year = 0;
    gradeVo.stage = 0;
    gradeVo.gradeName = "未知";
    std::list<GradeVo> gradeVolist = getConcreteClazzGradeVoByStageAndEntranceYear(stage, year);

    int nRet = -1;
    if (gradeVolist.empty()) {
        //throw ("通过入学年级和学校阶段未获取到对应的年级名称");
        return gradeVo;
    }
    //获取传入的时间点所在的学年
    //当前stage下的1年级的年级
    int gradeNoOfBegin = (*gradeVolist.begin()).gradeNo;
    //当前stage下最大的年级
    int gradeNoOfEnd = (*gradeVolist.rbegin()).gradeNo;
    //当前是几年级
    int realYearOfGradeOne = calculateGradeOneYear();
    int gradeNo = realYearOfGradeOne - year + 1;
    //传入的年份比实际1年的年份都还小，则是未入学
    if (gradeNo < gradeNoOfBegin) {
        gradeVo.gradeName = "未入学";
    }
    //传入的年份比实际最大年的年份都还大，则是已毕业
    else if (gradeNo > gradeNoOfEnd) {
        gradeVo.gradeName = "已毕业";
    }
    //年级值和算出来的年级值相等的则是传入的时间点入学year对应的年级
    else
    {
        nRet = 0;
        for(auto &gradVo : gradeVolist)
        {
            if(gradVo.gradeNo == gradeNo)
            {
                gradeVo = gradVo;
                break;
            }
        }
    }

    return gradeVo;
}

void CGrade::initMap()
{
    //初始化各个阶段毕业年级
    stageGraduateInfoMap[STAGE_YEY]    = GRADUATE_GRADE_NO[0];
    stageGraduateInfoMap[STAGE_XX]     = GRADUATE_GRADE_NO[3];
    stageGraduateInfoMap[STAGE_CZ]     = GRADUATE_GRADE_NO[0];
    stageGraduateInfoMap[STAGE_GZ]     = GRADUATE_GRADE_NO[0];
    stageGraduateInfoMap[STAGE_DX]     = GRADUATE_GRADE_NO[1];
    stageGraduateInfoMap[STAGE_JYX_XX] = GRADUATE_GRADE_NO[3];
    stageGraduateInfoMap[STAGE_JYX_CZ] = GRADUATE_GRADE_NO[0];
    stageGraduateInfoMap[STAGE_GWZ_CZ] = GRADUATE_GRADE_NO[0];
    stageGraduateInfoMap[STAGE_GWZ_GZ] = GRADUATE_GRADE_NO[0];
    stageGraduateInfoMap[STAGE_XX_5]   = GRADUATE_GRADE_NO[2];
    stageGraduateInfoMap[STAGE_CZ_4]   = GRADUATE_GRADE_NO[1];
    stageGraduateInfoMap[STAGE_DX_5]   = GRADUATE_GRADE_NO[2];

    list<int> listTemp;

    //初始化学校类型和阶段对应关系,高完中，九义校，一个学校会存在两个stage阶段，支持往多的扩展，正常的学校只会有一个stage阶段
    listTemp.clear();
    listTemp.push_back(STAGE_YEY);
    typeStageMap[TYPE_YEY] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_XX);
    typeStageMap[TYPE_XX] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_CZ);
    typeStageMap[TYPE_CZ] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_GZ);
    typeStageMap[TYPE_GZ] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_DX);
    typeStageMap[TYPE_DX] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_JYX_XX);
    listTemp.push_back(STAGE_JYX_CZ);
    typeStageMap[TYPE_JYX] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_GWZ_CZ);
    listTemp.push_back(STAGE_XX);
    typeStageMap[TYPE_GWZ] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_XX_5);
    typeStageMap[TYPE_XX_5] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_CZ_4);
    typeStageMap[TYPE_CZ_4] = listTemp;

    listTemp.clear();
    listTemp.push_back(STAGE_DX_5);
    typeStageMap[TYPE_DX_5] = listTemp;

    //初始换年级相关的参数
    //幼儿园

    list<GradeInfo> gradeInfolist;
    GradeInfo gradeInfo;

    gradeInfo.stage = STAGE_YEY;
    gradeInfo.gradeName = KINDERGARTEN_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    //幼儿园没有得分标准，计算得分对应的年级设置成0
    gradeInfo.scoreGradeNo = 0;
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_YEY;
    gradeInfo.gradeName = KINDERGARTEN_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = 0;
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_YEY;
    gradeInfo.gradeName = KINDERGARTEN_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = 0;
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_YEY] = gradeInfolist;
    //小学
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_XX;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[0];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[1];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[2];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[3];
    gradeInfo.gradeNo = GRADE_NO[3];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[3];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[4];
    gradeInfo.gradeNo = GRADE_NO[4];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[4];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[5];
    gradeInfo.gradeNo = GRADE_NO[5];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[5];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_XX] = gradeInfolist;
    //初中
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_CZ;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[6];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_CZ;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[7];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_CZ;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[8];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_CZ] = gradeInfolist;
    //高中
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_GZ;
    gradeInfo.gradeName = STANDARD_SENIOR_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[9];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_GZ;
    gradeInfo.gradeName = STANDARD_SENIOR_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[10];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_GZ;
    gradeInfo.gradeName = STANDARD_SENIOR_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[11];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_GZ] = gradeInfolist;
    //大学
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_DX;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[12];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[13];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[14];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[3];
    gradeInfo.gradeNo = GRADE_NO[3];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[15];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_DX] = gradeInfolist;
    //九义校小学
    gradeInfo.stage = STAGE_JYX_XX;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[0];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_XX;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[1];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_XX;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[2];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_XX;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[3];
    gradeInfo.gradeNo = GRADE_NO[3];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[3];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_XX;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[4];
    gradeInfo.gradeNo = GRADE_NO[4];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[4];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_XX;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[5];
    gradeInfo.gradeNo = GRADE_NO[5];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[5];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_JYX_XX] = gradeInfolist;
    //九义校初中
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_JYX_CZ;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[6];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[6];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_CZ;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[7];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[7];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_JYX_CZ;
    gradeInfo.gradeName = NINE_YEAR_GRADE_NAME[8];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[8];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_JYX_CZ] = gradeInfolist;
    //高完中初中
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_GWZ_CZ;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[6];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_GWZ_CZ;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[7];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_GWZ_CZ;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[8];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_GWZ_CZ] = gradeInfolist;
    //高完中高中
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_GWZ_GZ;
    gradeInfo.gradeName = STANDARD_SENIOR_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[9];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_GWZ_GZ;
    gradeInfo.gradeName = STANDARD_SENIOR_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[10];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_GWZ_GZ;
    gradeInfo.gradeName = STANDARD_SENIOR_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[11];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_GWZ_GZ] = gradeInfolist;
    //5年制制小学
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_XX_5;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[0];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX_5;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[1];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX_5;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[2];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX_5;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[3];
    gradeInfo.gradeNo = GRADE_NO[3];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[3];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_XX_5;
    gradeInfo.gradeName = STANDARD_PRIMARY_GRADE_NAME[4];
    gradeInfo.gradeNo = GRADE_NO[4];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[4];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_XX_5] = gradeInfolist;
    //4年制初中
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_CZ_4;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[5];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_CZ_4;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[6];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_CZ_4;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[7];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_CZ_4;
    gradeInfo.gradeName = STANDARD_JUNIOR_GRADE_NAME[3];
    gradeInfo.gradeNo = GRADE_NO[3];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[8];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_CZ_4] = gradeInfolist;
    //5年制大学
    gradeInfolist.clear();
    gradeInfo.stage = STAGE_DX_5;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[0];
    gradeInfo.gradeNo = GRADE_NO[0];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[12];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX_5;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[1];
    gradeInfo.gradeNo = GRADE_NO[1];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[13];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX_5;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[2];
    gradeInfo.gradeNo = GRADE_NO[2];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[14];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX_5;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[3];
    gradeInfo.gradeNo = GRADE_NO[3];
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[15];
    gradeInfolist.push_back(gradeInfo);
    gradeInfo.stage = STAGE_DX_5;
    gradeInfo.gradeName = STANDARD_UNIVERSITY_GRADE_NAME[4];
    gradeInfo.gradeNo = GRADE_NO[4];
    //目前没遇到此类大学，不清楚业务咋样，故默认大5的成绩标准和大四是一样的
    gradeInfo.scoreGradeNo = SCORE_GRADE_NO[15];
    gradeInfolist.push_back(gradeInfo);
    gradeInfoMap[STAGE_DX_5] = gradeInfolist;
}
