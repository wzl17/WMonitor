#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    initChart();
    this->setCentralWidget(chartView);
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
    // chart->createDefaultAxes();

}

void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(9898,QUdpSocket::ShareAddress);
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
           json = new JsonData;
           json->setData(datagram.data());
           series->replace(json->pattern);
           chart->setTitle(QString("Frequency:%1 THz").arg(json->freq,0,'f',5));
           delete json;
       }
    }
    statusBar()->showMessage(tr("Package received."));
}
