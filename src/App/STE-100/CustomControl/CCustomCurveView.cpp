#include "CCustomCurveView.h"
#include <QDebug>
#include <QJsonObject>
#include <math.h>
#include <QGuiApplication>
#include <QScreen>

CustomCurveView::CustomCurveView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    initialize();
}

CustomCurveView::~CustomCurveView()
{
    QVector<float>().swap(m_qlFrame_Data);
}

void CustomCurveView::initialize()
{
    QScreen *screen = QGuiApplication::primaryScreen ();
    int height = screen->size().height();
    m_screenFactor = height == 1080.0 ? float(( height - 100 )/ 2880.0 ): float(height / 2880.0);
    m_Cha_Width = int((1582 - 100) * m_screenFactor);
    m_Cha_Height = int((864 - 150) * m_screenFactor);
    m_shUnit_NumX = 6;

    m_Recv_Time = 100;   //ms
    m_Recv_Count = 1;  //一帧数据10个点

    m_qlFrame_Data.reserve(MAX_PIONT_SIZE);

}

QString CustomCurveView::getName() const
{
    return m_name;
}

void CustomCurveView::setName(const QString &name)
{
    m_name = name;
}

QString CustomCurveView::getXUnit() const
{
    return m_xUnit;
}

void CustomCurveView::setXUnit(const QString &unit)
{
    m_xUnit = unit;
}

QString CustomCurveView::getYUnit() const
{
    return m_yUnit;
}

void CustomCurveView::setYUnit(const QString &unit)
{
    m_yUnit = unit;
}

QColor CustomCurveView::getColor() const
{
    return m_color;
}

void CustomCurveView::setColor(const QColor &color)
{
    m_color = color;
}

int CustomCurveView::getChartType() const
{
    return m_chartType;
}

void CustomCurveView::setChartType(const int &type)
{
    m_chartType = type;
}

float CustomCurveView::getScalevalue() const
{
    return m_scale;
}

//修改坐标轴放大比例
void CustomCurveView::setScaleValue(const float &value)
{
    m_scale = value;
    int totalCount = int( (1000/m_Recv_Time)*m_Recv_Count*60*m_scale * (m_shUnit_NumX-1) );

    if(m_qlFrame_Data.size() > totalCount && !m_chartType)
    {
        iDataOffset = m_qlFrame_Data.size() - totalCount;
    }
    else
    {
        iDataOffset=0;
    }

    update();
}

int CustomCurveView::getReplayOffset() const
{
    return iReplayOffset;
}

//设置反演时间轴偏移
void CustomCurveView::setReplayOffset(int value)
{
    iReplayOffset = value;
    qDebug()<< "iReplayOffset" <<iReplayOffset;
    update();
}

void CustomCurveView::setPenBrush()
{
    //线条画笔
    m_DotlinePen.setColor("#143D97");
    m_DotlinePen.setWidth(int(1 * m_screenFactor));
    m_DotlinePen.setStyle(Qt::DotLine);

    //坐标轴画笔
    m_AxisPen.setColor("#3695FF");
    m_AxisPen.setWidth(int(2 * m_screenFactor));
    m_AxisPen.setStyle(Qt::SolidLine);

    //文本画笔
    m_TextPen.setColor("#A6CCFF");
    m_TextPen.setWidth(1);
    m_TextPen.setStyle(Qt::SolidLine);

    //曲线画笔
    m_curvePen.setColor("#FF6B6E");
    m_TextPen.setWidth(2);
    m_TextPen.setStyle(Qt::SolidLine);

    //设置字体
    m_font.setFamily("Microsoft YaHei");
    m_font.setPixelSize(16);
}

//获取坐标轴y值
int  CustomCurveView::getAxisYValue(double value)
{
    int height = int (m_Cha_Height - 40 * m_screenFactor);
    int yValue = 0;

    //double nums[5] = {m_F1,m_F2,m_F3,m_F4,m_F5};
    //double yMax = *std::max_element(nums, nums + 5);
    double yMax = 200.0;
    if(yMax != 0.0)
    {
        yValue = int(height/yMax*value);
    }
    return yValue;
}

void CustomCurveView::updatePaint()
{
    update();
}

void CustomCurveView::updateSampToXPixel()
{
    if(m_qlFrame_Data.size() > 0)
    {
        //通过是否绘制X轴来判断绘制区域，进而确定像素点和物理点间的映射关系
        //得到X轴实际单元间隔长度(和当前扫描速度对应)，考虑到默认满屏显示，则长度即整体宽度
        int iUnitLength = int(m_Cha_Width-100* m_screenFactor)/ (m_shUnit_NumX-1);

        //根据采样率得到1ms占多少个采样点
        int iUnitSampNum = 1;
        if(!m_chartType)
        {
            iUnitSampNum = int( (1000/m_Recv_Time) * m_Recv_Count * 60 * m_scale );
        }
        else
        {
            iUnitSampNum = (1000/m_Recv_Time) * m_Recv_Count * 60; //* m_scale;
        }

        //得到采样点到像素点的映射关系(1个采样点占多少个像素点)
        m_Samp_To_XPixel = double (iUnitLength / iUnitSampNum );
    }
}

