#include "qtquick2controlsapplicationviewer.h"
#include <QtQml>
#include "cameraview.hpp"
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << "Starting.";
    Application app(argc, argv);

    QtQuick2ControlsApplicationViewer viewer;
    qmlRegisterType<CameraView>("Motion.View", 1, 0, "CameraView");


    viewer.setMainQmlFile(QStringLiteral("qml/GestureCamera/main.qml"));
    viewer.show();

    return app.exec();
}
