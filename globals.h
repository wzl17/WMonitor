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
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#define AUTO_CONFIG(NAME,TYPE) NAME=settings.value(#NAME).to##TYPE();
#define AUTO_SAVE_CONFIG(NAME) settings.setValue(#NAME,NAME);

extern qint16 wm_channel;
extern qint16 wm_channel_min;
extern qint16 wm_channel_max;

extern qint16 chart_x_min;
extern qint16 chart_x_max;
extern qint8 chart_x_tick_counts;
extern qint16 chart_y_min;
extern qint16 chart_y_max;

extern QString title_font_family;
extern qint16 title_font_size;

extern qint32 udp_port;
extern QString ao_device;
extern qint8 ao_decimals;
extern qreal ao_stepsize;
extern qreal ao_min;
extern qreal ao_max;
extern qreal ao_value;

#endif // GLOBALS_H
