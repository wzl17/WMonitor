#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QSizePolicy>
#include <QNetworkDatagram>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    config_dir = QCoreApplication::applicationDirPath()+"/configs/config.ini";
    readConfig();

    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    udpSocket = new UdpSocket;
    chartView = new ChartView;
    QObject::connect(udpSocket, &QUdpSocket::readyRead,
                     chartView, [=](){
        while (udpSocket->hasPendingDatagrams()) {
            json = new JsonData;
            json->loadWavemeterData(udpSocket->receiveDatagram().data());
            chartView->series->replace(json->wm_pattern);
            chartView->chart->setTitle(QString("Frequency:%1 THz").arg(json->wm_freq,0,'f',5));
            emit chartView->freqChanged(json->wm_freq);
            delete json;
        }
    });
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(chartView);
    sideLayout = new QVBoxLayout;
    sideLayout->addWidget(udpSocket->groupWidget);

    laserControl = new LaserControl;
    QObject::connect(chartView, &ChartView::freqChanged,
                     laserControl, &LaserControl::voltFeedback);
    QObject::connect(laserControl, &LaserControl::error,
                     this, &MainWindow::statusShow);
    sideLayout->addWidget(laserControl->groupWidget);
    if (servo1_enable) {
        servo1 = new Shutters(servo1_name,servo1_channel,servo1_on,servo1_off,servo1_scan_on,servo1_scan_off,servo1_scan_on_time,servo1_scan_off_time);
        QObject::connect(servo1, &Shutters::error,
                         this, &MainWindow::statusShow);
        sideLayout->addWidget(servo1);
    }
    if (servo2_enable) {
        servo2 = new Shutters(servo2_name,servo2_channel,servo2_on,servo2_off,servo2_scan_on,servo2_scan_off,servo2_scan_on_time,servo2_scan_off_time);
        QObject::connect(servo2, &Shutters::error,
                         this, &MainWindow::statusShow);
        sideLayout->addWidget(servo2);
    }
    if (servo3_enable) {
        servo3 = new Shutters(servo3_name,servo3_channel,servo3_on,servo3_off,servo3_scan_on,servo3_scan_off,servo3_scan_on_time,servo3_scan_off_time);
        QObject::connect(servo3, &Shutters::error,
                         this, &MainWindow::statusShow);
        sideLayout->addWidget(servo3);
    }

    sideLayout->addStretch(-1);
    mainLayout->addLayout(sideLayout);
    mainLayout->setStretch(0,3);
    mainLayout->setStretch(1,1);
    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);
}

