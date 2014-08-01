#include "opencvcamerasource.hpp"

#include <QApplication>
#include <QDebug>

#include <QThread>

OpenCVCameraSource::OpenCVCameraSource(QObject *parent) :
    QObject(parent),
    camNum(0),
    vidcap()
{
}

void OpenCVCameraSource::init(){
    vidcap.open(1);
    if(!vidcap.isOpened()){
        qDebug() << "Camera not found, defaulting to 0.";
    }
    vidcap.open(CV_CAP_ANDROID + camNum);

    vidcap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    vidcap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    qDebug() << "Height" << vidcap.get(CV_CAP_PROP_FRAME_HEIGHT);
    qDebug() << "Width" << vidcap.get(CV_CAP_PROP_FRAME_WIDTH);
}

OpenCVCameraSource::~OpenCVCameraSource()
{
    vidcap.release();
}

void OpenCVCameraSource::switchCamera(int num)
{
    if(num != camNum){
        qDebug() << "Switching camera to " << num;
        vidcap.open(num);
        camNum = num;
    }
}

bool OpenCVCameraSource::isReady()
{
    return vidcap.isOpened();
}

void OpenCVCameraSource::set(int propid, double value)
{
    vidcap.set(propid, value);
}

cv::Mat OpenCVCameraSource::update()
{
    vidcap >> _frame;
    cv::flip(_frame,_frame,1);
    cv::cvtColor(_frame,_frame,cv::COLOR_BGR2RGB);
    return _frame;
}


