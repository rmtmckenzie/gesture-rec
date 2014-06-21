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
    ~CameraSource();

    static CameraSource* get(VideoSurface* surf);

    void start();


    enum STATUS{
        STATUS_UNINITIALIZED,
        STATUS_RUNNING,
        STATUS_READY,
        STATUS_ERROR
    };

    void init(QByteArray cam = "");

    CameraSource(VideoSurface* surf, QObject *parent = 0); //todo get rid of
    void stop();
    void setFocus(QCameraFocus::FocusMode f);
    void setExposureComp(qreal ec);
protected:
    inline void setStatus(STATUS s){status = s; emit(statusChanged(s));}

private:
//    CameraSource(VideoSurface* surf, QObject *parent = 0);
    STATUS status;
    QCamera* camera;
    QCameraImageProcessing* processing;
    QCameraExposure* exposure;
    QCameraFocus* focus;
    VideoSurface* surface;


    void setCamera(QByteArray camLocation);
    QByteArray findCamera();
signals:
    void statusChanged(STATUS s);

public slots:
};

Q_DECLARE_METATYPE(CameraSource::STATUS)

#endif // CAMERASOURCE_HPP
