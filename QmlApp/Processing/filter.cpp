#include "filter.hpp"

Filter::Filter(QObject *parent) :
    QObject(parent)
{
}

cv::Mat Filter::filter(const cv::Mat inmat)
{

    cv::Mat mat;
    //TODO - downsize to ?? (320 x 240?)
    // by cv::pyrDown(in,out,cv::Size((src.cols+1)/?,(src.rows+1)/?)

    //blur
    cv::blur(inmat,mat,cv::Size(3,3));

    //as HSV
//    cv::cvtColor(mat,mat,CV_RGB2HLS);

//    cv::Mat bw(mat.rows,mat.cols,CV_8U);

    //figure out what scalar and scalar are supposed to be
//    cv::inRange(mat,cv::Scalar(1,1,1),cv::Scalar(1,1,1),bw);

    return mat;
}
