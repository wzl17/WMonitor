#include "lasercontrol.h"

LaserControl::LaserControl(LaserCtrl *laser, QWidget *parent)
    : QWidget(parent)
    , ptr(laser)
{
    initDevice();
    initControlBox();
}

LaserControl::~LaserControl()
{
    stopDevice();
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
    groupWidget = new QGroupBox(ptr->name);

    voltageLabel = new QLabel(tr("Voltage:"));
    voltageInput = new QDoubleSpinBox;
    voltageInput->setDecimals(ptr->decimals);
    voltageInput->setSingleStep(ptr->stepsize);
    voltageInput->setRange(ptr->min, ptr->max);
    voltageInput->setSuffix(" V");
    voltageInput->setValue(ptr->value);
    QObject::connect(voltageInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     this, &LaserControl::changeVoltage);

    pLabel = new QLabel(tr("P(reg):"));
    pInput = new QLineEdit;
    pInput->setText(QString::number(ptr->p,'f',2));
    QObject::connect(pInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeP);
    iLabel = new QLabel(tr("I(reg):"));
    iInput = new QLineEdit;
    iInput->setText(QString::number(ptr->i,'f',2));
    QObject::connect(iInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeI);
    setpLabel = new QLabel(tr("Setpoint:"));
    setpInput = new QLineEdit;
    setpInput->setText(QString::number(ptr->setpoint,'f',5));
    QObject::connect(setpInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeSetP);
    merrLabel = new QLabel(tr("MaxError:"));
    merrInput = new QLineEdit;
    merrInput->setText(QString::number(ptr->maxerr,'f',5));
    QObject::connect(merrInput, &QLineEdit::editingFinished,
                     this, &LaserControl::changeMaxErr);

    regLabel = new QLabel(tr("regulation:"));
    regStatus = new QCheckBox(tr("Locked"));
    regStatus->setChecked(false);
    feedback_counter = 0;
    err_sum = 0;

    controlBoxGrid = new QFormLayout;
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
                    ptr->device.toUtf8(),
                    "",
                    ptr->min,
                    ptr->max,
                    DAQmx_Val_Volts,
                    NULL));
    DAQmxStartTask(taskHandle);
}

void LaserControl::changeVoltage()
{
    ptr->value = voltageInput->value();
    qDebug() << DAQmxWriteAnalogScalarF64(
                taskHandle,
                false,
                0,
                ptr->value,
                NULL);
}

void LaserControl::changeP()
{
    ptr->p = pInput->text().toDouble();
}

void LaserControl::changeI()
{
    ptr->i = iInput->text().toDouble();
}

void LaserControl::changeSetP()
{
    ptr->setpoint = setpInput->text().toDouble();
}

void LaserControl::changeMaxErr()
{
    ptr->maxerr = merrInput->text().toDouble();
}

void LaserControl::voltFeedback(const qreal &freq)
{
    if (!regStatus->isChecked()) {
        feedback_counter = 0;
        err_sum = 0.0;
        offset = ptr->value;
        return;
    }
    if (freq <=0) { // invalid wavelength
        regStatus->setChecked(false);
        return;
    }
    qreal f_err = freq - ptr->setpoint;
    // for error over maximum error, unchecked lock state and return
    if (f_err >= ptr->maxerr) {
        regStatus->setChecked(false);
        return;
    }
    feedback_counter++;
    err_sum += f_err;
    // only do the feedback when the error is stable for given time
    if (feedback_counter >= ptr->fb_pending) {
        voltageInput->setValue( offset + qBound(ptr->fb_min,
                                      err_sum * ptr->i + f_err*ptr->p,
                                      ptr->fb_max) );
    }
    //emit feedbackDone();
    return;
}
