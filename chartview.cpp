#include "chartview.h"

#include <QValueAxis>
#include <QLineSeries>

ChartView::ChartView(QWidget *parent)
    : QChartView(parent)
    , chart(new QChart)
    , series(new QLineSeries)
{
    initChart();
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
    axisX->setRange(chart_x_min, chart_x_max);
    axisX->setLabelFormat("%g");
    axisX->setTickCount(chart_x_tick_counts);
    axisX->setTitleText("");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    delete axisX; // reduce %CPU

    // ----------- Y Axis -----------
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(chart_y_min, chart_y_max);
    axisY->setLabelFormat("%g");
    axisY->setTitleText("");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    delete axisY; // reduce %CPU

    QFont titleFont(title_font_family, title_font_size, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitle(QString("Frequency:"));
    chart->legend()->hide();

    this->setChart(chart);
    //this->setRenderHint(QPainter::Antialiasing); //Disable to reduce %CPU
}

