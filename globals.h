#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGlobal>
#include <QMultiMap>
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

typedef struct
{
    bool ao_enable;
    QString name;
    quint32 wm_channel;
    QString device;
    quint32 decimals;
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
    quint32 fb_pending;
    qreal plot_range;
} Laser;

typedef struct
{
    bool servo_enable;
    QString name;
    QString channel;
    QString on_name;
    QString off_name;
    QString on_command;
    QString off_command;
    QString scan_on_command;
    QString scan_off_command;
    QString scan_on_time;
    QString scan_off_time;
} Shutter;

extern QVector<Laser> lasers;
extern QMultiMap<qint16, qint16> channels_lasers;
extern QMap<qint16, qreal> channels_freqs;
extern QList<qint16> channels;
extern qint16 pattern_channel;
extern QVector<Shutter> shutters;

extern qint16 laser_num;
extern qint16 shutter_num;
extern QString shutters_arduino_port;

extern qint16 wm_channel_min;
extern qint16 wm_channel_max;
extern qint32 udp_port;

extern qint16 pattern_x_min;
extern qint16 pattern_x_max;
extern qint8 pattern_x_tick_counts;
extern qint16 pattern_y_min;
extern qint16 pattern_y_max;
extern qint16 freqplot_x_length;

extern QString pattern_title_font_family;
extern qint16 pattern_title_font_size;

#endif // GLOBALS_H
