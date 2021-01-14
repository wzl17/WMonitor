#ifndef JSONDATA_H
#define JSONDATA_H

#include <QString>
#include <QJsonArray>
#include <QList>
#include <QPointF>
#include <QByteArray>

class JsonData
{

public:
    JsonData(const qint16 &channel);
    virtual ~JsonData();

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
    /// @brief store channel number
    qint16 ch;
};

#endif // JSONDATA_H
