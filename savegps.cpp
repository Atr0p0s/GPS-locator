//#include <QQmlEngine>
#include <QQmlFile>
//#include <QFile>
//#include <QDesktopServices>
//#include <QDebug>

#include "savegps.h"

SaveGPS::SaveGPS(QObject* parent)
    : QObject{parent}
{
}

SaveGPS::~SaveGPS()
{
    if (file) {
        delete stream;
    }
}

void SaveGPS::openFile(const QUrl& fileUrl)
{
    const QString filePath = QQmlFile::urlToLocalFileOrQrc(fileUrl);
//    const QString filePath = fileUrl.toLocalFile();
//    QFile file(filePath);
    file = new QFile(filePath);
    if (!file->open(QIODevice::WriteOnly | QIODeviceBase::Append)) {
        emit error("Cannot save: " + file->errorString());
        return;
    }

    stream = new QTextStream(file);
    stream->setRealNumberPrecision(13);
}

void SaveGPS::closeFile()
{
    if (file) {
        delete stream;
        file->close();
        delete file;
    }
}

void SaveGPS::write(const QString& timestamp, const QString& latStr, const QString& lngStr)
//void SaveGPS::write(const QString& data)
{
    bool ok;
    double lat = latStr.toDouble(&ok);
    double lng = lngStr.toDouble(&ok);
    QByteArray time = timestamp.split(' ')[3].toUtf8();
    QList<QByteArray> timeList = time.split(':');
    if (!(ok && timeList.size() == 3)) {
        return;
    }
    QString timeFormatted = QString("%1%2%3.00").arg(timeList[0], 2, '0').arg(timeList[1], 2, '0').arg(timeList[2], 2, '0');

    uint lat_abs = abs(lat);
    double lat_min = (abs(lat) - lat_abs) * 60;
    char lat_pol = (lat > 0)? 'N' : 'S';
    QString latFormatted = QString("%1%2").arg(lat_abs, 2, 10, QChar('0')).arg(lat_min, -8, 'g', 7, '0');

    uint lng_abs = abs(lng);
    double lng_min = (abs(lng) - lng_abs) * 60;
    char lng_pol = (lng > 0)? 'E' : 'W';
    QString lngFormatted = QString("%1%2").arg(lng_abs, 3, 10, QChar('0')).arg(lng_min, -8, 'g', 7, '0');

    QByteArray full = "$GPGGA,"+timeFormatted.toUtf8()+','+latFormatted.toUtf8()+','+lat_pol+','+\
                        lngFormatted.toUtf8()+','+lng_pol+",1,01,1.0,0.0,M,0.0,M,,*";
    int crc = 0;
    for (qsizetype i = 1; i < full.size()-1; ++i) {
        crc = crc ^ full.at(i);
    }
    crc = crc & 0xFF;

    full.append(QString::asprintf("%.2X", crc).toUtf8());
    full.append("\r\n");
    *stream << full;
}
