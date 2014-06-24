#include "handrecprivate.hpp"
#include <QApplication>
#include "opencvvideobuffer.hpp"

#include <QDebug>
#include <QThread>
HandRecPrivate::HandRecPrivate(int cam_number, QObject* parent):
    QObject(parent),
    cam(cam_number),
    keepRunning(true)
{
    connect(this,&HandRecPrivate::_stop,this,&HandRecPrivate::quit);
}

void HandRecPrivate::run()
{
    qDebug() << "Hand Recognition Started!";
    qDebug() << "Private: " << QThread::currentThreadId();
    cv::Mat frame;
    while(keepRunning){

        frame = cam.update();
        OpenCVVideoBuffer* vidbuf = new OpenCVVideoBuffer(frame);
        QVideoFrame qframe(vidbuf,QSize(frame.cols,frame.rows),OPENCV_PIXEL_FORMAT);
        emit updateFrame(qframe);

        //wait for at most 50 ms
        QApplication::processEvents(QEventLoop::AllEvents,50);
    }
}

void HandRecPrivate::stop()
{
    emit _stop();
}
