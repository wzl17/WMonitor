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
    QObject::connect(udpSocket, &UdpSocket::status,
                     this, &MainWindow::statusShow);
    chartView = new ChartView;
    QObject::connect(udpSocket, &QUdpSocket::readyRead,
                     chartView, [=](){
        while (udpSocket->hasPendingDatagrams()) {
            json = new JsonData;
            json->loadWavemeterData(udpSocket->receiveDatagram().data());
            chartView->series->replace(json->wm_pattern);
            chartView->chart->setTitle(QString("Frequency:%1 THz").arg(json->wm_freq,0,'f',5));
            if ( abs(json->wm_freq - laser1.setpoint ) <= abs(json->wm_freq - laser2.setpoint ) )
                emit chartView->freqChanged1(json->wm_freq);
            else emit chartView->freqChanged2(json->wm_freq);
            delete json;
        }
    });
    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(chartView);
    sideLayout = new QVBoxLayout;
    sideLayout->addWidget(udpSocket->groupWidget);

    if (ao1_enable) {
        laserControl1 = new LaserControl(&laser1);
        QObject::connect(chartView, &ChartView::freqChanged1,
                         laserControl1, &LaserControl::voltFeedback);
        QObject::connect(laserControl1, &LaserControl::error,
                         this, &MainWindow::statusShow);
        sideLayout->addWidget(laserControl1->groupWidget);
    }
    if (ao2_enable) {
        laserControl2 = new LaserControl(&laser2);
        QObject::connect(chartView, &ChartView::freqChanged2,
                         laserControl2, &LaserControl::voltFeedback);
        QObject::connect(laserControl2, &LaserControl::error,
                         this, &MainWindow::statusShow);
        sideLayout->addWidget(laserControl2->groupWidget);
    }
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

    settings.beginGroup("laser1");
    AUTO_CONFIG(ao1_enable,Bool);
    laser1.name = settings.value("name1").toString();
    laser1.device = settings.value("ao1_device").toString();
    laser1.decimals = settings.value("ao1_decimals").toInt();
    laser1.stepsize = settings.value("ao1_stepsize").toReal();
    laser1.min = settings.value("ao1_min").toReal();
    laser1.max = settings.value("ao1_max").toReal();
    laser1.value = settings.value("ao1_value").toReal();
    laser1.p = settings.value("fb1_p").toReal();
    laser1.i = settings.value("fb1_i").toReal();
    laser1.setpoint = settings.value("fb1_setpoint").toReal();
    laser1.maxerr = settings.value("fb1_maxerr").toReal();
    laser1.fb_min = settings.value("fb1_min").toReal();
    laser1.fb_max = settings.value("fb1_max").toReal();
    laser1.fb_pending = settings.value("fb1_pending").toReal();
    settings.endGroup();

    settings.beginGroup("laser2");
    AUTO_CONFIG(ao2_enable,Bool);
    laser2.name = settings.value("name2").toString();
    laser2.device = settings.value("ao2_device").toString();
    laser2.decimals = settings.value("ao2_decimals").toInt();
    laser2.stepsize = settings.value("ao2_stepsize").toReal();
    laser2.min = settings.value("ao2_min").toReal();
    laser2.max = settings.value("ao2_max").toReal();
    laser2.value = settings.value("ao2_value").toReal();
    laser2.p = settings.value("fb2_p").toReal();
    laser2.i = settings.value("fb2_i").toReal();
    laser2.setpoint = settings.value("fb2_setpoint").toReal();
    laser2.maxerr = settings.value("fb2_maxerr").toReal();
    laser2.fb_min = settings.value("fb2_min").toReal();
    laser2.fb_max = settings.value("fb2_max").toReal();
    laser2.fb_pending = settings.value("fb2_pending").toReal();
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

    settings.beginGroup("laser1");
    settings.setValue("ao1_value",laser1.value);
    settings.setValue("fb1_p",laser1.p);
    settings.setValue("fb1_i",laser1.i);
    settings.setValue("fb1_setpoint",laser1.setpoint);
    settings.setValue("fb1_maxerr",laser1.maxerr);
    settings.endGroup();

    settings.beginGroup("laser2");
    settings.setValue("ao2_value",laser2.value);
    settings.setValue("fb2_p",laser2.p);
    settings.setValue("fb2_i",laser2.i);
    settings.setValue("fb2_setpoint",laser2.setpoint);
    settings.setValue("fb2_maxerr",laser2.maxerr);
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
