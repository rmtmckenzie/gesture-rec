#include "handrecthread.hpp"
#include <QApplication>
#include "opencvvideobuffer.hpp"

#include "../handrecapi.hpp"

#include <QDebug>
#include <QThread>
HandRecThread::HandRecThread(int cam_number, QObject* parent):
    QObject(parent),
    cam(cam_number),
    filter(),
    parser(),
    recognizer(),
    settings(&cam,&filter,&parser,&recognizer),
    keepRunning(true)
{
    connect(this,&HandRecThread::_stop,this,&HandRecThread::quit);
}

bool HandRecThread::init(HandRecAPI *api)
{
    //other intialization stuff goes here

    //connect stuff from API to settings

    return cam.isReady();
}

void HandRecThread::run()
{
    qDebug() << "Hand Recognition Started!";
    cv::Mat frame, filtered, *tosend;

    //todo: check cam ready!

    while(keepRunning){
        frame = cam.update();

        filtered = filter.filter(frame);

        switch(settings.stage()){
        case 1:
            tosend = &filtered;
            break;
        case 0:
        default:
            tosend = &frame;
        }

        OpenCVVideoBuffer* vidbuf = new OpenCVVideoBuffer(*tosend);
        QVideoFrame qframe(vidbuf,QSize(tosend->cols,tosend->rows),OPENCV_PIXEL_FORMAT);
        emit updateFrame(qframe);
        //wait for at most 50 ms
        QApplication::processEvents(QEventLoop::AllEvents,50);
    }
}

void HandRecThread::stop()
{
    emit _stop();
}
