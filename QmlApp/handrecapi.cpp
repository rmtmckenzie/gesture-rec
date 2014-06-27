#include "handrecapi.hpp"

#include <QDebug>

#include <QCamera>
#include <QCameraInfo>

HandRecAPI::HandRecAPI(QObject *parent) :
    QObject(parent),
    isInitialized(false)
{

}

bool HandRecAPI::init(int camnum){
    handrec = new HandRecThread(camnum);
    bool success = handrec->init((HandRecAPI*)this);

    handrec->moveToThread(&runnerThread);
    QObject::connect(&runnerThread,&QThread::started,handrec,&HandRecThread::run);
    QObject::connect(&runnerThread,&QThread::finished,handrec,&HandRecThread::deleteLater);

    QObject::connect(handrec,&HandRecThread::updateFrame,this,&HandRecAPI::receiveFrame,Qt::QueuedConnection);
    qDebug() << "API: " << QThread::currentThreadId();
    runnerThread.start();

    isInitialized = true;

    return success;
}

HandRecAPI::~HandRecAPI()
{
    handrec->stop();
    runnerThread.quit();
    runnerThread.wait();
}


QVideoFrame::PixelFormat HandRecAPI::format()
{
    return OPENCV_PIXEL_FORMAT;
}

void HandRecAPI::receiveFrame(QVideoFrame frame)
{
    emit frameUpdate(frame);
}
