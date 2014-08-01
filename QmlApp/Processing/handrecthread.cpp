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
    surf = new VideoSurface(this);
    source = new CameraSource(surf,this);

//    cam.init();
    source->init();
    connect(surf,&VideoSurface::frameReceived,this,&HandRecThread::frameReceived);
    source->start();

//    startTimer(0);
}

void HandRecThread::timerEvent(QTimerEvent *)
{
    process();
}

void HandRecThread::TakeBackgroundImage()
{
    filter.setBackground(frame);
}

void HandRecThread::frameReceived(QVideoFrame inframe)
{
    inframe.map(QAbstractVideoBuffer::ReadOnly);
    qDebug() << inframe.pixelFormat();
    frame = cMat(inframe.width(),inframe.height(),CV_8UC3,CV_8UC4).clone();
    inframe.unmap();
    process();
}

void HandRecThread::process(){
    cMat filtered, tosend;
    PARSED parsed;
    int action = 0;

//    frame = cam.update();

    qDebug() << "Process";
    filtered = filter.filter(frame);

    parsed = parser.parse(filtered);

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
        //Draw Hand
        action = recognizer.recognize(parsed);
//        if (action == 0) parser.OutputText = "No Gesture";
//        if (action == 1) parser.OutputText = "Swipe Left";
//        if (action == 2) parser.OutputText = "Swipe Right";
//        if (action == 3) parser.OutputText = "Rotation";
//        if (action == 4) parser.OutputText = "Rotation Left";
//        if (action == 5) parser.OutputText = "Rotation Right";
        tosend = frame.clone();
        parser.drawHand(tosend);       
        break;
    case 1:
        cv::cvtColor(filtered,tosend,CV_GRAY2RGB);
//        tosend = filter.sharpen(frame);
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
