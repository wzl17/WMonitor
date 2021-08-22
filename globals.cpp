#include "globals.h"

//default settings of parameters

QVector<Laser> lasers = {};
QMultiMap<qint16, qint16> channels_lasers = {};
QMap<qint16, qreal> channels_freqs = {};
QList<qint16> channels = {};
QVector<Shutter> shutters = {};

qint16 laser_num = 0;
qint16 shutter_num = 0;
QString shutters_arduino_port = "COM3";

qint16 wm_channel_min = 1;
qint16 wm_channel_max = 9;
qint32 udp_port = 9898;

qint16 pattern_channel = 2;
qint16 pattern_x_min = 0;
qint16 pattern_x_max = 2048;
qint8 pattern_x_tick_counts = 5;
qint16 pattern_y_min = 0;
qint16 pattern_y_max = 4000;
qint16 freqplot_x_length = 100;

QString pattern_title_font_family = "Times";
qint16 pattern_title_font_size = 20;
