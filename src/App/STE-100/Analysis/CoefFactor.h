/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:CoefFactor.h
* 文件标识:
* 摘要: 文件实现人体成分系数定义的申明
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2018.07.17
*/

#ifndef _CoefFactor_h
#define _CoefFactor_h

//系数因子的行数与列数
#define ROW_NUM 52//一共52个成分系数
#define COL_NUM 35
/*
0:头部重量(可能推定值)(单位:Kg)
1:头部脂肪(可能推定值)(单位:Kg)
2:头部肌肉(可能推定值)(单位:Kg)
3:全身体脂百分比(单位:%)
4:未知成分
5:全身去脂体重(瘦组织+无机盐)(单位:Kg)
6:全身体脂肪(单位:Kg)
7:右手肌肉(单位:Kg)
8:左手肌肉(单位:Kg)
9:躯干肌肉(单位:Kg)
10:右腿肌肉(单位:Kg)
11:左腿肌肉(单位:Kg)
12:未知成分
13:全身肌肉量(单位:Kg)
14:SLM-SLMO(单位:Kg)
15:全身无机盐(单位:Kg)
16:全身瘦组织(TBW水+蛋白质PW)(单位:Kg)
17:右手细胞外液率ECF/TBF
18:左手细胞外液率ECF/TBF
19:躯干细胞外液率ECF/TBF
20:右腿细胞外液率ECF/TBF
21:左腿细胞外液率ECF/TBF
22:右手细胞外水分率ECW/TBW
23:左手细胞外水分率ECW/TBW
24:躯干细胞外水分率ECW/TBW
25:右腿细胞外水分率ECW/TBW
26:左腿细胞外水分率ECW/TBW
27:全身细胞外液率ECF/TBF
28:全身细胞外水分率ECW/TBW
29:全身细胞内水分ICW(单位:Kg)
30:全身细胞外水分ECW(单位:Kg)
31:全身身体总水分TBW(单位:Kg)
32:全身蛋白质PW(单位:Kg)
33:未知成分
34:未知成分
35:未知成分
36:右手脂肪(单位:Kg)
37:左手脂肪(单位:Kg)
38:躯干脂肪(单位:Kg)
39:右腿脂肪(单位:Kg)
40:左腿脂肪(单位:Kg)
41:去头体重(可能推定值)(单位:Kg)
42:未知成分
43:未知成分
44:未知成分
45:未知成分
46:未知成分
47:未知成分
48:腰围ABD
49:臀围HIP
50:全身腰臀比WHR
51:内账脂肪指数VFA(单位:面积/10cm)
*/

#ifdef __cplusplus
extern "C"
{
#endif

extern const double g_CoefFactorCode[ROW_NUM][COL_NUM];//系数因子

#ifdef __cplusplus
}
#endif
#endif
