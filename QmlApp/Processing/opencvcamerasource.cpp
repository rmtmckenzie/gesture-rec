#include "opencvcamerasource.hpp"

#include <QApplication>
#include <QDebug>

OpenCVCameraSource::OpenCVCameraSource(int num,QObject *parent) :
    QObject(parent)
{

    //TODO - options for opening camera
    vidcap = new cv::VideoCapture(num);
}

OpenCVCameraSource::~OpenCVCameraSource()
{
    vidcap->release();
    delete vidcap;
}

cv::Mat OpenCVCameraSource::update()
{
    (*vidcap) >> _frame;
    cv::flip(_frame,_frame,1);
    cv::cvtColor(_frame,_frame,CV_BGR2RGB);
    return _frame;
}


