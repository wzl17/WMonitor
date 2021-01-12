#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "jsondata.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QXYSeries>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QStatusBar>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QObject>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QUdpSocket;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /// @brief Initiation of UI
    void initUI();
    /// @brief Initiation of chart plotting
    void initChart();
    /// @brief Initiation of UDP socket
    void initSocket();
    /// @brief Create status bar
    void createStatus();

    Ui::MainWindow *ui;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;

    QUdpSocket *udpSocket;
    QNetworkDatagram datagram;
    JsonData *json;

private slots:
    /// @brief Update charts whenever udp packages arrive
    void onReceiveUDP();
};
#endif // MAINWINDOW_H
