#include "shutters.h"

Shutters::Shutters(const QString &name,
                   const QString &channel,
                   const QString &on,
                   const QString &off,
                   const QString &scan_on,
                   const QString &scan_off,
                   const QString &scan_on_time,
                   const QString &scan_off_time,
                   QWidget *parent)
    : QGroupBox(name,parent)
    , m_channel(channel)
    , m_on(on)
    , m_off(off)
    , m_scan_on(scan_on)
    , m_scan_off(scan_off)
    , m_scan_on_time(scan_on_time)
    , m_scan_off_time(scan_off_time)
{
    initShutter();
}

Shutters::~Shutters()
{
    delete onoff_buttons;
    delete scan_buttons;
    delete boxLayout;
    serial->close();
    delete serial;
}

void Shutters::initShutter() {
    boxLayout = new QVBoxLayout;

    onoff_buttons = new OnOffButtons(tr(""));
    QObject::connect(onoff_buttons->onButton, &QPushButton::clicked,
                     this, &Shutters::shutterON);
    QObject::connect(onoff_buttons->offButton, &QPushButton::clicked,
                     this, &Shutters::shutterOFF);
    boxLayout->addWidget(onoff_buttons);
    scan_buttons = new StartStopButtons(tr("Scan:"));
    QObject::connect(scan_buttons->startButton, &QPushButton::clicked,
                     this, &Shutters::shutterScanON);
    QObject::connect(scan_buttons->stopButton, &QPushButton::clicked,
                     this, &Shutters::shutterScanOFF);
    boxLayout->addWidget(scan_buttons);
    this->setLayout(boxLayout);
    serial = new QSerialPort;
    serial->close();
    serial->setPortName(arduino_port);
    if (!serial->open(QIODevice::ReadWrite)) {
        emit error( tr("Can't open %1, error code %2")
                   .arg(arduino_port).arg(serial->error()) );
    }
    QObject::connect(serial, &QSerialPort::readyRead,
                     this, &Shutters::readStatus);
}

void Shutters::shutterON() {
    const QByteArray command = (m_channel + " to " + m_on + "(0,0)\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void Shutters::shutterOFF() {
    const QByteArray command = (m_channel + " to " + m_off + "(0,0)\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void Shutters::shutterScanON() {
    const QByteArray command = (m_channel+" to "+m_scan_on+"("+m_scan_on_time+","+m_scan_off_time+")\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void Shutters::shutterScanOFF() {
    const QByteArray command = (m_channel+" to "+m_scan_off+"("+m_scan_on_time+","+m_scan_off_time+")\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void Shutters::readStatus() {
    QByteArray buf = serial->readLine();
    emit error(buf);
}
