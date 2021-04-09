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
            delete json;
        }
    });

    laserControl = new LaserControl;

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(chartView);
    sideLayout = new QVBoxLayout;
    sideLayout->addWidget(udpSocket->groupWidget);
    sideLayout->addWidget(laserControl->groupWidget);
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

    settings.beginGroup("devices");
    AUTO_CONFIG(udp_port,Int);
    AUTO_CONFIG(ao_device,String);
    AUTO_CONFIG(ao_decimals,Int);
    AUTO_CONFIG(ao_min,Real);
    AUTO_CONFIG(ao_max,Real);
    AUTO_CONFIG(ao_value,Real);
    settings.endGroup();

    statusBar()->showMessage(QString("Loaded config.ini"));
}

void MainWindow::writeConfig()
{
    QSettings settings(config_dir, QSettings::IniFormat);

    settings.beginGroup("wavemeter");
    AUTO_SAVE_CONFIG(wm_channel);
    AUTO_SAVE_CONFIG(wm_channel_min);
    AUTO_SAVE_CONFIG(wm_channel_max);
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

    settings.beginGroup("devices");
    AUTO_SAVE_CONFIG(udp_port);
    AUTO_SAVE_CONFIG(ao_device);
    AUTO_SAVE_CONFIG(ao_decimals);
    AUTO_SAVE_CONFIG(ao_min);
    AUTO_SAVE_CONFIG(ao_max);
    AUTO_SAVE_CONFIG(ao_value);
    settings.endGroup();

    statusBar()->showMessage(QString("Loaded config.ini"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeConfig();
    event->accept();
}
