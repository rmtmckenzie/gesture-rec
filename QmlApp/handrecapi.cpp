#include "handrecapi.hpp"

#include <QDebug>

HandRecAPI::HandRecAPI(QObject *parent) :
    QObject(parent)
{
    handrec = new HandRecPrivate(0);
    handrec->moveToThread(&runnerThread);
    QObject::connect(&runnerThread,&QThread::started,handrec,&HandRecPrivate::run);
    QObject::connect(&runnerThread,&QThread::finished,handrec,&HandRecPrivate::deleteLater);

    QObject::connect(handrec,&HandRecPrivate::updateFrame,this,&HandRecAPI::receiveFrame,Qt::QueuedConnection);
    qDebug() << "API: " << QThread::currentThreadId();
    runnerThread.start();
}

HandRecAPI::~HandRecAPI()
{
    runnerThread.exit();
}

QVideoFrame::PixelFormat HandRecAPI::format()
{
    return OPENCV_PIXEL_FORMAT;
}

void HandRecAPI::receiveFrame(QVideoFrame frame)
{
    qDebug() << "Public API Received Frame!";
    emit frameUpdate(frame);
}
