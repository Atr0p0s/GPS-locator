#ifndef DrawGPS_H
#define DrawGPS_H

#include <QObject>
#include <QPointer>
#include <QTextStream>
#include <QFile>

class SaveGPS : public QObject
{
    Q_OBJECT

public:
    explicit SaveGPS(QObject* parent = nullptr);
    ~SaveGPS();

public Q_SLOTS:
    void openFile(const QUrl& fileUrl);
    void closeFile();
    void write(const QString& timestamp, const QString& lat, const QString& lng);
//    void write(const QString& data);

Q_SIGNALS:
    void error(const QString &message);

private:
//    QPointer<QTextStream> stream;
    QTextStream* stream;
    QPointer<QFile> file;
};

#endif // SaveGPS_H
