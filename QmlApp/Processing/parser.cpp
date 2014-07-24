#include "parser.hpp"

#include <QVector>
#include <QLine>

#include <QDebug>

#include <qmath.h>

#include <opencv2/video/video.hpp>

// ANGLETOLERANCE = 95 * PI / 180
//const float ANGLETOLERANCE = 1.658062789f;
const float ANGLETOLERANCE = 3.14159f;

float angle(cPoint s, cPoint e, cPoint f)
{
    float a1 = atan2(s.y - f.y, s.x - f.x);
    float a2 = atan2(e.y - f.y, e.x - f.x);
    float t = fabs(a2 - a1);
    if(t > M_PI)
        return t - M_PI;
    else if(t > M_PI_2)
        return M_PI - t;
    else
        return t;
}

int lensqrd(cPoint s, cPoint e)
{
    int xse = e.x-s.x, yse = e.y-s.y;
    return (xse*xse) + (yse*yse);
}

Parser::Parser(QObject *parent) :
    QObject(parent)
{
}

PARSED Parser::parse(cMat c)
{
    PARSED parserValues;

    contour = getContour(c);
    if(!contour.size()){
        defects.clear();
        fingers.clear();
        inners.clear();
        return parserValues;
    }
    defects = getDefects(contour);
    bounds = getBounds(contour);
    defects = filterDefects(bounds,defects,contour);
//    defects = filterEndpoints(bounds,defects,contour);

    if(defects.size()){
        fingers = getFingertips(defects, contour);
        inners = getFingerInners(defects,contour);
    } else {
        //find one finger
        fingers = getSingleFingertip(defects, contour);
    }

    //Placeholder for determining hand center
    cPoint center, diff, handCenter;

    cv::Scalar fingerMiddle = cv::mean(fingers);
    center = cPoint(bounds.x + (bounds.width >> 1),bounds.y + (bounds.height >> 1));
    diff = cPoint(fingerMiddle[0], fingerMiddle[1]) - center;
    handCenter = center - cPoint(diff.x >> 1, diff.y >> 1);

    parserValues.contour = contour;
    parserValues.defects = defects;
    parserValues.fingers = fingers;
    parserValues.bounds = bounds;
    parserValues.inners = inners;
    parserValues.handCenter = handCenter;

    return parserValues;
}



DefectArray Parser::filterEndpoints(cRect bounds, DefectArray defects, PointArray contour)
{
    int toleranceSqrd = (bounds.width)*(bounds.width)/36;

    int istart,iend;
    int oistart,oiend;

    cPoint pstart,pend;
    cPoint opstart,opend;


    //TODO: fix this, seems a dumb way of doing it.
    // wouldn't you change the defects instead or remove
    // intermediate contour points?
    for(unsigned int i = 0; i < defects.size(); i++){
        for(unsigned int j = i; j < defects.size(); j++){
            istart = defects[i][0];
            iend = defects[i][1];
            oistart = defects[j][0];
            oiend = defects[j][0];


            pstart = contour[istart];
            pend = contour[iend];
            opstart = contour[oistart];
            opend = contour[oiend];

            if(lensqrd(pstart,opend) < toleranceSqrd) {
                contour[istart] = opend;
                break;
            }

            if(lensqrd(opstart,pend) < toleranceSqrd) {
                contour[oistart] = pend;
            }
        }
    }

    return defects;
}

DefectArray Parser::filterDefects(cRect bounds, DefectArray defects, PointArray contour)
{
    //todo: just pass in tolerance
    int toleranceSqrd = bounds.height*bounds.height/49;
    int tolerance = bounds.height/25;

    std::vector<cv::Vec4i> newDefects;
    int istart,iend,ifar,depth;
    cPoint pstart, pend, pfar;

    for(std::vector<cv::Vec4i>::iterator i = defects.begin(),e = defects.end();
            i != e;
            i++){

        cv::Vec4i& v = (*i);
        istart = v[0];
        iend = v[1];
        ifar = v[2];
        depth = v[3];

        pstart = contour[istart];
        pend = contour[iend];
        pfar = contour[ifar];

        if(
                1
//                && depth > tolerance
                && lensqrd(pfar,pstart) > toleranceSqrd
                && lensqrd(pfar,pend) > toleranceSqrd
                && angle(pstart, pend, pfar) < ANGLETOLERANCE
                ){
            newDefects.push_back(v);
        }
    }

    return newDefects;
}