//绘制x坐标轴
void CustomCurveView::drawAxisX()
{
    //设置文字大小
    m_painter->setFont(m_font);

    //绘制x 轴线
    m_painter->setPen(m_AxisPen);
    m_painter->drawLine(0,0,m_Cha_Width,0);

    //绘制x轴单位
    m_painter->setPen(m_TextPen);
    m_painter->drawText(m_Cha_Width - 30, 20, m_xUnit);

    //计算单位间隔长度 10格，每个像素大小66
    int unitLength = int(m_Cha_Width-100 * m_screenFactor)/ (m_shUnit_NumX-1);

    for(int i = 0;i < m_shUnit_NumX;++i)
    {
        //绘制刻度线
        m_painter->setPen(m_AxisPen);
        m_painter->drawLine(i*unitLength,0,i*unitLength,6);

        //绘制10s刻度
        for(int j=0 ; j<6 && i<m_shUnit_NumX-1; j++)
        {
            double xPos = (i*unitLength)+(unitLength/6 * j);
            m_painter->drawLine(xPos,0,xPos,2);
        }

        /*******绘制刻度值*********/
        //1、计算当前刻度大小
        int iCurLength = i * unitLength;

        //2、得到当前刻度占用的时间(ms),间隔数乘以扫描速度(默认满屏显示)
        unsigned long long iCur_MSecond = 0;
        if(!m_chartType)
        {
            //实时监控
            iCur_MSecond = static_cast<unsigned long long>((i * 60 * 1000 * m_scale) + iDataOffset * 100);
        }
        else
        {
            iCur_MSecond = static_cast<unsigned long long>((i * 60 * 1000 * m_scale ) + iReplayOffset * 100);
        }

        //3、分别得到对应的分、秒、毫秒值并转化为QString类型
        QString qsMSecond;
        qsMSecond = getTimeStr(iCur_MSecond);

        //4、绘制当前刻度值，默认样式：00:00:000
        QRect rect = QRect(iCurLength - 30,5, 110,30);
        m_painter->setPen(m_TextPen);
        m_painter->setFont(m_font);
        m_painter->drawText(rect,Qt::AlignCenter,qsMSecond);
    }
}

void CustomCurveView::drawAxisY()
{
    //设置文字大小
    m_painter->setFont(m_font);

    //绘制Y轴坐标
    m_yMap.insert("F5", m_F5);
    m_yMap.insert("F4", m_F4);
    m_yMap.insert("F3", m_F3);
    m_yMap.insert("F2", m_F2);
    m_yMap.insert("F1", m_F1);

    //绘制y 轴线
    m_painter->setPen(m_AxisPen);
    m_painter->drawLine(0,-m_Cha_Height,0,0);

    //绘制y轴单位
    m_painter->setPen(m_TextPen);
    m_painter->drawText(-30, -m_Cha_Height, m_yUnit);

    //绘制y轴
    /*
    QMap<QString, double>::iterator it_yAxis;
    double yValue = 0;
    for(it_yAxis=m_yMap.begin();it_yAxis!=m_yMap.end();it_yAxis++)
    {
        if(it_yAxis.value()>0)
        {
            yValue = getAxisYValue(it_yAxis.value());

            m_painter->setPen(m_DotlinePen);
            m_painter->drawLine(0, -yValue, int(m_Cha_Width-4*m_screenFactor), -yValue);

            m_painter->setPen(m_AxisPen);
            m_painter->drawLine(0,-yValue, int(-4*m_screenFactor),-yValue);

            m_painter->setPen(m_TextPen);
            QRect rect = QRect(-42 ,-yValue-10,38,20);
            m_painter->drawText(rect,Qt::AlignRight, QString::number(it_yAxis.value()));
        }
    }
    */
}

