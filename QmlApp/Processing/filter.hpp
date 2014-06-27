#ifndef FILTER_HPP
#define FILTER_HPP

#include <QObject>

#include <opencv2/video/video.hpp>

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = 0);

    cv::Mat filter(const cv::Mat inmat);


signals:

public slots:

};

#endif // FILTER_HPP