PointArray Parser::getContour(cMat c)
{
    std::vector<PointArray> contours;

    cMat contourmat = c.clone();
    cv::findContours(contourmat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    PointArray largestContour(0);

    foreach (PointArray pts, contours) {
        if(pts.size() > largestContour.size()){
            largestContour = pts;
        }
    }

    if(!largestContour.size()){
        //no contour found!
        //do something?
    }
    return largestContour;
}

cRect Parser::getBounds(PointArray contour)
{
    if(!contour.size()){
        return cRect();
    }
    return cv::boundingRect(contour);
}

DefectArray Parser::getDefects(PointArray contour)
{

    //defects are points which go in from the entire object:
    //
    //  defect[sind, eind, find, l]
    //
    //   -------s       e------    -----
    //  |        \     /       |     |
    //  |         |   |        |     |  l
    //  |          \ /         |     |
    //  |           f          |   -----
    //  |                      |
    //  |______________________|
    //
    //

    DefectArray defects;

    cRect bounds = getBounds(contour);

    std::vector<int> hullPointIndices(contour.size());
    PointArray hullPoints(contour.size());

    cv::convexHull(contour,hullPoints,false);
    cv::convexHull(contour,hullPointIndices,false);

    cv::approxPolyDP(hullPoints,hullPoints,18,true);

    if(hullPoints.size() > 2) {
        cv::convexityDefects(contour, hullPointIndices, defects);
        //todo: eliminate extra endpoints
    }

    return defects;

}

PointArray Parser::getFingerInners(DefectArray defects, PointArray contour)
{
    PointArray points(0);

    if(!defects.size()) {
        return points;
    }

    for(DefectArIter d = defects.begin(), e = defects.end(); d != e; d++){
        points.push_back(contour[(*d)[2]]);
    }

    return points;

}

PointArray Parser::getFingertips(DefectArray defects, PointArray contour)
{
    PointArray points(0);

    if(!defects.size()) {
        return points;
    }

    //add start (first finger)
    points.push_back(contour[defects[0][0]]);

    for(DefectArIter d = defects.begin(), e = defects.end(); d != e; d++){
        //add end (next finger)
        points.push_back(contour[(*d)[1]]);
    }

    return points;
}

PointArray Parser::getSingleFingertip(DefectArray defects, PointArray contour)
{
    Q_UNUSED(defects)
    Q_UNUSED(contour)

    //TODO
    return PointArray();
}

void Parser::drawFingerPoints(cMat c)
{
    for(PointArIter s = fingers.begin(), e = fingers.end(); s != e; s++){
        cv::circle(c,*s,10,cColor(255,0,0),2);
//        cv::putText(c,QString::number((*s).x).toStdString() + "," + QString::number((*s).y).toStdString(),
//                    *s, cv::FONT_HERSHEY_PLAIN, 1.2f, cv::Scalar(200,0,0),2);
    }
    for(PointArIter s = inners.begin(), e = inners.end(); s != e; s++){
        cv::circle(c,*s,10,cColor(0,255,255),2);
    }

//    for(DefectArIter s = defects.begin(), e = defects.end(); s != e; s++) {
//        cPoint pfar = contour[(*s)[2]];

//        float f = angle(contour[(*s)[0]], contour[(*s)[1]], pfar)*180/3.14159;

//        cv::putText(c,QString::number(f).toStdString(),pfar,cv::FONT_HERSHEY_PLAIN,1.2f,cv::Scalar(200,0,0),2);
//    }
}

void Parser::drawContour(cMat c)
{
    std::vector<PointArray> contours;
    contours.push_back(contour);
    cv::drawContours(c,contours,0,cColor(255,255,0),2);
}

void Parser::drawHand(cMat c)
{
    drawContour(c);
    drawFingerPoints(c);
    cv::Scalar fingerMiddle = cv::mean(fingers);
    cPoint center, diff, handCenter;

    center = cPoint(bounds.x + (bounds.width >> 1),bounds.y + (bounds.height >> 1));
    diff = cPoint(fingerMiddle[0], fingerMiddle[1]) - center;

    handCenter = center - cPoint(diff.x >> 1, diff.y >> 1);

    cv::circle(c,handCenter,10,cColor(0,0,255),2);
    cv::line(c,handCenter,center,cColor(0,0,255));
}
