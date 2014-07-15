#include "qtquick2controlsapplicationviewer.h"
#include <QtQml>
#include "cameraview.hpp"
#include "handrecapi.hpp"
#include <QDebug>

#include <QRgb>


int main(int argc, char *argv[])
{
    qDebug() << "Starting.";
    Application app(argc, argv);

    qRegisterMetaType<QRgb>("QRgb");

    QtQuick2ControlsApplicationViewer viewer;
    qmlRegisterType<CameraView>("Motion.View", 1, 0, "CameraView");
    qmlRegisterType<HandRecAPI>("Motion.HandRec",1,0, "HandRec");

    viewer.setMainQmlFile(QStringLiteral("qml/GestureCamera/main.qml"));
    viewer.show();

    return app.exec();
}
