
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "savegps.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<SaveGPS>("com.myinc.SaveGPS", 1, 0, "SaveGPS");

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GPS-locator", "Main");

    return app.exec();
}
