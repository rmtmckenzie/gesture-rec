#include "opencvvideobuffer.hpp"

OpenCVVideoBuffer::OpenCVVideoBuffer(cv::Mat matdata) :
    QAbstractVideoBuffer(QAbstractVideoBuffer::NoHandle),
    data(matdata)
{
    //todo - make sure the cv::Mat is releasing itself
}

uchar *OpenCVVideoBuffer::map(QAbstractVideoBuffer::MapMode mode, int *numBytes, int *bytesPerLine)
{
    if(mode){
        _mapmode = mode;
    }

    return data.data;
}

void OpenCVVideoBuffer::unmap()
{
    //do nothing for now
    _mapmode = QAbstractVideoBuffer::NotMapped;
}

QAbstractVideoBuffer::MapMode OpenCVVideoBuffer::mapMode() const
{
    return _mapmode;
}

OpenCVVideoBuffer &OpenCVVideoBuffer::OpenCVVideoBuffer::operator =(const OpenCVVideoBuffer &b)
{
    this->data = b.data;
    this->_mapmode = b.mapMode();
}
