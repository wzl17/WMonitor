#include "udpsocket.h"

UdpSocket::UdpSocket(QObject *parent)
    : QUdpSocket(parent)
{
    initConfigBox();
    initSocket();
}

UdpSocket::~UdpSocket()
{
    delete channelBox;
    delete channelLabel;
    delete channelForm;
    delete channelWidget;
    delete buttons;
    delete groupLayout;
    delete groupWidget;
}

void UdpSocket::initSocket()
{
    this->bind(udp_port,QUdpSocket::ShareAddress);
    this->open(QIODevice::ReadOnly);
}

void UdpSocket::initConfigBox()
{
    groupWidget = new QGroupBox(tr("UDP connection"));

    channelLabel = new QLabel(tr("WM Channel:"));
    channelBox = new QSpinBox;
    channelBox->setRange(1,9);
    channelBox->setSingleStep(1);
    QObject::connect(channelBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, &UdpSocket::switchChannel);
    channelBox->setValue(wm_channel);
    channelForm = new QFormLayout;
    channelForm->addRow(channelLabel,channelBox);
    channelWidget = new QWidget;
    channelWidget->setLayout(channelForm);

    buttons = new Buttons(tr(""));
    QObject::connect(buttons->startButton, &QPushButton::clicked,
                     this, &UdpSocket::startUDP);
    QObject::connect(buttons->stopButton, &QPushButton::clicked,
                     this, &UdpSocket::stopUDP);

    groupLayout = new QVBoxLayout;
    groupLayout->addWidget(channelWidget);
    groupLayout->addWidget(buttons);
    groupWidget->setLayout(groupLayout);

}

void UdpSocket::switchChannel(qint16 newchannel)
{
    wm_channel = newchannel;
}

void UdpSocket::startUDP()
{
    if (!this->isOpen()) {
        this->open(QIODevice::ReadOnly);
        qDebug() << "UDP Started.";
        this->bind(udp_port,QUdpSocket::ShareAddress);
    }
}

void UdpSocket::stopUDP()
{
    if (this->isOpen()) {
        qDebug() << "UDP Stopped.";
        this->close();
    }
}
