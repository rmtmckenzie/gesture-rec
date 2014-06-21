#ifndef HANDRECPRIVATE_HPP
#define HANDRECPRIVATE_HPP

#include <QObject>
#include "opencvcamerasource.hpp"

#include <QVideoFrame>

#define OPENCV_PIXEL_FORMAT QVideoFrame::Format_BGR24

class HandRecPrivate : public QObject
{
    Q_OBJECT
public:
    explicit HandRecPrivate(int cam_number, QObject *parent = 0);

    void run();

private:
    OpenCVCameraSource cam;
signals:

    void updateFrame(QVideoFrame f);

public slots:

};

#endif // HANDRECPRIVATE_HPP
