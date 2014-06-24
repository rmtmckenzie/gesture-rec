#ifndef HANDRECPRIVATE_HPP
#define HANDRECPRIVATE_HPP

#include <QObject>
#include "opencvcamerasource.hpp"

#include <QVideoFrame>

#define OPENCV_PIXEL_FORMAT QVideoFrame::Format_RGB24

class HandRecPrivate : public QObject
{
    Q_OBJECT
public:
    explicit HandRecPrivate(int cam_number, QObject *parent = 0);

    void run();

    void stop();

private:
    OpenCVCameraSource cam;
    bool keepRunning;

signals:
    void updateFrame(QVideoFrame f);
    void _stop();

private slots:
    void quit(){keepRunning = false;}
};

#endif // HANDRECPRIVATE_HPP
