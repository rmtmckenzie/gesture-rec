#include "parser.hpp"

#include <QVector>
#include <QLine>

// ANGLETOLERANCE = 95 * PI / 180
const float ANGLETOLERANCE = 1.658062789f;

float angle(cv::Point s, cv::Point e, cv::Point f)
{
    float a1 = atan2(s.y - f.y, s.x - f.x);
    float a2 = atan2(e.y - f.y, e.x - f.x);
    return fabs(a2 - a1);
}

int lensqrd(cv::Point s, cv::Point e)
{
    int xse = e.x-s.x, yse = e.y-s.y;
    return (xse*xse) + (yse*yse);
}

Parser::Parser(QObject *parent) :
    QObject(parent)
{
}


std::vector<cv::Vec4i> Parser::filterEndpoints(cv::Rect bounds,
                                               std::vector<cv::Vec4i> defects,
                                               std::vector<cv::Point> contour)
{
    int toleranceSqrd = (bounds.width)*(bounds.width)/36;

    int istart,iend;
    int oistart,oiend;

    cv::Point pstart,pend;
    cv::Point opstart,opend;


    //TODO: fix this, seems a dumb way of doing it.
    // wouldn't you change the defects instead or remove
    // intermediate contour points?
    for(int i = 0; i < defects.size(); i++){
        for(int j = i; j < defects.size(); j++){
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

    //    Vec4i temp;
//	float avgX, avgY;
//	float tolerance=bRect_width/6;
//	int startidx, endidx, faridx;
//	int startidx2, endidx2;
//	for(int i=0;i<newDefects.size();i++){
//		for(int j=i;j<newDefects.size();j++){
//	    	startidx=newDefects[i][0]; Point ptStart(contours[cIdx][startidx] );
//	   		endidx=newDefects[i][1]; Point ptEnd(contours[cIdx][endidx] );
//	    	startidx2=newDefects[j][0]; Point ptStart2(contours[cIdx][startidx2] );
//	   		endidx2=newDefects[j][1]; Point ptEnd2(contours[cIdx][endidx2] );
//			if(distanceP2P(ptStart,ptEnd2) < tolerance ){
//				contours[cIdx][startidx]=ptEnd2;
//				break;
//			}if(distanceP2P(ptEnd,ptStart2) < tolerance ){
//				contours[cIdx][startidx2]=ptEnd;
//			}
//		}
}

std::vector<cv::Vec4i> Parser::filterDefects(cv::Rect bounds, std::vector<cv::Vec4i> defects, std::vector<cv::Point> contour)
{
    //todo: just pass in tolerance
    int toleranceSqrd = bounds.height*bounds.height/25;

    std::vector<cv::Vec4i> newDefects;
    int istart,iend,ifar;
    cv::Point pstart, pend, pfar;

    for(std::vector<cv::Vec4i>::iterator i = defects.begin(),e = defects.end();
            i != e;
            i++){

        cv::Vec4i& v = (*i);
        istart = v[0];
        iend = v[1];
        ifar = v[2];

        pstart = contour[istart];
        pend = contour[iend];
        pfar = contour[ifar];

        if(
//                pend.y < (bounds.y + bounds.height - bounds.height/4)
//                && pstart.y < (bounds.y + bounds.height - bounds.height/4)
                lensqrd(pfar,pstart) > toleranceSqrd
                && lensqrd(pfar,pend) > toleranceSqrd
                && angle(pstart, pend, pfar) < ANGLETOLERANCE) {
            newDefects.push_back(v);
        }
    }

    return newDefects;
}


std::vector<cv::Point> Parser::parse(cv::Mat mat)
{
    std::vector<std::vector<cv::Point> > contours;

    cv::Mat contourmat = mat.clone();
    cv::findContours(contourmat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    std::vector<cv::Point> largestContour(0);

    foreach (std::vector<cv::Point> pts, contours) {
        if(pts.size() > largestContour.size()){
            largestContour = pts;
        }
    }

    if(!largestContour.size()){
        //no contour found!
        return largestContour;
    }

    cv::Rect bounds = cv::boundingRect(largestContour);

    std::vector<int> hullPointIndices(largestContour.size());
    std::vector<cv::Point> hullPoints(largestContour.size());
    std::vector<cv::Vec4i> defects;

    cv::convexHull(largestContour,hullPoints,false);
    cv::convexHull(largestContour,hullPointIndices,false);

    cv::approxPolyDP(hullPoints,hullPoints,18,true);

    if(largestContour.size() > 3) {
        cv::convexityDefects(largestContour, hullPointIndices, defects);
        defects = filterDefects(bounds, defects, largestContour);
        //todo: eliminate extra endpoints
    }


//    hg->bRect=boundingRect(Mat(hg->contours[hg->cIdx]));
//    convexHull(Mat(hg->contours[hg->cIdx]),hg->hullP[hg->cIdx],false,true);
//    convexHull(Mat(hg->contours[hg->cIdx]),hg->hullI[hg->cIdx],false,false);
//    approxPolyDP( Mat(hg->hullP[hg->cIdx]), hg->hullP[hg->cIdx], 18, true );
//    if(hg->contours[hg->cIdx].size()>3 ){
//        convexityDefects(hg->contours[hg->cIdx],hg->hullI[hg->cIdx],hg->defects[hg->cIdx]);
//        hg->eleminateDefects(m);
//    }
//    bool isHand=hg->detectIfHand();
//    hg->printGestureInfo(m->src);
//    if(isHand){
//        hg->getFingerTips(m);
//        hg->drawFingerTips(m);
//        myDrawContours(m,hg);
//    }

    return largestContour;

}
