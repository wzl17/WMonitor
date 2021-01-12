#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "jsondata.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QXYSeries>
#include <QtCharts/QLineSeries>
#include <QStatusBar>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QGroupBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

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
    /// @brief Initiation of config box
    void initConfigBox();
    /// @brief Initiation of control buttons
    void initButtons();
    /// @brief Initiation of UDP socket
    void initSocket();
    /// @brief Create status bar
    void createStatus();

    Ui::MainWindow *ui;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *sideLayout;

    QGroupBox *configBoxGroup;
    QSpinBox *channelBox;
    qint16 WM_CHANNEL = 1;
    QLineEdit *portBox;

    QGroupBox *controlGroup;
    QPushButton *startButton;
    QPushButton *stopButton;

    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;

    QUdpSocket *udpSocket;
    QNetworkDatagram datagram;
    qint32 UDP_PORT = 9898;
    JsonData *json;

private slots:
    /// @brief Update charts whenever udp packages arrive
    void onReceiveUDP();
    /// @brief Update channel according to signal
    void switchChannel(qint16 newchannel);
    /// @brief Update UDP port according to signal
    void switchPort();
    /// @brief Start UDP connection
    void startUDP();
    /// @brief Stop UDP connection
    void stopUDP();
};
#endif // MAINWINDOW_H
