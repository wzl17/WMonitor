#include "globals.h"

qint16 wm_channel = 2;
qint16 wm_channel_min = 1;
qint16 wm_channel_max = 9;

qint16 chart_x_min = 0;
qint16 chart_x_max = 2048;
qint8 chart_x_tick_counts = 5;
qint16 chart_y_min = 0;
qint16 chart_y_max = 4000;

QString title_font_family = "Times";
qint16 title_font_size = 30;

qint32 udp_port = 9898;
QString ao_device = "Dev1/ao0";
qint8 ao_decimals = 3;
qreal ao_stepsize = 0.001;
qreal ao_min = 0.000;
qreal ao_max = 10.000;
qreal ao_value = 0.000;
