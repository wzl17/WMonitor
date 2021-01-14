#ifndef UDPBUTTONS_H
#define UDPBUTTONS_H

#include <QGroupBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class UdpButtons : public QGroupBox
{
    Q_OBJECT

public:
    UdpButtons(const QString &title, QWidget *parent = nullptr);
    virtual ~UdpButtons();

    QPushButton *startButton;
    QPushButton *stopButton;
private:
    /// @brief Initiation of control buttons
    void initButtons();
};

#endif // UDPBUTTONS_H
