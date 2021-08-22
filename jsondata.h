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

    /// @brief json string data input
    void loadWavemeterData(const QByteArray &data);

private:
    qint16 m_ch;
};

#endif // JSONDATA_H
