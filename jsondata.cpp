#include "jsondata.h"

#include <QTextStream>
#include <QIODevice>
#include <QJsonDocument>
#include <QDebug>

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

//void JsonData::processJson()
//{
///// test by reading from file
//    QFile file("D:/Onedrive/Codes/Qt_projects/WavelengthMonitor/udp_log_0.json");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//            return;

//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        data = in.readLine();
//    }
//    QJsonDocument doc = QJsonDocument::fromJson(data);

//    tempArray = doc[6]["PatternData"].toArray();
//    for ( qint32 i = 0; i < tempArray.size(); i++ ) {
//        pattern.append( QPointF( i, tempArray[i].toDouble() ) );
//    }

//    freq = doc[6]["Frequency"].toDouble();
///// TODO:
////    if (!doc.isObject()) {
////        // handle parse error...
////    }

//}
