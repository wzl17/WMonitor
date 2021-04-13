#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGlobal>
#include <QWidget>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#define AUTO_CONFIG(NAME,TYPE) NAME=settings.value(#NAME).to##TYPE();
#define AUTO_SAVE_CONFIG(NAME) settings.setValue(#NAME,NAME);

extern struct LaserCtrl
{
    QString name;
    QString device;
    quint8 decimals;
    qreal stepsize;
    qreal min;
    qreal max;
    qreal value;
    qreal p;
    qreal i;
    qreal setpoint;
    qreal maxerr;
    qreal fb_max;
    qreal fb_min;
    qint16 fb_pending;
} laser1,laser2;

extern qint16 wm_channel;
extern qint16 wm_channel_min;
extern qint16 wm_channel_max;
extern qint32 udp_port;

extern qint16 chart_x_min;
extern qint16 chart_x_max;
extern qint8 chart_x_tick_counts;
extern qint16 chart_y_min;
extern qint16 chart_y_max;

extern QString title_font_family;
extern qint16 title_font_size;

extern bool ao1_enable;
extern bool ao2_enable;

extern QString arduino_port;
extern bool servo1_enable;
extern QString servo1_name;
extern QString servo1_channel;
extern QString servo1_on;
extern QString servo1_off;
extern QString servo1_scan_on;
extern QString servo1_scan_off;
extern QString servo1_scan_on_time;
extern QString servo1_scan_off_time;
extern bool servo2_enable;
extern QString servo2_name;
extern QString servo2_channel;
extern QString servo2_on;
extern QString servo2_off;
extern QString servo2_scan_on;
extern QString servo2_scan_off;
extern QString servo2_scan_on_time;
extern QString servo2_scan_off_time;
extern bool servo3_enable;
extern QString servo3_name;
extern QString servo3_channel;
extern QString servo3_on;
extern QString servo3_off;
extern QString servo3_scan_on;
extern QString servo3_scan_off;
extern QString servo3_scan_on_time;
extern QString servo3_scan_off_time;

#endif // GLOBALS_H
