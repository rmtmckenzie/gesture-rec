#include "handrecapi.hpp"

#include <QDebug>

#include <QCamera>
#include <QCameraInfo>
#include <QRgb>

HandRecAPI::HandRecAPI(QQuickItem *parent) :
    QQuickItem(parent),
    priv(this),
    m_running(false)
{
    if(m_running){
        QObject::connect(priv.handrec,&HandRecThread::updateFrame,this,&HandRecAPI::receiveFrame,Qt::QueuedConnection);
    }
}

bool HandRecAPI::running() const
{
    return m_running;
}

void HandRecAPI::setRunning(bool r)
{
    if(r == m_running){
        //do nothing
    } else {
        if(r){
            qDebug() << "Running set to true!";
            connect(priv.handrec,&HandRecThread::updateFrame,this,&HandRecAPI::receiveFrame,Qt::QueuedConnection);
        } else {
            disconnect(priv.handrec,&HandRecThread::updateFrame,this,&HandRecAPI::receiveFrame);
        }
        m_running = r;
        emit runningChanged(r);
    }
}

unsigned int HandRecAPI::camNumber() const
{
    return m_camNum;
}

void HandRecAPI::setCamNumber(unsigned int n)
{
    if(n != m_camNum){
        priv.setCamNum(n);
    }
}

unsigned int HandRecAPI::stage() const
{
    return m_stage;
}

void HandRecAPI::setStage(unsigned int s)
{
    if(s != m_stage){
        priv.setStage(s);
        m_stage = s;
        emit stageChanged(s);
    }
}

//void HandRecAPI::setExposure(float arg)
//{
//    if (m_exposure != arg) {
//        m_exposure = arg;
//        priv.setExposure(arg);
//        emit exposureChanged(arg);
//    }
//}

QColor HandRecAPI::lowColor()
{
    return m_lowColor;
}

void HandRecAPI::setLowColor(QColor c)
{
    m_lowColor = c;
    emit lowColorChanged(c);
}

QColor HandRecAPI::highColor()
{
    return m_highColor;
}

float HandRecAPI::exposure() const
{
    return m_exposure;
}

void HandRecAPI::setHighColor(QColor c)
{
    m_highColor = c;
    emit highColorChanged(c);
}

void HandRecAPI::resetHandColors()
{
    priv.resetHandColors();
}

void HandRecAPI::addHandColor(QRgb colour)
{
    priv.addHandColor(colour);
}

void HandRecAPI::addHandColor(QColor colour)
{
    addHandColor(colour.rgb());
}

void HandRecAPI::receiveFrame(QVideoFrame frame)
{
    emit frameUpdate(frame);
}

void HandRecAPI::invokeTakeBackgroundImage()
{
    priv.TakeBackgroundImage();
}

void HandRecAPI::setExposure(double a, double e)
{
    priv.setExposure(a,e);
}

void HandRecAPI::OutputRotation()
{
//    parent->rootObject->setProperty("RotationText", QVariant("Left"));
}
