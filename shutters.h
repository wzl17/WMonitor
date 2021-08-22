#ifndef SHUTTERS_H
#define SHUTTERS_H

#include "globals.h"
#include "buttons.h"
#include <QSerialPort>
#include <QMutex>

class ShutterControl : public QGroupBox
{
    Q_OBJECT

public:
    ShutterControl(Shutter *shutter, QWidget *parent = nullptr);
    virtual ~ShutterControl();

    OnOffButtons *onoff_buttons;
    StartStopButtons *scan_buttons;

signals:
    void error(const QString &s);

private:
    void initShutter();
    QSerialPort *serial;
    Shutter *m_shutter;
    QVBoxLayout *boxLayout;

private slots:
    void shutterON();
    void shutterOFF();
    void shutterScanON();
    void shutterScanOFF();
    void readStatus();
};

#endif // SHUTTERS_H
