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
    explicit LaserControl(QWidget *parent = nullptr);
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
    QLabel *deviceLabel;
    QLineEdit *deviceInput;
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

    QLabel *regLabel;
    QCheckBox *regStatus;
    QLabel *shutterLabel;
    Buttons *shutterButtons;

    QFormLayout *controlBoxGrid;

    qint32 feedback_counter;
    qreal err_sum;

signals:
    void error(QString &s);

public slots:
    /// @brief Change the analog output device for laser control
    void changeDevice();
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
    /// @brief Feedback;
    void voltFeedback(const qreal &freq);
};

#endif // LASERCONTROL_H
