#include "opencvcamerasource.hpp"

#include <QApplication>

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
    return _frame;
}


