#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <opencv2/highgui/highgui.hpp>

typedef cv::Mat cMat;
typedef std::vector<cv::Vec4i> DefectArray;
typedef cv::Point cPoint;
typedef std::vector<cPoint> PointArray;
typedef cv::Scalar cScalar,cColor;
typedef cv::Rect cRect;

typedef DefectArray::iterator DefectArIter;
typedef PointArray::iterator PointArIter;

#endif // DEFINES_HPP
