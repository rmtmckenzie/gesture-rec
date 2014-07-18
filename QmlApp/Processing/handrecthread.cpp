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
    filter.setBackground(frame);
}

void HandRecThread::process(){
    cMat filtered, tosend;
    std::vector<cv::Point> contour;

    frame = cam.update();

    filtered = filter.filter(frame);

    contour = parser.parse(filtered);

    switch(settings.stage()){
    case 4:
        tosend = filter.getBackDiff(filtered);
        cv::cvtColor(tosend,tosend,CV_GRAY2RGB);
        break;
    case 3:
        //View the Background Image
        tosend = filter.getBackground();
        break;
    case 2:
        tosend = frame.clone();
        parser.drawHand(tosend);
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
