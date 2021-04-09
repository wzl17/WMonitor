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

    controlBoxGrid = new QGridLayout;
    controlBoxGrid->addWidget(deviceLabel, 0, 0);
    controlBoxGrid->addWidget(deviceInput, 0, 1);
    controlBoxGrid->addWidget(voltageLabel, 1, 0);
    controlBoxGrid->addWidget(voltageInput, 1, 1);
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
