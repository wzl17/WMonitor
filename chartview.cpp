#include "chartview.h"

#include <QValueAxis>
#include <QLineSeries>

PatternChart::PatternChart(QWidget *parent)
    : QWidget(parent)
    , chart(new QChart)
    , series(new QLineSeries)
{
    initChart();
}

PatternChart::~PatternChart()
{
}

void PatternChart::initChart()
{
    chart->addSeries(series);

    // ----------- X Axis -----------
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(pattern_x_min, pattern_x_max);
    axisX->setLabelFormat("%g");
    axisX->setTickCount(pattern_x_tick_counts);
    axisX->setTitleText("");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    delete axisX; // reduce %CPU

    // ----------- Y Axis -----------
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(pattern_y_min, pattern_y_max);
    axisY->setLabelFormat("%g");
    axisY->setTitleText("");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    delete axisY; // reduce %CPU

    QFont titleFont(pattern_title_font_family, pattern_title_font_size, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitle(QString("Frequency:"));
    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setMinimumSize(300,150);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView);
    //this->setRenderHint(QPainter::Antialiasing); //Disable to reduce %CPU
}

LaserFreqPlot::LaserFreqPlot(laserCtrl *laser, QWidget *parent) :
    QWidget(parent),
    chart(new QChart),
    series(new QLineSeries),
    m_laser(laser)
{
    initPage();
}

LaserFreqPlot::~LaserFreqPlot()
{
    delete freqLabel;
}

void LaserFreqPlot::initPage()
{
    QChartView *chartView = new QChartView(chart);
    chartView->setMinimumSize(400,200);
    chart->addSeries(series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, freqplot_x_length);
    axisX->setLabelFormat("%g");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(m_laser->setpoint - m_laser->plot_range/2, m_laser->setpoint + m_laser->plot_range/2);
    axisY->setTitleText("Frequency/THz");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->hide();
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView);
    count = 0;
}

void LaserFreqPlot::updateFreq()
{
    if (m_buffer.size() < freqplot_x_length) {
        m_buffer.append(QPointF(count,channels_freqs[m_laser->wm_channel]));
        count++;
    } else {
        for (int i = 0; i < freqplot_x_length-1; ++i)
            m_buffer[i].setY(m_buffer[i+1].y());
        m_buffer[freqplot_x_length-1].setY(channels_freqs[m_laser->wm_channel]);
    }
    series->replace(m_buffer);
}

