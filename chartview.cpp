#include "chartview.h"

#include <QValueAxis>
#include <QLineSeries>

ChartView::ChartView(QWidget *parent)
    : QChartView(parent)
    , chart(new QChart)
    , series(new QLineSeries)
{
}

// cause pure virtual function error
//ChartView::~ChartView()
//{
//    delete chart;
//    delete series;
//}

void ChartView::initChart()
{
    chart->addSeries(series);

    // ----------- X Axis -----------
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 2048);
    axisX->setLabelFormat("%g");
    axisX->setTickCount(5);
    axisX->setTitleText("");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    delete axisX; // reduce %CPU

    // ----------- Y Axis -----------
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 4000);
    axisY->setLabelFormat("%g");
    axisY->setTitleText("");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    delete axisY; // reduce %CPU

    chart->setTitle(QString("Frequency:"));
    chart->legend()->hide();

    this->setChart(chart);
    //this->setRenderHint(QPainter::Antialiasing);
}

