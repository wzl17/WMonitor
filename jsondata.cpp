#include "jsondata.h"

#include <QJsonDocument>

JsonData::JsonData(const qint16 &channel)
    : pattern({})
    , freq(-1)
    , ch(channel-1)
{

}

JsonData::~JsonData()
{
}

void JsonData::setData(const QByteArray &data)
{
    if (ch<0) {
        qDebug() << "Invalid channel number!";
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray tempArray = doc[ch]["PatternData"].toArray();
    for ( qint32 i = 0; i < tempArray.size(); i++ ) {
        pattern.append( QPointF( i, tempArray[i].toDouble() ) );
    }
    freq = doc[ch]["Frequency"].toDouble();
}

bool JsonData::isNull()
{
    if (pattern.isEmpty())
        return true;
    else return false;
}
