#include "jsondata.h"

#include <QJsonDocument>

JsonData::JsonData()
    : wm_pattern({})
{
    wm_pattern.reserve(pattern_x_max+1);
}

JsonData::~JsonData()
{
}

void JsonData::loadWavemeterData(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray tempArray = doc[pattern_channel-1]["PatternData"].toArray();
    for ( qint32 i = 0; i < tempArray.size(); i++ ) {
        wm_pattern.append( QPointF( i, tempArray[i].toDouble() ) );
    }
    for (qint16 i = 0; i < channels.size(); ++i) { // update the freq of all listed channels
        m_ch = qBound(wm_channel_min, channels[i], wm_channel_max);
        channels_freqs[m_ch] = doc[m_ch-1]["Frequency"].toDouble();
    }
}

