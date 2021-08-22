#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include "globals.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class PatternChart : public QWidget
{
    Q_OBJECT

public:
    PatternChart(QWidget *parent = nullptr);
    ~PatternChart();

    /// @brief Initiation of chart plotting
    void initChart();

    QChart *chart;
    QLineSeries *series;
};

class LaserFreqPlot : public QWidget
{
    Q_OBJECT

public:
    explicit LaserFreqPlot(Laser *laser, QWidget *parent = nullptr);
    ~LaserFreqPlot();

    QChart *chart;
    QLineSeries *series;

private:
    void initPage();

    Laser *m_laser;
    QVector<QPointF> m_buffer;
    quint16 count;
    QLabel *freqLabel;

public slots:
    void updateFreq();

};

#endif // CHARTVIEW_H
