#include "globals.h"

//default settings of parameters

qint16 wm_channel = 2;
qint16 wm_channel_min = 1;
qint16 wm_channel_max = 9;
qint32 udp_port = 9898;

qint16 chart_x_min = 0;
qint16 chart_x_max = 2048;
qint8 chart_x_tick_counts = 5;
qint16 chart_y_min = 0;
qint16 chart_y_max = 4000;

QString title_font_family = "Times";
qint16 title_font_size = 30;

bool ao_enable = true;
QString ao_device = "Dev1/ao0";
qint8 ao_decimals = 3;
qreal ao_stepsize = 0.001;
qreal ao_min = 0.000;
qreal ao_max = 10.000;
qreal ao_value = 0.000;

qreal p_parameter = 10.0;
qreal i_parameter = 0.0;
qreal freq_setpoint = 751.52643;
qreal freq_max_error = 0.00001;
qreal feedback_max = 0.01;
qreal feedback_min = -0.01;
qint16 pending_time = 5;

QString arduino_port = "COM3";
bool servo1_enable = true;
QString servo1_name = "370and400";
QString servo1_channel = "servo00";
QString servo1_on = "A";
QString servo1_off = "B";
QString servo1_scan_on = "C";
QString servo1_scan_off = "D";
QString servo1_scan_on_time = "3";
QString servo1_scan_off_time = "3";
bool servo2_enable = false;
QString servo2_name = "NA";
QString servo2_channel = "servo01";
QString servo2_on = "A";
QString servo2_off = "B";
QString servo2_scan_on = "C";
QString servo2_scan_off = "D";
QString servo2_scan_on_time = "3";
QString servo2_scan_off_time = "3";
bool servo3_enable = false;
QString servo3_name = "NA";
QString servo3_channel = "servo02";
QString servo3_on = "A";
QString servo3_off = "B";
QString servo3_scan_on = "C";
QString servo3_scan_off = "D";
QString servo3_scan_on_time = "3";
QString servo3_scan_off_time = "3";
