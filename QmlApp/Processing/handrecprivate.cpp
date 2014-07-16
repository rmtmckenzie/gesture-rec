#include "handrecprivate.hpp"
#include <QDebug>

#include "../handrecapi.hpp"

HandRecPrivate::HandRecPrivate(HandRecAPI *parent) :
    QObject(parent),
    api(parent)
{
    handrec = new HandRecThread();
    handrec->moveToThread(&runnerThread);

    QObject::connect(&runnerThread,&QThread::started,handrec,&HandRecThread::run);
    QObject::connect(&runnerThread,&QThread::finished,handrec,&HandRecThread::deleteLater);
    runnerThread.start();
    setupConnections();
}

HandRecPrivate::~HandRecPrivate()
{
    handrec->stop();
    runnerThread.quit();
    runnerThread.wait();
}

void HandRecPrivate::setupConnections()
{
    CameraSettings* s = &handrec->settings;
    connect(this, &HandRecPrivate::_setCamNum, s, &CameraSettings::changeCam);
    connect(this, &HandRecPrivate::_resetHandColors, s, &CameraSettings::resetHandColors);
    connect(this, &HandRecPrivate::_addHandColor, s, &CameraSettings::addHandColor);
    connect(this, &HandRecPrivate::_setStage, s, &CameraSettings::frameStage);

    connect(this, &HandRecPrivate::_setExposure, s, &CameraSettings::exposure);

    connect(this, &HandRecPrivate::_takeBackgroundImage, handrec, &HandRecThread::TakeBackgroundImage);
}

void HandRecPrivate::resetHandColors()
{
    emit _resetHandColors();
}

void HandRecPrivate::addHandColor(QRgb color)
{
    emit _addHandColor(color);
}

void HandRecPrivate::setCamNum(unsigned int n)
{
    emit _setCamNum(n);
}

void HandRecPrivate::setStage(unsigned int s)
{
    emit _setStage(s);
}

void HandRecPrivate::setExposure(float f)
{
    emit _setExposure(f);
}

void HandRecPrivate::TakeBackgroundImage()
{
    emit _takeBackgroundImage();
}
