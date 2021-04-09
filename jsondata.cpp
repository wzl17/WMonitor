#include "jsondata.h"

#include <QJsonDocument>

JsonData::JsonData()
    : wm_pattern({})
    , wm_freq(-1)
{

}

JsonData::~JsonData()
{
}

void JsonData::loadWavemeterData(const QByteArray &data)
{
    m_ch = qBound(wm_channel_min, wm_channel, wm_channel_max) - 1;
    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray tempArray = doc[m_ch]["PatternData"].toArray();
    for ( qint32 i = 0; i < tempArray.size(); i++ ) {
        wm_pattern.append( QPointF( i, tempArray[i].toDouble() ) );
    }
    wm_freq = doc[m_ch]["Frequency"].toDouble();
}

