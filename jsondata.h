#ifndef JSONDATA_H
#define JSONDATA_H

#include <QFile>
#include <QString>
#include <QJsonArray>
#include <QtCharts/QLineSeries>
#include <QByteArray>

QT_CHARTS_USE_NAMESPACE

class JsonData
{

public:
    JsonData(const qint16 &channel);
    ~JsonData();

    /// @brief pattern data
    QList<QPointF> pattern;
    /// @brief frequency value read from channel 7
    qreal freq;
    // TODO:
    // make freq a list

    /// @brief json string data input
    void setData(const QByteArray &data);
    /// @brief whether data is loaded or not
    bool isNull(void);

private:
    // void processJson();
    /// @brief store channel number
    qint16 ch;
};

#endif // JSONDATA_H
