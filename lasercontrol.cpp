#include "lasercontrol.h"

LaserControl::LaserControl(QWidget *parent)
    : QWidget(parent)
{
    initDevice();
    initControlBox();
}

LaserControl::~LaserControl()
{
    stopDevice();
    delete deviceLabel;
    delete deviceInput;
    delete voltageLabel;
    delete voltageInput;
    delete controlBoxGrid;
    delete groupWidget;
}

void LaserControl::initDevice()
{
    stopDevice();
    startDevice();
}

void LaserControl::initControlBox()
{
    groupWidget = new QGroupBox(tr("Laser Control"));

    deviceLabel = new QLabel(tr("Device Name:"));
    deviceInput = new QLineEdit;
    deviceInput->setText(ao_device);
    QObject::connect(deviceInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeDevice);

    voltageLabel = new QLabel(tr("Voltage:"));
    voltageInput = new QDoubleSpinBox;
    voltageInput->setDecimals(ao_decimals);
    voltageInput->setSingleStep(ao_stepsize);
    voltageInput->setRange(ao_min, ao_max);
    voltageInput->setSuffix(" V");
    voltageInput->setValue(ao_value);
    QObject::connect(voltageInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &LaserControl::changeVoltage);

    pLabel = new QLabel(tr("P(reg):"));
    pInput = new QLineEdit;
    pInput->setText(QString::number(p_parameter,'f',2));
    QObject::connect(pInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeP);
    iLabel = new QLabel(tr("I(reg):"));
    iInput = new QLineEdit;
    iInput->setText(QString::number(i_parameter,'f',2));
    QObject::connect(iInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeI);
    setpLabel = new QLabel(tr("400 Setpoint:"));
    setpInput = new QLineEdit;
    setpInput->setText(QString::number(freq_setpoint,'f',5));
    QObject::connect(setpInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeSetP);
    merrLabel = new QLabel(tr("400 MaxError:"));
    merrInput = new QLineEdit;
    merrInput->setText(QString::number(freq_max_error,'f',5));
    QObject::connect(merrInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeMaxErr);

    regLabel = new QLabel(tr("400regulation:"));
    regStatus = new QCheckBox(tr("Locked"));
    regStatus->setChecked(false);
    feedback_counter = 0;
    err_sum = 0;

    controlBoxGrid = new QFormLayout;
    controlBoxGrid->addRow(deviceLabel, deviceInput);
    controlBoxGrid->addRow(voltageLabel, voltageInput);
    controlBoxGrid->addRow(setpLabel, setpInput);
    controlBoxGrid->addRow(merrLabel, merrInput);
    controlBoxGrid->addRow(pLabel, pInput);
    controlBoxGrid->addRow(iLabel, iInput);
    controlBoxGrid->addRow(regLabel, regStatus);
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
                    ao_device.toUtf8(),
                    "",
                    ao_min,
                    ao_max,
                    DAQmx_Val_Volts,
                    NULL));
    DAQmxStartTask(taskHandle);
}

void LaserControl::changeDevice()
{
    stopDevice();
    ao_device = deviceInput->text();
    startDevice();
}

void LaserControl::changeVoltage()
{
    ao_value = voltageInput->value();
    qDebug() << DAQmxWriteAnalogScalarF64(
                taskHandle,
                false,
                0,
                ao_value,
                NULL);
}

void LaserControl::changeP()
{
    p_parameter = pInput->text().toDouble();
}

void LaserControl::changeI()
{
    i_parameter = iInput->text().toDouble();
}

void LaserControl::changeSetP()
{
    freq_setpoint = setpInput->text().toDouble();
}

void LaserControl::changeMaxErr()
{
    freq_max_error = merrInput->text().toDouble();
}

void LaserControl::voltFeedback(const qreal &freq)
{
    if (!regStatus->isChecked()) {
        feedback_counter = 0;
        err_sum = 0.0;
        return;
    }
    if (freq <=0) {
        regStatus->setChecked(false);
        return;
    }
    qreal f_err = freq - freq_setpoint;
    // for error over maximum error, unchecked lock state and return
    if (f_err >= freq_max_error) {
        regStatus->setChecked(false);
        return;
    }
    feedback_counter++;
    err_sum += f_err;
    // only do the feedback when the error is stable for given time
    if (feedback_counter >= pending_time) {
        voltageInput->setValue(qBound(feedback_min,err_sum*i_parameter+f_err*p_parameter,feedback_max)+ao_value);
    }
}
