#include "filter.hpp"

#include <QColor>
#include <QDebug>

Filter::Filter(QObject *parent) :
    QObject(parent)
{
    resetColors();
}

void Filter::resetColors()
{
    lh = 255; ll = 255; ls = 255;
    hh = 0; hl = 0; hs = 0;

    updateScalars();
}

void Filter::addColor(QRgb c)
{
    QColor col(c);
    unsigned char h,l,s;
    unsigned char* convd;
    cv::Mat conv(1,1,CV_8UC3,cv::Scalar(col.blue(),col.green(),col.red()));

    cv::cvtColor(conv,conv,CV_BGR2HLS);
    convd = (unsigned char*)conv.data;
    h = convd[0];
    l = convd[1];
    s = convd[2];

    if(h < lh) {
        lh = h;
    } else if(h > hh) {
        hh = h;
    }

    if(l < ll) {
        ll = l;
    } else if(h > hl) {
        hl = l;
    }

    if(s < ls) {
        ls = s;
    } else if(s > hs) {
        hs = s;
    }

    updateScalars();

}



cv::Mat Filter::filter(const cv::Mat inmat)
{

    cv::Mat mat;
    //TODO - downsize to ?? (320 x 240?)
    // by cv::pyrDown(in,out,cv::Size((src.cols+1)/?,(src.rows+1)/?)

    //blur
    cv::blur(inmat,mat,cv::Size(3,3));

    //as HLS
    cv::cvtColor(mat,mat,CV_RGB2HLS);

    //make bw array
    cv::Mat bw(mat.rows,mat.cols,CV_8U);

    //figure out what scalar and scalar are supposed to be
    cv::inRange(mat,lowColor,highColor,bw);

    cv::medianBlur(bw,bw,5);

    return bw;
}

void Filter::updateScalars()
{
    lowColor[0] = lh > 10 ? lh - 10 : 0;
    lowColor[1] = ll > 10 ? ll - 10 : 0;
    lowColor[2] = ls > 10 ? ls - 10 : 0;

    highColor[0] = hh < 255-10 ? hh + 10 : 255;
    highColor[1] = hl < 255-10 ? hl + 10 : 255;
    highColor[2] = hs < 255-10 ? hs + 10 : 255;
}

void Filter::printColors()
{
    qDebug() << "Low:" << lowColor[0] << "," << lowColor[1] << "," << lowColor[2];
    qDebug() << "High:" << highColor[0] << "," << highColor[1] << "," << highColor[2];
}
