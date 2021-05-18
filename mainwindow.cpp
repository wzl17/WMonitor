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

    qint16 i = 0;

    // laser freq plot
    laserFreqTabs = new QTabWidget;
    for (i = 0; i < lasers.size(); ++i) {
        if (lasers.at(i).ao_enable) {
            new_freqplot = new LaserFreqPlot(&lasers[i]);
            QObject::connect(this, &MainWindow::freqReceived,
                             new_freqplot, &LaserFreqPlot::updateFreq);
            laserFreqTabs->addTab(new_freqplot, lasers.at(i).name);
        }
    }

    // NI card based laser control
    for (i = 0; i < lasers.size(); ++i) {
        if (lasers.at(i).ao_enable) {
            new_laserctrl = new LaserControl(&lasers[i]);
            QObject::connect(this, &MainWindow::freqReceived,
                             new_laserctrl, &LaserControl::voltFeedback);
            QObject::connect(new_laserctrl, &LaserControl::error,
                             this, &MainWindow::statusShow);
            sideLayout->addWidget(new_laserctrl->groupWidget);
        }
    }

    // servo shutter control
    for (i = 0; i < shutters.size(); ++i) {
        if (shutters.at(i).servo_enable) {
            new_shutterctrl = new ShutterControl(&shutters[i]);
            QObject::connect(new_shutterctrl, &ShutterControl::error,
                             this, &MainWindow::statusShow);
            sideLayout->addWidget(new_shutterctrl);
        }
    }

    mainLayout->addWidget(laserFreqTabs);
    mainLayout->addWidget(pattern);
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
    AUTO_CONFIG(shutter_num,Int);
    AUTO_CONFIG(shutters_arduino_port,String);
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

    qint16 i = 0;

    for (i = 0; i < laser_num; ++i) {
        settings.beginGroup(QString("laser%1").arg(i));
        lasers.append( (Laser){
            settings.value("ao_enable").toBool(),
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

    for (i = 0; i < shutter_num; ++i) {
        settings.beginGroup(QString("shutter%1").arg(i));
        shutters.append( (Shutter){
            settings.value("servo_enable").toBool(),
            settings.value("name").toString(),
            settings.value("channel").toString(),
            settings.value("on_name").toString(),
            settings.value("off_name").toString(),
            settings.value("on_command").toString(),
            settings.value("off_command").toString(),
            settings.value("scan_on_command").toString(),
            settings.value("scan_off_command").toString(),
            settings.value("scan_on_time").toString(),
            settings.value("scan_off_time").toString()}  );
        settings.endGroup();
    }

    statusBar()->showMessage(QString("Loaded config.ini"));
}

void MainWindow::writeConfig()
{
    QSettings settings(config_dir, QSettings::IniFormat);

    qint16 i = 0;

    for (i = 0; i < lasers.size(); ++i) {
        settings.beginGroup(QString("laser%1").arg(i));
        settings.setValue("ao_value",lasers[i].value);
        settings.setValue("fb_p",lasers[i].p);
        settings.setValue("fb_i",lasers[i].i);
        settings.setValue("fb_setpoint",lasers[i].setpoint);
        settings.setValue("fb_maxerr",lasers[i].maxerr);
        settings.endGroup();
    }
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
