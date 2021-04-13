#include "globals.h"

//default settings of parameters

LaserCtrl laser1 = {
    "399nm",
    "Dev1/ao0",
    3,
    0.001,
    0,
    10,
    0.078,
    10.0,
    0.0,
    751.52643,
    0.00001,
    -0.01,
    0.01,
    5
};
LaserCtrl laser2 = {
    "369nm",
    "Dev1/ao4",
    3,
    0.001,
    0,
    10,
    0.078,
    10.0,
    0.0,
    811.29120,
    0.00001,
    -0.01,
    0.01,
    5
};

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

bool ao1_enable = true;
bool ao2_enable = false;

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
