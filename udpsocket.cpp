#include "udpsocket.h"

UdpSocket::UdpSocket(QObject *parent)
    : QUdpSocket(parent)
{
}

UdpSocket::~UdpSocket()
{
    delete buttons;
    delete channelBox;
    delete portBox;
    delete channelLabel;
    delete portLabel;
    delete configForm;
    delete configBoxGroup;
}

void UdpSocket::initSocket()
{
    this->bind(UDP_PORT,QUdpSocket::ShareAddress);
    this->open(QIODevice::ReadOnly);

    buttons = new ControlButtons(tr("Control"));
    QObject::connect(buttons->startButton, &QPushButton::clicked,
                     this, &UdpSocket::startUDP);
    QObject::connect(buttons->stopButton, &QPushButton::clicked,
                     this, &UdpSocket::stopUDP);
}

void UdpSocket::initConfigBox()
{
    configBoxGroup = new QGroupBox(tr("Config"));

    channelLabel = new QLabel(tr("WM Channel:"));
    channelBox = new QSpinBox;
    channelBox->setRange(1,9);
    channelBox->setSingleStep(1);
    QObject::connect(channelBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, &UdpSocket::switchChannel);
    channelBox->setValue(1);

    portLabel = new QLabel(tr("UDP Port:"));
    portBox = new QLineEdit;
    portBox->setText("9898");
    QObject::connect(portBox, &QLineEdit::editingFinished,
                     this, &UdpSocket::switchPort);

    configForm = new QFormLayout;
    configForm->addRow(channelLabel,channelBox);
    configForm->addRow(portLabel,portBox);
    configBoxGroup->setLayout(configForm);
}

void UdpSocket::switchChannel(qint16 newchannel)
{
    WM_CHANNEL = newchannel;
}

void UdpSocket::switchPort()
{
    UDP_PORT = portBox->text().toInt();
    if (this->isOpen()) {
        this->close();
        this->open(QIODevice::ReadOnly);
    }
    this->bind(UDP_PORT,QUdpSocket::ShareAddress);
    qDebug() << UDP_PORT;
}

void UdpSocket::startUDP()
{
    if (!this->isOpen()) {
        this->open(QIODevice::ReadOnly);
        qDebug() << "UDP Started.";
        this->bind(UDP_PORT,QUdpSocket::ShareAddress);
    }
}

void UdpSocket::stopUDP()
{
    if (this->isOpen()) {
        qDebug() << "UDP Stopped.";
        this->close();
    }
}
