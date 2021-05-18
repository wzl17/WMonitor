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
    mainLayout = new QVBoxLayout;
    sideLayout = new QHBoxLayout;

    // udp connection to the wavemeter
    udpSocket = new UdpSocket;
    QObject::connect(udpSocket, &UdpSocket::status,
                     this, &MainWindow::statusShow);
    sideLayout->addWidget(udpSocket->groupWidget);

    // wavemeter pattern display
    pattern = new PatternChart;
    QObject::connect(udpSocket, &QUdpSocket::readyRead,
                     this, [=](){
        while (udpSocket->hasPendingDatagrams()) {
            json = new JsonData;
            json->loadWavemeterData(udpSocket->receiveDatagram().data());
            pattern->series->replace(json->wm_pattern);
            pattern->chart->setTitle(QString("Frequency:%1 THz").arg(channels_freqs[pattern_channel],0,'f',5));
            emit freqReceived(); // this triggers pid at lasercontrols
            delete json;
        }
    });

    // laser freq plot
    laserFreqTabs = new QTabWidget;
    for (i = 0; i < lasers.size(); ++i) {
        if (lasers.at(i).ao_enable) {
            new_freqplot = new LaserFreqPlot(&lasers[i]);
            laserFreqTabs->addTab(new_freqplot, lasers.at(i).name);
            QObject::connect(this, &MainWindow::freqReceived,
                             new_freqplot, &LaserFreqPlot::updateFreq);
        }
    }
    mainLayout->addWidget(laserFreqTabs);
    mainLayout->addWidget(pattern);

    for (i = 0; i < lasers.size(); ++i) {
        if (lasers.at(i).ao_enable) {
            new_ctrl = new LaserControl(&lasers[i]);
            QObject::connect(this, &MainWindow::freqReceived,
                             new_ctrl, &LaserControl::voltFeedback);
            QObject::connect(new_ctrl, &LaserControl::error,
                             this, &MainWindow::statusShow);
            sideLayout->addWidget(new_ctrl->groupWidget);
        }
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
    mainLayout->setStretch(0,1);
    mainLayout->setStretch(1,1);
    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);
}

void MainWindow::readConfig()
{
    QSettings settings(config_dir, QSettings::IniFormat);

    settings.beginGroup("global");
    AUTO_CONFIG(laser_num,Int);
    settings.endGroup();

    settings.beginGroup("wavemeter");
    AUTO_CONFIG(wm_channel_min,Int);
    AUTO_CONFIG(wm_channel_max,Int);
    AUTO_CONFIG(udp_port,Int);
    settings.endGroup();

    settings.beginGroup("chart");
    AUTO_CONFIG(pattern_x_min,Int);
    AUTO_CONFIG(pattern_x_max,Int);
    AUTO_CONFIG(pattern_x_tick_counts,Int);
    AUTO_CONFIG(pattern_y_min,Int);
    AUTO_CONFIG(pattern_y_max,Int);
    AUTO_CONFIG(freqplot_x_length,Int);
    settings.endGroup();

    settings.beginGroup("font");
    AUTO_CONFIG(pattern_title_font_family,String);
    AUTO_CONFIG(pattern_title_font_size,Int);
    settings.endGroup();

    for (i = 0; i < laser_num; ++i) {
        settings.beginGroup(QString("laser%1").arg(i));
        lasers.append( (LaserCtrl){settings.value("ao_enable").toBool(),
            settings.value("name").toString(),
            settings.value("wm_channel").toUInt(),
            settings.value("ao_device").toString(),
            settings.value("ao_decimals").toUInt(),
            settings.value("ao_stepsize").toReal(),
            settings.value("ao_min").toReal(),
            settings.value("ao_max").toReal(),
            settings.value("ao_value").toReal(),
            settings.value("fb_p").toReal(),
            settings.value("fb_i").toReal(),
            settings.value("fb_setpoint").toReal(),
            settings.value("fb_maxerr").toReal(),
            settings.value("fb_min").toReal(),
            settings.value("fb_max").toReal(),
            settings.value("fb_pending").toUInt(),
            settings.value("plot_range").toReal()}  );
        channels_lasers.insert(settings.value("wm_channel").toUInt(),i);
        settings.endGroup();
    }
    qDebug() << channels_lasers;
    channels = channels_lasers.uniqueKeys();

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
    AUTO_SAVE_CONFIG(wm_channel_min);
    AUTO_SAVE_CONFIG(wm_channel_max);
    AUTO_SAVE_CONFIG(udp_port);
    settings.endGroup();

    settings.beginGroup("chart");
    AUTO_SAVE_CONFIG(pattern_x_min);
    AUTO_SAVE_CONFIG(pattern_x_max);
    AUTO_SAVE_CONFIG(pattern_x_tick_counts);
    AUTO_SAVE_CONFIG(pattern_y_min);
    AUTO_SAVE_CONFIG(pattern_y_max);
    AUTO_SAVE_CONFIG(freqplot_x_length);
    settings.endGroup();

    settings.beginGroup("font");
    AUTO_SAVE_CONFIG(pattern_title_font_family);
    AUTO_SAVE_CONFIG(pattern_title_font_size);
    settings.endGroup();

    for (i = 0; i < lasers.size(); ++i) {
        settings.beginGroup(QString("laser%1").arg(i));
        settings.setValue("ao_value",lasers[i].value);
        settings.setValue("fb_p",lasers[i].p);
        settings.setValue("fb_i",lasers[i].i);
        settings.setValue("fb_setpoint",lasers[i].setpoint);
        settings.setValue("fb_maxerr",lasers[i].maxerr);
        settings.endGroup();
    }

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
