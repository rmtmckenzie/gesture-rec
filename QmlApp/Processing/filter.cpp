#include "filter.hpp"

#include <QColor>
#include <QDebug>

#include <opencv2/video/video.hpp>

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
    cMat conv(1,1,CV_8UC3,cColor(col.blue(),col.green(),col.red()));

    cv::cvtColor(conv,conv,cv::COLOR_BGR2HLS);
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

void Filter::setBackground(const cMat inmat)
{
    backgroundImage = inmat.clone();
    backgroundFiltered = filter(backgroundImage);
}

cMat Filter::getBackground()
{
    return backgroundImage;
}

cMat Filter::getBackDiff(const cMat inmat)
{
    //Subtract first
    //absdiff(frame.clone(),backgroundImage, subtracted);
    //cv::cvtColor(filter.filter(subtracted),tosend,CV_GRAY2RGB);

    //Subtract second
    cMat diff;
    absdiff(backgroundFiltered,inmat,diff);
    return diff;
}

cMat Filter::filter(const cMat inmat)
{
    cMat mat;
    //TODO - downsize to ?? (320 x 240?)
    // by cv::pyrDown(in,out,cv::Size((src.cols+1)/?,(src.rows+1)/?)

    //blur
    cv::blur(inmat,mat,cv::Size(3,3));

    //as HLS
    cv::cvtColor(mat,mat,cv::COLOR_RGB2HLS);

    //make bw array
    cMat bw(mat.rows,mat.cols,CV_8U);

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
    qDebug() << "Low:" << lh << "," << ll << "," << ls;
    qDebug() << "High:" << hh << "," << hl << "," << hs;
}
