#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include "udpsocket.h"
#include "jsondata.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT

public:
    ChartView(QWidget *parent = nullptr);

    /// @brief Initiation of chart plotting
    void initChart();

    QChart *chart;
    QLineSeries *series;

private:

private slots:
//    /// @brief Update charts whenever udp packages arrive
//    void onReceiveUDP();
};

#endif // CHARTVIEW_H
