#ifndef COMMONDEFIN_H
#define COMMONDEFIN_H

//添加过程资源路径定义
#define TEST_PROC_SRC "TestProcRes"

///、------------ 定义软件测试项目编号 short---------------------
#define TEST_ITEM_UNKNOW             0x00
#define TEST_ITEM_HEIGHT             0x01
#define TEST_ITEM_WEIGHT             0x02
//人体成分分析
#define TEST_ITEM_HBC                0x03
//肺活量
#define TEST_ITEM_VC                 0x04
//立定跳远standing broad jump
#define TEST_ITEM_SBJ                0x05
//坐位体前屈 Sitting forward bending
#define TEST_ITEM_SFB                0x06
//引体向上 Pull-ups
#define TEST_ITEM_PULL_UP            0x07
//仰卧起坐 Sit-up
#define TEST_ITEM_SIT_UP             0x08
//BMI
#define TEST_ITEM_BMI                0x09
//握力
#define TEST_ITEM_GRIP               0x12
//俯卧撑
#define TEST_ITEM_PUSHUP             0x13
//视力
#define TEST_ITEM_EYESIGHT           0x14

//肺通气量
#define TEST_ITEM_PVV                0x16
#define TEST_ITEM_PVV_VC             0x161 //肺活量
#define TEST_ITEM_PVV_FVC            0x162 //用力肺活量
#define TEST_ITEM_PVV_MPV            0x163 //最大肺通气量
//足球
#define TEST_ITEM_FOOTBALL           0x17
//篮球
#define TEST_ITEM_BASKETBALL         0x18
//排球
#define TEST_ITEM_VOLLEYBALL         0x19
//斜身引体向上 PULL_UP_OBLIQUE
#define TEST_ITEM_PUO                0x1A
//实心球
#define TEST_ITEM_SOLIDBALL          0x1B
//纵跳摸高 VERTICAL_JUMP
#define TEST_ITEM_VJUMP              0x1C

//1000m
#define TEST_ITEM_RUN_1000           0xA0
//800m
#define TEST_ITEM_RUN_800            0xA1
//50m
#define TEST_ITEM_RUN_50             0xA2
//跳绳
#define TEST_ITEM_ROPE_SKIP          0xA3
//往返跑 8*50
#define TEST_ITEM_RUN_BACK_8_50      0xA4
//比赛模式(跑步:60,100,200,400,4X100 跨栏:100,110)
#define TEST_ITEM_RUN_60             0xA5
#define TEST_ITEM_RUN_100            0xA6
#define TEST_ITEM_RUN_200            0xA8
#define TEST_ITEM_RUN_400            0xA9
#define TEST_ITEM_RUN_4_100          0xAA
#define TEST_ITEM_RUN_HURDLES_100    0xAC
#define TEST_ITEM_RUN_HURDLES_110    0xA7
//1500m
#define TEST_ITEM_RUN_1500           0xAB

//往返跑 4*10
#define TEST_ITEM_RUN_BACK_4_10      0xAD
//2000m
#define TEST_ITEM_RUN_2000           0xAE

///、------------ 定义测试设备编号 short---------------------
//如身高体重仪可以测试 TEST_ITEM_HEIGHT TEST_ITEM_WEIGHT TEST_ITEM_HBC TEST_ITEM_BMI这几个项目

#define DEV_TYPE_NETWORK             -1    //组网
#define DEV_TYPE_UNKNOW              0x00
//身高体重人体成分仪器，定义为体型仪
#define DEV_TYPE_BODY                0x01
//肺活量
#define DEV_TYPE_VC                  0x02
//立定跳远仪 standing broad jump
#define DEV_TYPE_SBJ                 0x03
//坐位体前屈 Sitting forward bending
#define DEV_TYPE_SFB                 0x04
//引体向上 Pull-ups
#define DEV_TYPE_PULL_UP             0x05
//仰卧起坐 Sit-ups
#define DEV_TYPE_SIT_UP              0x06
//跑步
#define DEV_TYPE_RUN_SHORT           0x07
#define DEV_TYPE_RUN_MID             0x08
//跳绳
#define DEV_TYPE_ROPE_SKIP           0x09
//握力
#define DEV_TYPE_GRIP                0x0A
//俯卧撑
#define DEV_TYPE_PUSH_UP             0x0B
//视力
#define DEV_TYPE_EYESIGHT            0x0C
//肺通气量
#define DEV_TYPE_PVV                 0x0D
//足球
#define DEV_TYPE_FOOTBALL            0x0E
//篮球
#define DEV_TYPE_BASKETBALL          0x0F
//排球
#define DEV_TYPE_VOLLEYBALL          0x10
//斜身引体向上 PULL_UP_OBLIQUE
#define DEV_TYPE_PUO                 0x11
//实心球
#define DEV_TYPE_SOLIDBALL           0x12
//纵跳摸高 VERTICAL_JUMP
#define DEV_TYPE_VJUMP               0x13


///定义测量犯规类型
const unsigned char FOUL_TYPE_ = 0x00;

template<class T>
void safeDeleteClassPoint(T **p)
{
    if(p == nullptr)
        return;

    if(*p != nullptr)
        delete *p;

    *p = nullptr;
}

template<class T>
void safeNewClassPoint(T **p)
{
    safeDeleteClassPoint(p);

    *p = new T();
}

//在字符转换过程中，float 和 double 都会出现精度损失
//因此在此处对浮点数的比较进行统一
//返回值 >0 即Num1 > Num2; =0 即Num1=Num2; <0 即Num1<Num2
template<class T>
int compareFloating(const T &Num1,const T &Num2,const float EXPSINON = 0.00001f)
{
    if((Num1 - Num2) > EXPSINON)
    {
        return 1;
    }
    else if((Num1 - Num2) < -EXPSINON)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

#endif // COMMONDEFIN_H
