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
    delete buttons;
    delete gridLayout;
}

void Shutters::initShutter() {
    gridLayout = new QGridLayout;

    buttons = new Buttons(tr(""));
    QObject::connect(buttons->startButton, &QPushButton::clicked,
                     this, &Shutters::shutterON);
    QObject::connect(buttons->stopButton, &QPushButton::clicked,
                     this, &Shutters::shutterOFF);
    gridLayout->addWidget(buttons);
    this->setLayout(gridLayout);
}

void Shutters::shutterON() {
    QSerialPort serial;
    serial.close();
    serial.setPortName(arduino_port);
    if (!serial.open(QIODevice::WriteOnly)) {
        emit error( tr("Can't open %1, error code %2")
                   .arg(arduino_port).arg(serial.error()) );
        return;
    }
    const QByteArray command = (m_channel + " to " + m_on).toUtf8();
    qDebug() << command;
    serial.write(command);
    serial.close();
}

void Shutters::shutterOFF() {
    QSerialPort serial;
    serial.close();
    serial.setPortName(arduino_port);
    if (!serial.open(QIODevice::WriteOnly)) {
        emit error( tr("Can't open %1, error code %2")
                   .arg(arduino_port).arg(serial.error()) );
        return;
    }
    const QByteArray command = (m_channel + " to " + m_off).toUtf8();
    qDebug() << command;
    serial.write(command);
    serial.close();
}

void Shutters::shutterScanON() {
    QSerialPort serial;
    serial.close();
    serial.setPortName(arduino_port);
    if (!serial.open(QIODevice::WriteOnly)) {
        emit error( tr("Can't open %1, error code %2")
                   .arg(arduino_port).arg(serial.error()) );
        return;
    }
    const QByteArray command = (m_channel + " to " + m_scan_on).toUtf8();
    qDebug() << command;
    serial.write(command);
    serial.close();
}

void Shutters::shutterScanOFF() {
    QSerialPort serial;
    serial.close();
    serial.setPortName(arduino_port);
    if (!serial.open(QIODevice::WriteOnly)) {
        emit error( tr("Can't open %1, error code %2")
                   .arg(arduino_port).arg(serial.error()) );
        return;
    }
    const QByteArray command = (m_channel + " to " + m_scan_off).toUtf8();
    qDebug() << command;
    serial.write(command);
    serial.close();
}
