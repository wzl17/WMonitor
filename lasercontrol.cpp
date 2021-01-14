#include "lasercontrol.h"

#include <QDebug>
#include <QObject>

LaserControl::LaserControl(QWidget *parent)
    : QWidget(parent)
{
}

LaserControl::~LaserControl()
{
    stopDevice();
}

void LaserControl::initDevice()
{
    stopDevice();
    startDevice();
}

void LaserControl::initControlBox()
{
    controlBoxGroup = new QGroupBox(tr("Laser Control"));

    deviceLabel = new QLabel(tr("Device Name:"));
    deviceInput = new QLineEdit;
    deviceInput->setText(device);
    QObject::connect(deviceInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeDevice);

    voltageLabel = new QLabel(tr("Voltage:"));
    voltageInput = new QDoubleSpinBox;
    voltageInput->setDecimals(3);
    voltageInput->setSingleStep(0.01);
    voltageInput->setRange(0.0, 10.0);
    voltageInput->setSuffix(" V");
    voltageInput->setValue(0.000);
    QObject::connect(voltageInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &LaserControl::changeVoltage);

    controlBoxGrid = new QGridLayout;
    controlBoxGrid->addWidget(deviceLabel, 0, 0);
    controlBoxGrid->addWidget(deviceInput, 0, 1);
    controlBoxGrid->addWidget(voltageLabel, 1, 0);
    controlBoxGrid->addWidget(voltageInput, 1, 1);
    controlBoxGroup->setLayout(controlBoxGrid);
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
                    device.toUtf8(),
                    "",
                    0.0,
                    10.0,
                    DAQmx_Val_Volts,
                    NULL));
    DAQmxStartTask(taskHandle);
}

void LaserControl::changeDevice()
{
    stopDevice();
    device = deviceInput->text();
    startDevice();
}

void LaserControl::changeVoltage()
{
    qDebug() << DAQmxWriteAnalogScalarF64(
                taskHandle,
                false,
                0,
                voltageInput->value(),
                NULL);
}
