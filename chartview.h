#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include "globals.h"
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

signals:
    void freqChanged(double freq);

};

#endif // CHARTVIEW_H