void CustomCurveView::drawCurve()
{
    //设置曲线绘制笔刷
    m_painter->setPen(m_curvePen);

    //设置字体
    QFont font;
    font.setPointSize(1);
    m_painter->setFont(font);

    //设置边界
    //int width = int(m_Cha_Width / m_shUnit_NumX * 10 * m_screenFactor);
    //QRect rect = QRect(0,int(-250* m_screenFactor),width,int(250* m_screenFactor));
    //m_painter->setClipRect(rect);

    //vector
    if(m_qlFrame_Data.size() > 0)
    {
        //得到Y方向第一个点的值
        double firstPos_Actual_Y = getAxisYValue(double(m_qlFrame_Data.at(0 + iDataOffset)));

        //从第二个点开始遍历，依次连接前后两个点
        for(long i = 1 + iDataOffset;i < m_qlFrame_Data.size();++i)
        {
            //通过采样点大小映射为Y轴对应的值
            double fPos_Actual_Y = getAxisYValue(double(m_qlFrame_Data.at(i)));

            //连接前后两个点(注意，这里值的正负号转换是因为要对应当前坐标轴)
            QPointF point1 = QPointF((i  - 1 - iDataOffset) * m_Samp_To_XPixel,firstPos_Actual_Y * (-1));
            QPointF point2 = QPointF((i - iDataOffset) * m_Samp_To_XPixel,fPos_Actual_Y * (-1));
            m_painter->drawLine(point1, point2);

            //对第一个点重新赋值
            firstPos_Actual_Y = fPos_Actual_Y;
        }
    }
}

void CustomCurveView::paint(QPainter *painter)
{
    m_painter = painter;

    //参数初始化
    initialize();

    //构造笔刷
    setPenBrush();

    updateSampToXPixel();

    //设置反锯齿模式
    m_painter->setRenderHints(QPainter::Antialiasing, true);

    //外围的矩形(标识)
    //m_painter->drawRect(0,0,730,280);

    //确定坐标轴起点坐标，这里定义(53,187)
    m_painter->translate(10, 260);

    //构造坐标系
    //drawAxisLine(m_painter);
    drawAxisX();
    drawAxisY();
    drawCurve();
}

void CustomCurveView::setRandArray()
{
    for(int i=0; i<50000; i++)
    {
        int value = (rand() % (210-90+1))+ 90;
        m_qlFrame_test.push_back(value/10);
    }
}

float CustomCurveView::getScreenFactor() const
{
    return m_screenFactor;
}

void CustomCurveView::setScreenFactor(float screenFactor)
{
    m_screenFactor = screenFactor;
}

int CustomCurveView::getChaHeight() const
{
    return m_Cha_Height;
}

void CustomCurveView::setChaHeight(int Cha_Height)
{
    m_Cha_Height = Cha_Height;
}

int CustomCurveView::getChaWidth() const
{
    return m_Cha_Width;
}

void CustomCurveView::setChaWidth(int Cha_Width)
{
    m_Cha_Width = Cha_Width;
}

void CustomCurveView::set_Point_Data(long lLen, QVector<float> varData)
{
    if(lLen <= 0)
        return;

    //绘制传入点数据
    m_qlFrame_Data.clear();
    iDataOffset = 0;
    std::copy( varData.begin(), varData.end(), m_qlFrame_Data.begin() );
    //刷新绘制
    update();
}

void CustomCurveView::load_Point_Data(float data)
{
    //有数据就加入
    m_qlFrame_Data.push_back(data);

    //超出一屏最大显示点数
    if(m_qlFrame_Data.size() > MAX_PIONT_SIZE)
    {
        //数据偏移量为0，数据出栈驱动
        std::copy( m_qlFrame_Data.begin() + 1, m_qlFrame_Data.end(), m_qlFrame_Data.begin() );
        iDataOffset = 0;
        m_qlFrame_Data.pop_back();
    }

    //超过当前比例下的一屏最大显示数，计算偏移
    if(m_qlFrame_Data.size()>(1000/m_Recv_Time)*m_Recv_Count * 60 * m_scale * (m_shUnit_NumX-1))
    {
        iDataOffset++;
    }

    /*
    QVector<float>(m_qlFrame_Data).swap(m_qlFrame_Data);

    if(m_qlFrame_Data.size()>(1000/m_Recv_Time)*m_Recv_Count*60*5)
    {
        iDataOffset++;
        //超出界面范围,数据前推
        for(int i = 0; i<m_qlFrame_Data.size()-1; i++)
        {
            m_qlFrame_Data[i] = m_qlFrame_Data[i+1];
        }
        m_qlFrame_Data.pop_back();
    }
    */

    //刷新绘制
    update();
}

QString CustomCurveView::getTimeStr(unsigned long long value)
{
    QString time = "";
    int hour = 0;
    int min = 0;
    int sec = 0;
    int msec = 0;

    if(m_scale > 6)
    {
        //hh:mm:ss
        hour = int(value/3600000);
        min = value%3600000 / 60000;
        sec = (value%60000)/1000;
        msec = value%1000;
        time = QString("%1:%2:%3").arg(hour,2,10,QLatin1Char('0')).arg(min,2,10,QLatin1Char('0')) .arg(sec,2,10,QLatin1Char('0'));
    }
    else
    {
        //mm:ss.msec
        min = int(value/60000);
        sec = (value%60000)/1000;
        msec = value%1000;
        time = QString("%1:%2.%3").arg(min,2,10,QLatin1Char('0')).arg(sec,2,10,QLatin1Char('0')) .arg(msec,3,10,QLatin1Char('0'));
    }
    return time;
}
