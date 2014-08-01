#include <QApplication>
#include <QQmlApplicationEngine>
#include "cameraview.hpp"
#include "handrecapi.hpp"
#include <QDebug>

#include <QRgb>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<QRgb>("QRgb");
    qmlRegisterType<CameraView>("Motion.View", 1, 0, "CameraView");
    qmlRegisterType<HandRecAPI>("Motion.HandRec",1,0, "HandRec");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/GestureCamera/main.qml")));

    return app.exec();
}
