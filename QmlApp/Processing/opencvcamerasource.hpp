#ifndef OPENCVCAMERASOURCE_HPP
#define OPENCVCAMERASOURCE_HPP

#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class OpenCVCameraSource : public QObject
{
    Q_OBJECT
public:
    explicit OpenCVCameraSource(QObject *parent = 0);
    ~OpenCVCameraSource();

    cv::Mat update();

    void switchCamera(int num);

    bool isReady();

    void set(int propid, double value);

    void init();
private:
    int camNum;
    cv::VideoCapture* vidcap;
    cv::Mat _frame;

signals:

public slots:

};

#endif // OPENCVCAMERASOURCE_HPP
