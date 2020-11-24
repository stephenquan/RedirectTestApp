#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "NetworkAccessManager.h"
#include "NetworkReply.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<NetworkAccessManager>("ArcGIS.AppFramework.Proto", 1, 0, "NetworkAccessManager");
    qmlRegisterType<NetworkReply>("ArcGIS.AppFramework.Proto", 1, 0, "NetworkReply");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
