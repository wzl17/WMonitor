#ifndef SHUTTERS_H
#define SHUTTERS_H

#include "globals.h"
#include "buttons.h"
#include <QSerialPort>
#include <QMutex>

class Shutters : public QGroupBox
{
    Q_OBJECT

public:
    Shutters(const QString &name,
             const QString &channel,
             const QString &on,
             const QString &off,
             const QString &scan_on,
             const QString &scan_off,
             const QString &scan_on_time,
             const QString &scan_off_time,
             QWidget *parent = nullptr);
    virtual ~Shutters();

    OnOffButtons *onoff_buttons;
    StartStopButtons *scan_buttons;

signals:
    void error(const QString &s);

private:
    void initShutter();

    QString m_channel;
    QString m_on;
    QString m_off;
    QString m_scan_on;
    QString m_scan_off;
    QString m_scan_on_time;
    QString m_scan_off_time;
    QVBoxLayout *boxLayout;

private slots:
    void shutterON();
    void shutterOFF();
    void shutterScanON();
    void shutterScanOFF();
};

#endif // SHUTTERS_H
