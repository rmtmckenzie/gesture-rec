#ifndef FILTER_HPP
#define FILTER_HPP

#include <QObject>

#include <opencv2/video/video.hpp>
#include <QRgb>

class Filter : public QObject
{
    Q_OBJECT

    friend class CameraSettings;
public:
    explicit Filter(QObject *parent = 0);

    void resetColors();
    void addColor(QRgb c);

    cv::Mat filter(const cv::Mat inmat);
    void setBackground(const cv::Mat inmat);
    cv::Mat getBackground();
    cv::Mat getBackDiff(const cv::Mat inmat);

    void printColors();

private:
    unsigned char lh,ll,ls, hh,hl,hs;
    cv::Scalar lowColor;
    cv::Scalar highColor;

    cv::Mat backgroundImage;
    cv::Mat backgroundFiltered;

    void updateScalars();

signals:

public slots:

};

#endif // FILTER_HPP