void MainWindow::readConfig()
{
    QSettings settings(config_dir, QSettings::IniFormat);

    settings.beginGroup("wavemeter");
    AUTO_CONFIG(wm_channel,Int);
    AUTO_CONFIG(wm_channel_min,Int);
    AUTO_CONFIG(wm_channel_max,Int);
    AUTO_CONFIG(udp_port,Int);
    settings.endGroup();

    settings.beginGroup("chart");
    AUTO_CONFIG(chart_x_min,Int);
    AUTO_CONFIG(chart_x_max,Int);
    AUTO_CONFIG(chart_x_tick_counts,Int);
    AUTO_CONFIG(chart_y_min,Int);
    AUTO_CONFIG(chart_y_max,Int);
    settings.endGroup();

    settings.beginGroup("font");
    AUTO_CONFIG(title_font_family,String);
    AUTO_CONFIG(title_font_size,Int);
    settings.endGroup();

    settings.beginGroup("analog output");
    AUTO_CONFIG(ao_enable,Bool);
    AUTO_CONFIG(ao_device,String);
    AUTO_CONFIG(ao_decimals,Int);
    AUTO_CONFIG(ao_min,Real);
    AUTO_CONFIG(ao_max,Real);
    AUTO_CONFIG(ao_value,Real);
    settings.endGroup();

    settings.beginGroup("pid");
    AUTO_CONFIG(p_parameter,Real);
    AUTO_CONFIG(i_parameter,Real);
    AUTO_CONFIG(freq_setpoint,Real);
    AUTO_CONFIG(freq_max_error,Real);
    AUTO_CONFIG(feedback_max,Real);
    AUTO_CONFIG(feedback_min,Real);
    AUTO_CONFIG(pending_time,Int);
    settings.endGroup();

    settings.beginGroup("shutters");
    AUTO_CONFIG(arduino_port,String);
    AUTO_CONFIG(servo1_enable,Bool);
    AUTO_CONFIG(servo1_name,String);
    AUTO_CONFIG(servo1_channel,String);
    AUTO_CONFIG(servo1_on,String);
    AUTO_CONFIG(servo1_off,String);
    AUTO_CONFIG(servo1_scan_on,String);
    AUTO_CONFIG(servo1_scan_off,String);
    AUTO_CONFIG(servo1_scan_on_time,String);
    AUTO_CONFIG(servo1_scan_off_time,String);
    AUTO_CONFIG(servo2_enable,Bool);
    AUTO_CONFIG(servo2_name,String);
    AUTO_CONFIG(servo2_channel,String);
    AUTO_CONFIG(servo2_on,String);
    AUTO_CONFIG(servo2_off,String);
    AUTO_CONFIG(servo2_scan_on,String);
    AUTO_CONFIG(servo2_scan_off,String);
    AUTO_CONFIG(servo2_scan_on_time,String);
    AUTO_CONFIG(servo2_scan_off_time,String);
    AUTO_CONFIG(servo3_enable,Bool);
    AUTO_CONFIG(servo3_name,String);
    AUTO_CONFIG(servo3_channel,String);
    AUTO_CONFIG(servo3_on,String);
    AUTO_CONFIG(servo3_off,String);
    AUTO_CONFIG(servo3_scan_on,String);
    AUTO_CONFIG(servo3_scan_off,String);
    AUTO_CONFIG(servo3_scan_on_time,String);
    AUTO_CONFIG(servo3_scan_off_time,String);

    statusBar()->showMessage(QString("Loaded config.ini"));
}

void MainWindow::writeConfig()
{
    QSettings settings(config_dir, QSettings::IniFormat);

    settings.beginGroup("wavemeter");
    AUTO_SAVE_CONFIG(wm_channel);
    AUTO_SAVE_CONFIG(wm_channel_min);
    AUTO_SAVE_CONFIG(wm_channel_max);
    AUTO_SAVE_CONFIG(udp_port);
    settings.endGroup();

    settings.beginGroup("chart");
    AUTO_SAVE_CONFIG(chart_x_min);
    AUTO_SAVE_CONFIG(chart_x_max);
    AUTO_SAVE_CONFIG(chart_x_tick_counts);
    AUTO_SAVE_CONFIG(chart_y_min);
    AUTO_SAVE_CONFIG(chart_y_max);
    settings.endGroup();

    settings.beginGroup("font");
    AUTO_SAVE_CONFIG(title_font_family);
    AUTO_SAVE_CONFIG(title_font_size);
    settings.endGroup();

    settings.beginGroup("analog output");
    AUTO_SAVE_CONFIG(ao_device);
    AUTO_SAVE_CONFIG(ao_decimals);
    AUTO_SAVE_CONFIG(ao_min);
    AUTO_SAVE_CONFIG(ao_max);
    AUTO_SAVE_CONFIG(ao_value);
    settings.endGroup();

    settings.beginGroup("pid");
    AUTO_SAVE_CONFIG(p_parameter);
    AUTO_SAVE_CONFIG(i_parameter);
    AUTO_SAVE_CONFIG(freq_setpoint);
    AUTO_SAVE_CONFIG(freq_max_error);
    settings.endGroup();

    statusBar()->showMessage(QString("Loaded config.ini"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeConfig();
    event->accept();
}

void MainWindow::statusShow(const QString &s)
{
    statusBar()->showMessage(s);
}
