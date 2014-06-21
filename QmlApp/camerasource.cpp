#include "camerasource.hpp"
#include <QCameraImageCapture>

CameraSource::CameraSource(VideoSurface *surf, QObject *parent) :
    QObject(parent),
    camera(0),
    status(CameraSource::STATUS_UNINITIALIZED),
    surface(surf)
{
}

void CameraSource::init(QByteArray cam){
    if(cam.isEmpty()){
        //find the camera
        cam = findCamera();

    }

    setCamera(cam);

    //Capture Still Supported
    qDebug("Capture Viewfinder %ssupported",(camera->isCaptureModeSupported(QCamera::CaptureViewfinder) ? "" : "not "));
    qDebug("Capture Image %ssupported",(camera->isCaptureModeSupported(QCamera::CaptureStillImage) ? "" : "not "));
    qDebug("Capture Video %ssupported",(camera->isCaptureModeSupported(QCamera::CaptureVideo) ? "" : "not "));

    if(camera->isCaptureModeSupported(QCamera::CaptureVideo)){
        camera->setCaptureMode(QCamera::CaptureVideo);
    } else {
        qFatal("Video capture not supported by webcam.");
    }

    camera->setViewfinder(surface);
}

CameraSource::~CameraSource()
{
    if(camera && camera->state() == QCamera::ActiveState){
        camera->stop();
        camera->deleteLater();
    }
}

void CameraSource::start()
{
    if(!camera)return;

    camera->start();

    if(camera->state() != QCamera::ActiveState){
        qDebug() << "Camera not started?";
    }
}

void CameraSource::stop()
{
    camera->stop();
    setStatus(CameraSource::STATUS_READY);
}

void CameraSource::setCamera(QByteArray camLocation){
    if(camera)
        camera->deleteLater();

    if(camLocation.isEmpty()){
        qFatal("Empty location passed to camera setter..");
    }

    camera = new QCamera(camLocation,this);
    processing = camera->imageProcessing();
    focus = camera->focus();
    exposure = camera->exposure();

    setStatus(CameraSource::STATUS_READY);
}

void CameraSource::setFocus(QCameraFocus::FocusMode f)
{
    if(focus && focus->isAvailable()){
        focus->setFocusMode(f);
    }
}

void CameraSource::setExposureComp(qreal ec)
{
    if(exposure && exposure->isAvailable()){

    }
}

QByteArray CameraSource::findCamera()
{
    QByteArray cam;
    QList<QByteArray> devs = QCamera::availableDevices();

    if(devs.length() == 0){
        qFatal("No webcams/cameras found!");
    }

    foreach(const QByteArray &loc,devs){
        qDebug() << loc << QCamera::deviceDescription(loc);
        if(QCamera::deviceDescription(loc).contains("LifeCam",Qt::CaseInsensitive)){
            cam = loc;
        }
    }

    if(cam.isEmpty()){
        qDebug() << "No LifeCam Found, using first camera.";
        cam = QCamera::availableDevices()[0];
    } else {
        qDebug() << "LifeCam Found!";
    }

    return cam;
}
