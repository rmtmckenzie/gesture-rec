#ifndef CAMERASOURCE_HPP
#define CAMERASOURCE_HPP

#include <QObject>
#include <QCamera>
#include <QCameraImageProcessing>
#include <QVideoProbe>
#include "videosurface.hpp"

class CameraSource : public QObject
{
    Q_OBJECT
public:
    explicit CameraSource(VideoSurface* surf, QObject *parent = 0);
    ~CameraSource();

    void start();
private:
    QCamera* camera;
    QCameraImageProcessing* processing;
    QCameraExposure* exposure;
    QCameraFocus* focus;
    VideoSurface* surface;

    void setCamera(QByteArray camLocation);
    QByteArray findCamera();
signals:

public slots:
};

#endif // CAMERASOURCE_HPP
