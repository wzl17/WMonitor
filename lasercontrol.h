#ifndef LASERCONTROL_H
#define LASERCONTROL_H

#include "globals.h"
#include "buttons.h"
#include <stdio.h>
#include <NIDAQmx.h>

class LaserControl : public QWidget
{
    Q_OBJECT

public:
    LaserControl(LaserCtrl *laser, QWidget *parent = nullptr);
    ~LaserControl();

    QGroupBox *groupWidget;

private:
    /// @brief Initiation of analog output control via certain channel
    void initDevice();
    /// @brief Initiation of control box
    void initControlBox();
    /// @brief Stop analog output task
    void stopDevice();
    /// @brief Start analog output task
    void startDevice();

    TaskHandle taskHandle = 0;
    LaserCtrl *ptr;

    QLabel *voltageLabel;
    QDoubleSpinBox *voltageInput;

    QLabel *pLabel;
    QLineEdit *pInput;
    QLabel *iLabel;
    QLineEdit *iInput;
    QLabel *setpLabel;
    QLineEdit *setpInput;
    QLabel *merrLabel;
    QLineEdit *merrInput;

    QPushButton *regStatus;
    QCheckBox *regSwitch;
    QLabel *shutterLabel;
    StartStopButtons *shutterButtons;

    QFormLayout *controlBoxGrid;

    quint32 feedback_counter;
    qreal err_sum;
    qreal offset;

signals:
    void error(QString &s);
    void feedbackDone();

public slots:
    /// @brief Change analog output voltage
    void changeVoltage();
    /// @brief Change P parameter
    void changeP();
    /// @brief Change I parameter
    void changeI();
    /// @brief Change setpoint
    void changeSetP();
    /// @brief Change max error
    void changeMaxErr();
    /// @brief Change analog output voltage
    void changeOffset();
    /// @brief Feedback;
    void voltFeedback();
};

#endif // LASERCONTROL_H
