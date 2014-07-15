#include "handrecthread.hpp"
#include <QApplication>
#include "opencvvideobuffer.hpp"

#include "../handrecapi.hpp"

#include <QDebug>
#include <QThread>
HandRecThread::HandRecThread(QObject* parent):
    QObject(parent),
    cam(this),
    filter(this),
    parser(this),
    recognizer(this),
    settings(&cam,&filter,&parser,&recognizer,this),
    keepRunning(true)
{
    connect(this,&HandRecThread::_stop,this,&HandRecThread::quit);
}

void HandRecThread::run()
{
    qDebug() << "Hand Recognition Started!";

    cam.init();

    startTimer(0);
}

void HandRecThread::timerEvent(QTimerEvent *)
{
    process();
}

void HandRecThread::TakeBackgroundImage()
{
    qDebug("INto function");
    cv::Mat frame;
    frame = cam.update();
    backgroundImage = frame.clone();
}

void HandRecThread::process(){
    cv::Mat frame, filtered, tosend;
    frame = cam.update();

    filtered = filter.filter(frame);

    switch(settings.stage()){
    case 2:
        tosend = backgroundImage;
        break;
    case 1:
        cv::cvtColor(filtered,tosend,CV_GRAY2RGB);
        break;
    case 0:
    default:
        tosend = frame.clone();
    }

    OpenCVVideoBuffer* vidbuf = new OpenCVVideoBuffer(tosend);
    QVideoFrame qframe(vidbuf,QSize(tosend.cols,tosend.rows),OPENCV_PIXEL_FORMAT);
    emit updateFrame(qframe);
}

void HandRecThread::stop()
{
    emit _stop();
}
