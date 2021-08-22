#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "globals.h"
#include "buttons.h"

#include <QUdpSocket>

class UdpSocket : public QUdpSocket
{
    Q_OBJECT

public:
    UdpSocket(QObject *parent = nullptr);
    ~UdpSocket();

    QGroupBox *groupWidget;
    StartStopButtons *buttons;
    QWidget *channelWidget;

private:
    /// @brief Initiation of config box
    void initConfigBox();
    /// @brief Initiation of UDP socket
    void initSocket();

    QSpinBox *channelBox;
    QLabel *channelLabel;
    QFormLayout *channelForm;
    QVBoxLayout *groupLayout;

signals:
    void status(const QString &s);

private slots:
    /// @brief Update channel according to signal
    void switchChannel(qint16 newchannel);
    /// @brief Start UDP connection
    void startUDP();
    /// @brief Stop UDP connection
    void stopUDP();
};

#endif // UDPSOCKET_H
