#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QObject>
#include <QValueAxis>
#include <QLabel>
#include <QSpinBox>
#include <QDockWidget>
#include <QFormLayout>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QNetworkDatagram>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    mainLayout = new QHBoxLayout;

    udpSocket = new UdpSocket;
    udpSocket->initConfigBox();
    udpSocket->initSocket();

    chartView = new ChartView;
    chartView->initChart();

    QObject::connect(udpSocket, &QUdpSocket::readyRead,
                     chartView, [=](){
        while (udpSocket->hasPendingDatagrams()) {
            json = new JsonData(udpSocket->WM_CHANNEL);
            json->setData(udpSocket->receiveDatagram().data());
            chartView->series->replace(json->pattern);
            chartView->chart->setTitle(QString("Frequency:%1 THz").arg(json->freq,0,'f',5));
            delete json;
        }
    });

    laserControl = new LaserControl;
    laserControl->initDevice();
    laserControl->initControlBox();

    mainLayout->addWidget(chartView);

    sideLayout = new QVBoxLayout;
    sideLayout->addWidget(udpSocket->configBoxGroup);
    sideLayout->addWidget(laserControl->controlBoxGroup);
    sideLayout->addStretch(-1);
    sideLayout->addWidget(udpSocket->buttons);
    mainLayout->addLayout(sideLayout);

    mainLayout->setStretch(0,3);
    mainLayout->setStretch(1,1);
    mainWidget->setLayout(mainLayout);
}
