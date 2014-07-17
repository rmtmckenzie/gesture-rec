#ifndef PARSER_HPP
#define PARSER_HPP

#include <QObject>

#include <opencv2/video.hpp>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);

    std::vector<cv::Point> parse(cv::Mat);

private:
    std::vector<cv::Vec4i> filterDefects(cv::Rect bounds, std::vector<cv::Vec4i> defects, std::vector<cv::Point> contour);
    std::vector<cv::Vec4i> filterEndpoints(cv::Rect bounds, std::vector<cv::Vec4i> defects, std::vector<cv::Point> contour);

signals:

public slots:

};

#endif // PARSER_HPP
