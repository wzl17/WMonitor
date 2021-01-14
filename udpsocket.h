#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "udpbuttons.h"

#include <QUdpSocket>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

class UdpSocket : public QUdpSocket
{
    Q_OBJECT

public:
    UdpSocket(QObject *parent = nullptr);
    ~UdpSocket();

    /// @brief Initiation of config box
    void initConfigBox();
    /// @brief Initiation of UDP socket
    void initSocket();

    QGroupBox *configBoxGroup;
    UdpButtons *buttons;
    qint16 WM_CHANNEL = 1;
    qint32 UDP_PORT = 9898;

private:
    QSpinBox *channelBox;
    QLineEdit *portBox;
    QLabel *channelLabel;
    QLabel *portLabel;
    QFormLayout *configForm;

private slots:
    /// @brief Update channel according to signal
    void switchChannel(qint16 newchannel);
    /// @brief Update UDP port according to signal
    void switchPort();
    /// @brief Start UDP connection
    void startUDP();
    /// @brief Stop UDP connection
    void stopUDP();
};

#endif // UDPSOCKET_H
