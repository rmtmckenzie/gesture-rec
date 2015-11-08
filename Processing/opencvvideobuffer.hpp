#ifndef OPENCVVIDEOBUFFER_HPP
#define OPENCVVIDEOBUFFER_HPP

#include <QAbstractVideoBuffer>
#include <opencv2/imgproc/imgproc.hpp>

class OpenCVVideoBuffer : public QAbstractVideoBuffer
{
public:
    explicit OpenCVVideoBuffer(cv::Mat matdata);

    uchar *map(MapMode mode, int *numBytes, int *bytesPerLine);
    void unmap();
    MapMode mapMode() const;

private:
    cv::Mat data;
    MapMode _mapmode;

signals:

public slots:

};

#endif // OPENCVVIDEOBUFFER_HPP
