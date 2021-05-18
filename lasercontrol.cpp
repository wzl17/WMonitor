#include "lasercontrol.h"

LaserControl::LaserControl(Laser *laser, QWidget *parent)
    : QWidget(parent)
    , m_laser(laser)
{
    initDevice();
    initControlBox();
}

LaserControl::~LaserControl()
{
    stopDevice();
    delete groupWidget;
}

void LaserControl::initDevice()
{
    stopDevice();
    startDevice();
}

void LaserControl::initControlBox()
{
    groupWidget = new QGroupBox(m_laser->name);

    voltageLabel = new QLabel(tr("Voltage:"));
    voltageInput = new QDoubleSpinBox;
    voltageInput->setDecimals(m_laser->decimals);
    voltageInput->setSingleStep(m_laser->stepsize);
    voltageInput->setRange(m_laser->min, m_laser->max);
    voltageInput->setSuffix(" V");
    voltageInput->setValue(m_laser->value);
    QObject::connect(voltageInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &LaserControl::changeVoltage);

    pLabel = new QLabel(tr("P(reg):"));
    pInput = new QLineEdit;
    pInput->setText(QString::number(m_laser->p,'f',2));
    QObject::connect(pInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeP);
    iLabel = new QLabel(tr("I(reg):"));
    iInput = new QLineEdit;
    iInput->setText(QString::number(m_laser->i,'f',2));
    QObject::connect(iInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeI);
    setpLabel = new QLabel(tr("Setpoint:"));
    setpInput = new QLineEdit;
    setpInput->setText(QString::number(m_laser->setpoint,'f',5));
    QObject::connect(setpInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeSetP);
    merrLabel = new QLabel(tr("MaxError:"));
    merrInput = new QLineEdit;
    merrInput->setText(QString::number(m_laser->maxerr,'f',5));
    QObject::connect(merrInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeMaxErr);

    regStatus = new QPushButton(tr("Lock State"));
    regStatus->setStyleSheet("QPushButton {background-color: white}");
    regSwitch = new QCheckBox(tr("Regulation"));
    regSwitch->setChecked(false);
    feedback_counter = 0;
    err_sum = 0.0;
    QObject::connect(regSwitch, &QCheckBox::clicked,
                     this, &LaserControl::changeOffset);

    controlBoxGrid = new QFormLayout;
    controlBoxGrid->addRow(voltageLabel, voltageInput);
    controlBoxGrid->addRow(setpLabel, setpInput);
    controlBoxGrid->addRow(merrLabel, merrInput);
    controlBoxGrid->addRow(pLabel, pInput);
    controlBoxGrid->addRow(iLabel, iInput);
    controlBoxGrid->addRow(regSwitch, regStatus);
    groupWidget->setLayout(controlBoxGrid);
}

void LaserControl::stopDevice()
{
    if (taskHandle) {
        DAQmxStopTask(taskHandle);
        DAQmxClearTask(taskHandle);
        taskHandle = 0;
    }
}

void LaserControl::startDevice()
{
    DAQmxCreateTask("", &taskHandle);
    qDebug() << QString("Analog output start with code %1.")
                .arg(DAQmxCreateAOVoltageChan(
                    taskHandle,
                    m_laser->device.toUtf8(),
                    "",
                    m_laser->min,
                    m_laser->max,
                    DAQmx_Val_Volts,
                    NULL));
    DAQmxStartTask(taskHandle);
}

void LaserControl::changeVoltage()
{
    m_laser->value = voltageInput->value();
    qDebug() << DAQmxWriteAnalogScalarF64(
                taskHandle,
                false,
                0,
                m_laser->value,
                NULL);
}

void LaserControl::changeP()
{
    m_laser->p = pInput->text().toDouble();
}

void LaserControl::changeI()
{
    m_laser->i = iInput->text().toDouble();
}

void LaserControl::changeSetP()
{
    m_laser->setpoint = setpInput->text().toDouble();
}

void LaserControl::changeMaxErr()
{
    m_laser->maxerr = merrInput->text().toDouble();
}

void LaserControl::changeOffset()
{
    if (regSwitch->isChecked()) {
        if (abs(channels_freqs[m_laser->wm_channel] - m_laser->setpoint) > m_laser->maxerr) { //out of range
            regSwitch->setChecked(false);
        } else offset = m_laser->value;
    } else regStatus->setStyleSheet("QPushButton {background-color: white}");
}

void LaserControl::voltFeedback()
{
    qreal f_err = channels_freqs[m_laser->wm_channel] - m_laser->setpoint;
    if (!regSwitch->isChecked()) { // Regulation off
        return;
    }
    else {
        if (abs(f_err) < m_laser->maxerr) { //counts valid data
            feedback_counter++;
            err_sum += f_err;
            regStatus->setStyleSheet("QPushButton {background-color: green}");
            regStatus->update();
        }
        else { //not locked or showing wrong freq, change lock state and clear counts
            regStatus->setStyleSheet("QPushButton {background-color: white}");
            regStatus->update();
            feedback_counter = 0;
            err_sum = 0.0;
        }
        // only do the feedback when the error is stable for given time
        if (feedback_counter >= m_laser->fb_pending) {
            voltageInput->setValue( offset + qBound(m_laser->fb_min,
                                    err_sum * m_laser->i + f_err*m_laser->p,
                                    m_laser->fb_max) );
        }
    }
    return;
}
