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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chart(new QChart)
    , series(new QLineSeries)
{
    ui->setupUi(this);

    initUI();
    createStatus();
    initSocket();
}

MainWindow::~MainWindow()
{
    if (udpSocket->isOpen()) {
        udpSocket->close();
    }

    delete ui;
}

void MainWindow::initUI()
{
    mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    mainLayout = new QHBoxLayout;

    initChart();
    mainLayout->addWidget(chartView);

    sideLayout = new QVBoxLayout;
    initConfigBox();
    sideLayout->addWidget(configBoxGroup);
    sideLayout->addStretch(-1);
    initButtons();
    sideLayout->addWidget(controlGroup);

    mainLayout->addLayout(sideLayout);

    mainLayout->setStretch(0,3);
    mainLayout->setStretch(1,1);
    mainWidget->setLayout(mainLayout);
}

void MainWindow::initChart()
{
    chart->addSeries(series);

    // ----------- X Axis -----------
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 2048);
    axisX->setLabelFormat("%g");
    axisX->setTickCount(5);
    axisX->setTitleText("");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // ----------- Y Axis -----------
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 4000);
    axisY->setLabelFormat("%g");
    axisY->setTitleText("");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->setTitle(QString("Frequency:"));
    chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::initConfigBox()
{
    configBoxGroup = new QGroupBox(tr("Config"));

    QLabel *channelLabel = new QLabel(tr("WM Channel:"));
    channelBox = new QSpinBox;
    channelBox->setRange(1,9);
    channelBox->setSingleStep(1);
    QObject::connect(channelBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, &MainWindow::switchChannel);
    channelBox->setValue(1);

    QLabel *portLabel = new QLabel(tr("UDP Port:"));
    portBox = new QLineEdit;
    portBox->setText("9898");
    QObject::connect(portBox, &QLineEdit::editingFinished,
                     this, &MainWindow::switchPort);

    QFormLayout *configForm = new QFormLayout;
    configForm->addRow(channelLabel,channelBox);
    configForm->addRow(portLabel,portBox);
    configBoxGroup->setLayout(configForm);
}

void MainWindow::initButtons()
{
    controlGroup = new QGroupBox(tr("Control"));

    startButton = new QPushButton(tr("Start"));
    QObject::connect(startButton, &QPushButton::clicked,
                     this, &MainWindow::startUDP);
    stopButton = new QPushButton(tr("Stop"));
    QObject::connect(stopButton, &QPushButton::clicked,
                     this, &MainWindow::stopUDP);

    QGridLayout *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(startButton, 0, 0);
    buttonGrid->addWidget(stopButton, 0, 1);
    controlGroup->setLayout(buttonGrid);
}

void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(UDP_PORT,QUdpSocket::ShareAddress);
    qDebug() << udpSocket->openMode();
    if (udpSocket->state() == udpSocket->BoundState) {
        statusBar()->showMessage(tr("Waiting package..."));
    }

    QObject::connect(udpSocket, &QUdpSocket::readyRead,
                     this, &MainWindow::onReceiveUDP);
}

void MainWindow::createStatus()
{
    statusBar()->showMessage(tr("Ready."));
}

void MainWindow::onReceiveUDP()
{
    while (udpSocket->hasPendingDatagrams()) {
       statusBar()->showMessage(tr("Package received."));
       datagram = udpSocket->receiveDatagram();
       if (datagram.isValid()) {
           statusBar()->showMessage(tr("Updating..."));
           json = new JsonData(WM_CHANNEL);
           json->setData(datagram.data());
           series->replace(json->pattern);
           chart->setTitle(QString("Frequency:%1 THz").arg(json->freq,0,'f',5));
           delete json;
       }
    }
    statusBar()->showMessage(tr("Package received."));
}

void MainWindow::switchChannel(qint16 newchannel)
{
    WM_CHANNEL = newchannel;
}

void MainWindow::switchPort()
{
    UDP_PORT = portBox->text().toInt();
    if (udpSocket->isOpen()) {
        udpSocket->close();
        udpSocket->open(QIODevice::ReadOnly);
    }
    udpSocket->bind(UDP_PORT,QUdpSocket::ShareAddress);
    qDebug() << UDP_PORT;
}

void MainWindow::startUDP()
{
    if (!udpSocket->isOpen()) {
        udpSocket->open(QIODevice::ReadOnly);
        udpSocket->bind(UDP_PORT,QUdpSocket::ShareAddress);
        if (udpSocket->state() == udpSocket->BoundState) {
            statusBar()->showMessage(tr("Waiting package..."));
        }
    }
}

void MainWindow::stopUDP()
{
    if (udpSocket->isOpen()) {
        qDebug() << "UDP Stopped.";
        udpSocket->close();
        if (udpSocket->state() != udpSocket->BoundState) {
            statusBar()->showMessage(tr("Connection stopped."));
        }
    }
}
