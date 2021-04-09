#ifndef LASERCONTROL_H
#define LASERCONTROL_H

#include "globals.h"
#include <stdio.h>
#include <NIDAQmx.h>


class LaserControl : public QWidget
{
    Q_OBJECT

public:
    explicit LaserControl(QWidget *parent = nullptr);
    ~LaserControl();

    /// @brief Initiation of analog output control via certain channel
    void initDevice();
    /// @brief Initiation of control box
    void initControlBox();

    QGroupBox *groupWidget;

private:
    /// @brief Stop analog output task
    void stopDevice();
    /// @brief Start analog output task
    void startDevice();

    TaskHandle taskHandle = 0;
    QLabel *deviceLabel;
    QLineEdit *deviceInput;
    QLabel *voltageLabel;
    QDoubleSpinBox *voltageInput;
    QGridLayout *controlBoxGrid;

private slots:
    /// @brief Change the analog output device for laser control
    void changeDevice();
    /// @brief Change analog output voltage
    void changeVoltage();
};

#endif // LASERCONTROL_H
