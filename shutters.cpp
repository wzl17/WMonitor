#include "shutters.h"

ShutterControl::ShutterControl(Shutter *shutter, QWidget *parent)
    : QGroupBox(parent)
    , m_shutter(shutter)
{
    this->setTitle(m_shutter->name);
    initShutter();
}

ShutterControl::~ShutterControl()
{
    delete onoff_buttons;
    delete scan_buttons;
    delete boxLayout;
    serial->close();
    delete serial;
}

void ShutterControl::initShutter() {
    boxLayout = new QVBoxLayout;

    onoff_buttons = new OnOffButtons(tr(""),m_shutter->on_name,m_shutter->off_name);
    QObject::connect(onoff_buttons->onButton, &QPushButton::clicked,
                     this, &ShutterControl::shutterON);
    QObject::connect(onoff_buttons->offButton, &QPushButton::clicked,
                     this, &ShutterControl::shutterOFF);
    boxLayout->addWidget(onoff_buttons);
    scan_buttons = new StartStopButtons(tr("Scan:"));
    QObject::connect(scan_buttons->startButton, &QPushButton::clicked,
                     this, &ShutterControl::shutterScanON);
    QObject::connect(scan_buttons->stopButton, &QPushButton::clicked,
                     this, &ShutterControl::shutterScanOFF);
    boxLayout->addWidget(scan_buttons);
    this->setLayout(boxLayout);
    serial = new QSerialPort;
    serial->close();
    serial->setPortName(shutters_arduino_port);
    if (!serial->open(QIODevice::ReadWrite)) {
        emit error( tr("Can't open %1, error code %2")
                   .arg(shutters_arduino_port).arg(serial->error()) );
    }
    QObject::connect(serial, &QSerialPort::readyRead,
                     this, &ShutterControl::readStatus);
}

void ShutterControl::shutterON() {
    const QByteArray command = (m_shutter->channel + " to " + m_shutter->on_command + "(0,0)\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void ShutterControl::shutterOFF() {
    const QByteArray command = (m_shutter->channel + " to " + m_shutter->off_command + "(0,0)\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void ShutterControl::shutterScanON() {
    const QByteArray command = (m_shutter->channel+" to "+m_shutter->scan_on_command+"("+m_shutter->scan_on_time+","+m_shutter->scan_off_time+")\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void ShutterControl::shutterScanOFF() {
    const QByteArray command = (m_shutter->channel+" to "+m_shutter->scan_off_command+"("+m_shutter->scan_on_time+","+m_shutter->scan_off_time+")\n").toUtf8();
    qDebug() << command;
    serial->write(command);
}

void ShutterControl::readStatus() {
    QByteArray buf = serial->readAll();
    emit error(buf);
}
