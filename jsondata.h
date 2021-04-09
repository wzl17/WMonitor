#ifndef JSONDATA_H
#define JSONDATA_H

#include "globals.h"
#include <QJsonArray>
#include <QList>
#include <QPointF>
#include <QByteArray>

class JsonData
{

public:
    JsonData();
    virtual ~JsonData();

    /// @brief wavemeter pattern data
    QList<QPointF> wm_pattern;
    /// @brief frequency value read from wavemeter channel
    qreal wm_freq;

    /// @brief json string data input
    void loadWavemeterData(const QByteArray &data);

private:
    qint8 m_ch;
};

#endif // JSONDATA_H
