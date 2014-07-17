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
    cv::Mat frame;
    frame = cam.update();
    backgroundImage = frame.clone();
}

void HandRecThread::process(){
    cv::Mat frame, filtered, tosend, subtracted;
    std::vector<cv::Point> contour;
    std::vector<std::vector<cv::Point> > contours;

    frame = cam.update();

    filtered = filter.filter(frame);

    contour = parser.parse(filtered);

    contours.push_back(contour);

    cv::Mat cvtFiltered, cvtBackground;

    switch(settings.stage()){
    case 4:
        tosend = frame.clone();
        cv::drawContours(tosend,contours,0,cv::Scalar(0,255,0));
        break;
    case 3:
        //Subtract first
        //absdiff(frame.clone(),backgroundImage, subtracted);
        //cv::cvtColor(filter.filter(subtracted),tosend,CV_GRAY2RGB);

        //Subtract second
        cv::cvtColor(filtered,cvtFiltered,CV_GRAY2RGB);
        cv::cvtColor(filter.filter(backgroundImage),cvtBackground,CV_GRAY2RGB);
        absdiff(cvtFiltered,cvtBackground,tosend);
        break;
    case 2:
        //View the Background Image
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
