#ifndef CGRADE_H
#define CGRADE_H

#include <string>
#include <map>
#include <list>
#include <QDateTime>

//新的计算年级需要进行的步骤较多添加命名空间进行限制,参照java代码进行生成,服务器代码更新之后需要进行对应的更新

struct GradeInfo{
    int stage;
    std::string gradeName;
    int gradeNo;
    int scoreGradeNo;
};

//不清楚 是个啥玩意儿
struct GradeVo{
    int stage;
    std::string gradeName;
    int gradeNo;
    int scoreGradeNo;
    int year;
};
class CGrade
{
public:
    CGrade();
    std::list<GradeVo> getConcreteClazzGradeVoByStageAndEntranceYear(const int &stage, const int &year);

    int calculateGradeOneYear();

    GradeVo getGradeByStageAndYearAndDate(QDate date, const int &stage, const int &year);

    void initMap();

private:
    //学校类型和阶段关联关系map<schoolType,list<stage>>
    std::map<int, std::list<int>> typeStageMap;
    //某阶段毕业年级map<stage,毕业年级>
    std::map<int, int> stageGraduateInfoMap;
    //年级相关固定参数值<stage,list<GradeInfo>> 初始化到缓存
    std::map<int, std::list<GradeInfo>> gradeInfoMap;
};

#endif // CGRADE_H
