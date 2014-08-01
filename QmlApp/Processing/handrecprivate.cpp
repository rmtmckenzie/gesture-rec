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
    connect(this, &HandRecPrivate::_takeBackgroundImage, handrec, &HandRecThread::TakeBackgroundImage);
    connect(this, &HandRecPrivate::_setBlur, s, &CameraSettings::blur);

    connect(&handrec->recognizer, &Recognizer::_swipeRight, api, &HandRecAPI::swipeRightDetected);
    connect(&handrec->recognizer, &Recognizer::_swipeLeft, api, &HandRecAPI::swipeLeftDetected);
    connect(&handrec->recognizer, &Recognizer::_panRight, api, &HandRecAPI::panRightDetected);
    connect(&handrec->recognizer, &Recognizer::_panLeft, api, &HandRecAPI::panLeftDetected);
    connect(&handrec->recognizer, &Recognizer::_rotateRight, api, &HandRecAPI::rotateRightDetected);
    connect(&handrec->recognizer, &Recognizer::_rotateLeft, api, &HandRecAPI::rotateLeftDetected);

    connect(&handrec->filter, &Filter::highColorChanged, this, &HandRecPrivate::highColorChanged);
    connect(&handrec->filter, &Filter::lowColorChanged, this, &HandRecPrivate::lowColorChanged);
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

void HandRecPrivate::setExposure(double a, double e)
{
    emit _setExposure(a,e);
}

void HandRecPrivate::setBlur(unsigned int b)
{
    emit _setBlur(b);
}

void HandRecPrivate::TakeBackgroundImage()
{
    emit _takeBackgroundImage();
}

void HandRecPrivate::lowColorChanged(QRgb c)
{
    api->setHighColor(QColor(c));
}

void HandRecPrivate::highColorChanged(QRgb c)
{
    api->setLowColor(QColor(c));
}
