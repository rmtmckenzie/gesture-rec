#include "filter.hpp"

#include <QColor>
#include <QDebug>

#include <opencv2/video/video.hpp>

//#define HLS

#define filteramount 20

Filter::Filter(QObject *parent) :
    QObject(parent),
    blur(0)
{
    resetColors();
}

void Filter::resetColors()
{
    l1 = 255; l2 = 255; l3 = 255;
    h1 = 0; h2 = 0; h3 = 0;

    updateColors();
}

static void rgb2hls(const unsigned char r, const unsigned char g,const unsigned char b,
             unsigned char* h, unsigned char* l, unsigned char* s)
{
    unsigned char* convd;
    cMat conv(1,1,CV_8UC3,cColor(r,g,b));
    cv::cvtColor(conv,conv,cv::COLOR_RGB2HLS);
    convd = (unsigned char*)conv.data;
    *h = convd[0];
    *l = convd[1];
    *s = convd[2];
}

static void hls2rgb(const unsigned char h, const unsigned char l,const unsigned char s,
             unsigned char* r, unsigned char* g, unsigned char* b)
{
    unsigned char* convd;
    cMat conv(1,1,CV_8UC3,cColor(h,l,s));
    cv::cvtColor(conv,conv,cv::COLOR_HLS2RGB);
    convd = (unsigned char*)conv.data;
    *r = convd[0];
    *g = convd[1];
    *b = convd[2];
}

void Filter::addColor(QRgb c)
{
    QColor col(c);
    unsigned char c1,c2,c3;
#ifdef HLS
    rgb2hls(col.red(),col.green(),col.blue(),&c1,&c2,&c3);
#else
    c1=col.red();c2=col.green();c3=col.blue();
#endif

    if(c1 < l1) {
        l1 = c1;
    } else if(c1 > h1) {
        h1 = c1;
    }

    if(c2 < l2) {
        l2 = c2;
    } else if(c1 > h2) {
        h2 = c2;
    }

    if(c3 < l3) {
        l3 = c3;
    } else if(c3 > h3) {
        h3 = c3;
    }

    updateColors();
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

void Filter::edgeDetect(const cMat inmat)
{
    Q_UNUSED(inmat)
    // Not implemented yet.
}

cMat Filter::sharpen(const cMat inmat)
{
    cMat mat;
    cv::GaussianBlur(inmat, mat, cv::Size(5,5), 5);
    cv::addWeighted(inmat, 1.5, mat, -0.5, 0, mat);
    return mat;
}

cMat Filter::filter(const cMat inmat)
{
    cMat mat;

    //TODO - downsize to ?? (320 x 240?)
    // by cv::pyrDown(in,out,cv::Size((src.cols+1)/?,(src.rows+1)/?)

    switch(blur){
    case 4:
        cv::GaussianBlur(inmat,inmat,cv::Size(5,5),3,3);
        break;
    case 3:
        cv::medianBlur(inmat,inmat,9);
        break;
    case 2:
        cv::blur(inmat,inmat,cv::Size(9,9));
        break;
    case 1:
        cv::blur(inmat,inmat,cv::Size(5,5));
        break;
    case 0:
    default:
        break;
    }

#ifdef HLS
    cv::cvtColor(inmat,mat,cv::COLOR_RGB2HLS);
#else
    mat = inmat;
#endif

    //make bw array
    cMat bw(mat.rows,mat.cols,CV_8U);

    cv::inRange(mat,lowColor,highColor,bw);

    cv::medianBlur(bw,bw,9);

    return bw;
}


void Filter::updateColors()
{
    lowColor[0] = l1 > filteramount ? l1 - filteramount  : 0;
    lowColor[1] = l2 > filteramount ? l2 - filteramount : 0;
    lowColor[2] = l3 > filteramount ? l3 - filteramount : 0;

    highColor[0] = h1 < 255-filteramount ? h1 + filteramount : 255;
    highColor[1] = h2 < 255-filteramount ? h2 + filteramount : 255;
    highColor[2] = h3 < 255-filteramount ? h3 + filteramount : 255;

#ifdef HLS
    unsigned char r,g,b;
    hls2rgb(l1,l2,l3,&r,&g,&b);
    emit lowColorChanged(QColor(r,g,b).rgb());
    hls2rgb(h1,h2,h3,&r,&g,&b);
    emit highColorChanged(QColor(r,g,b).rgb());
#else
    emit lowColorChanged(QColor(l1,l2,l3).rgb());
    emit highColorChanged(QColor(h1,h2,h3).rgb());
#endif
}

void Filter::printColors()
{
    qDebug() << "Low:" << l1 << "," << l2 << "," << l3;
    qDebug() << "High:" << h1 << "," << h2 << "," << h3;
}
