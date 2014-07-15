#ifndef PARSER_HPP
#define PARSER_HPP

#include <QObject>

#include <opencv2/video/video.hpp>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);

    void parse(cv::Mat);

private:
    void eliminateDefects(cv::Rect bounds, std::vector<cv::Vec4i> defects);


signals:

public slots:

};

#endif // PARSER_HPP
