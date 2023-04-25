#ifndef CCUSTFVCHARTVIEW_H
#define CCUSTFVCHARTVIEW_H

#include <QtQuick>
#include "CProjectEnums.h"
class QCustomPlot;
class QCPCurve;
class QCPAbstractPlottable;
class QCPScatterStyle;
class QCPItemText;

class CCustFVChartView : public QQuickPaintedItem
{
    Q_OBJECT

public:
    CCustFVChartView(QQuickItem* parent = nullptr);
    virtual ~CCustFVChartView();

    void paint(QPainter* painter);

    Q_INVOKABLE void initCustomPlot();
    Q_INVOKABLE void recvData(const int &stageType, const float &volume, const float &flow);

protected:
    void routeMouseEvents( QMouseEvent* event );
    void routeWheelEvents( QWheelEvent* event );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent *event );

    virtual void timerEvent(QTimerEvent *event);

signals:
    Q_INVOKABLE void sigStartInspiration();
    Q_INVOKABLE void sigStartExpiration();

private:
    QCustomPlot* m_CustomPlot;
    QCPCurve*    m_fvExCurve;   //呼气阶段
    QCPCurve*    m_fvInCurve;   //吸气阶段
    int          m_timerId;
    QColor       m_color;
    long long    m_dataCount{0};

private slots:
    void graphClicked( QCPAbstractPlottable* plottable );
    void onCustomReplot();
    void updateCustomPlotSize();
};

#endif // CCUSTFVCHARTVIEW_H
