#include "handrecprivate.hpp"
#include <QApplication>
#include "opencvvideobuffer.hpp"

#include <QDebug>
#include <QThread>
HandRecPrivate::HandRecPrivate(int cam_number, QObject* parent):
    QObject(parent),
    cam(cam_number)
{
}

void HandRecPrivate::run()
{
    qDebug() << "Hand Recognition Started!";
    qDebug() << "Private: " << QThread::currentThreadId();
    cv::Mat frame;
    for(;;){
        //wait for at most 50 ms
        QApplication::processEvents(QEventLoop::AllEvents,50);

        frame = cam.update();
        qDebug() << "Frame Received!";

        OpenCVVideoBuffer* vidbuf = new OpenCVVideoBuffer(frame);
        QVideoFrame qframe(vidbuf,QSize(frame.cols,frame.rows),OPENCV_PIXEL_FORMAT);
        emit updateFrame(qframe);

    }
}
