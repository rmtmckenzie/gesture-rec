#include "opencvcamerasource.hpp"

#include <QApplication>
#include <QDebug>

#include <QThread>

OpenCVCameraSource::OpenCVCameraSource(int num, QObject *parent) :
    QObject(parent),
    camNum(num)
{
    vidcap = new cv::VideoCapture(num);
    if(!vidcap->isOpened()){
        qDebug() << "Camera not found, defaulting to 0.";
    }
    vidcap->release();
    delete vidcap;

    vidcap = new cv::VideoCapture(0);

    vidcap->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    vidcap->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    qDebug() << "Height" << vidcap->get(CV_CAP_PROP_FRAME_HEIGHT);
    qDebug() << "Width" << vidcap->get(CV_CAP_PROP_FRAME_WIDTH);

}

OpenCVCameraSource::~OpenCVCameraSource()
{
    vidcap->release();
    delete vidcap;
}

void OpenCVCameraSource::switchCamera(int num)
{
    vidcap->release();
    delete vidcap;
    vidcap = new cv::VideoCapture(num);
    camNum = num;
}

bool OpenCVCameraSource::isReady()
{
    return vidcap->isOpened();
}

void OpenCVCameraSource::set(int propid, double value)
{
    vidcap->set(propid, value);
}

cv::Mat OpenCVCameraSource::update()
{
    (*vidcap) >> _frame;
    cv::flip(_frame,_frame,1);
    cv::cvtColor(_frame,_frame,CV_BGR2RGB);
    return _frame;
}


