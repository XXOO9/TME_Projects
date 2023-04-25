#ifndef CCUSTOMCURVEVIEW_H
#define CCUSTOMCURVEVIEW_H

#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QVariant>
//#include "GlobalCommandDefine.h"
//#include "CCycleQueue1.h"
//#include "Storage/CAHEFileStorge.h"

//定义一屏最大显示数据（24h） 24 * 60 * 60 * 10
#define MAX_PIONT_SIZE 864000
//#define MAX_PIONT_SIZE 30000

class CustomCurveView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString xUnit READ getXUnit WRITE setXUnit)
    Q_PROPERTY(QString yUnit READ getYUnit WRITE setYUnit)
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(float scale READ getScalevalue WRITE setScaleValue)
    Q_PROPERTY(int chartType READ getChartType WRITE setChartType)
    Q_PROPERTY(int replayOffset READ getReplayOffset WRITE setReplayOffset)
    Q_PROPERTY(int chaWidth READ getChaWidth WRITE setChaWidth)
    Q_PROPERTY(int chaHeight READ getChaHeight WRITE setChaHeight)
    Q_PROPERTY(int chaFactor READ getScreenFactor WRITE setScreenFactor)


public:
    CustomCurveView(QQuickItem *parent = 0);
    ~CustomCurveView() override;
    CustomCurveView(const CustomCurveView& rhs) = delete;
    CustomCurveView(CustomCurveView&& rhs) = delete;
    CustomCurveView& operator= (const CustomCurveView& rhs) = delete;
    CustomCurveView& operator= (CustomCurveView&& rhs) = delete;

    QString getName() const;
    void setName(const QString &name);

    QString getXUnit() const;
    void setXUnit(const QString &unit);

    QString getYUnit() const;
    void setYUnit(const QString &unit);

    QColor getColor() const;
    void setColor(const QColor &color);

    int getChartType() const;
    void setChartType(const int &type);

    float getScalevalue() const;
    void setScaleValue(const float &value);

    int getReplayOffset() const;
    void setReplayOffset(int value);

    int getChaWidth() const;
    void setChaWidth(int Cha_Width);

    int getChaHeight() const;
    void setChaHeight(int Cha_Height);

    float getScreenFactor() const;
    void setScreenFactor(float screenFactor);

    void paint(QPainter *m_painter) override;
    Q_INVOKABLE void updatePaint();
    Q_INVOKABLE void set_Point_Data(long lLen, QVector<float> var);             //设置氧气
    Q_INVOKABLE void load_Point_Data(float data);

    QString getTimeStr(unsigned long long value);

signals:
    void  checkFileValue(bool value);                     //检查文件有效性
    void  sendSingleChaData(float fOxygen,float fCarbon,float fTemp,float fHumi); //发送单通道数据
    void  sendLableData(QString label);                   //发送标签显示
private:
    void drawAxisX();   //绘制x坐标轴
    void drawAxisY();   //绘制y坐标轴
    void drawCurve();   //绘制曲线

    void updateSampToXPixel();   //更新采样点到像素点的映射

    void initialize();
    void setPenBrush();
    int  getAxisYValue(double value);

    void drawAxisLine(QPainter *m_painter);

    void setRandArray();
private:
    QPainter           *m_painter{nullptr};          //绘制指针
    QVector<float>      m_qlFrame_Data;              //保存事件帧数据信息(用于事件帧波形的绘制)
    QVector<float>      m_qSingle_Data;              //保存单个的数据
    //double m_qlFrame_Data[SAMPLE_RATE*5];
    QVector<double>     m_qlFrame_test;              //保存事件帧数据信息(用于事件帧波形的绘制)
    short		        m_shUnit_NumX{6};			 //屏幕宽度内窗口单元格格数，默认为5
    int                 m_Cha_Width{538};            //通道宽度
    int                 m_Cha_Height{200};           //通道高度

    double              m_Samp_To_XPixel;            //保存当前帧的采样点到像素点的映射，即一个采样点对应几个像素点
    int                 m_Samp_To_YPixel;            //采样点y轴映射

    int                 m_Recv_Time{100};
    int                 m_Recv_Count{1};

    int                 m_index{0};
    float               m_scale{float(0.1)};                  //放大比例
    int                 m_MimDataCount{1};             //获取最小采样点数据
    int                 m_MaxDataCout{3000};           //获取最大采样点数据

    float               m_screenFactor{ float(0.34)};

    QString m_xUnit;
    QString m_yUnit;
    QString m_name;
    QColor m_color;
    double m_F1{21.0};
    double m_F2{0.0};
    double m_F3{9.0};
    double m_F4{0.0};
    double m_F5{0.0};

    int m_chartType{0};         //0-监控 1-反演
    QMap<QString,int> m_xMap;
    QMap<QString, double> m_yMap;

    int iDataOffset{0}; //数据偏移量
    int iReplayOffset{0}; //反演数据偏移量

    QPen m_curvePen, m_AxisPen, m_DotlinePen, m_TextPen;
    QFont m_font;
};


#endif // CCUSTOMCURVEVIEW_H
