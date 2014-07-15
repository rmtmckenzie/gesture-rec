#include "parser.hpp"

#include <QVector>

Parser::Parser(QObject *parent) :
    QObject(parent)
{
}

void Parser::eliminateDefects(cv::Rect bounds, std::vector<cv::Vec4i> defects)
{
    int tolerance = bounds.height/5;
    float angleTolerance = 95;

    std::vector<cv::Vec4i> newDefects;
    int start,end,far;

    std::vector<cv::Vec4i>::iterator i = defects.begin(),
            e = defects.end();

//    while(i != e){

//    }



//    int tolerance =  bRect_height/5;
//	float angleTol=95;
//	vector<Vec4i> newDefects;
//	int startidx, endidx, faridx;
//	vector<Vec4i>::iterator d=defects[cIdx].begin();
//	while( d!=defects[cIdx].end() ) {
//   	    Vec4i& v=(*d);
//	    startidx=v[0]; Point ptStart(contours[cIdx][startidx] );
//   		endidx=v[1]; Point ptEnd(contours[cIdx][endidx] );
//  	    faridx=v[2]; Point ptFar(contours[cIdx][faridx] );
//		if(distanceP2P(ptStart, ptFar) > tolerance && distanceP2P(ptEnd, ptFar) > tolerance && getAngle(ptStart, ptFar, ptEnd  ) < angleTol ){
//			if( ptEnd.y > (bRect.y + bRect.height -bRect.height/4 ) ){
//			}else if( ptStart.y > (bRect.y + bRect.height -bRect.height/4 ) ){
//			}else {
//				newDefects.push_back(v);
//			}
//		}
//		d++;
//	}
//	nrOfDefects=newDefects.size();
//	defects[cIdx].swap(newDefects);
//	removeRedundantEndPoints(defects[cIdx], m);
}

void Parser::parse(cv::Mat mat)
{
    std::vector<std::vector<cv::Point> > contours;

    cv::findContours(mat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    std::vector<cv::Point> largestContour(0);

    foreach (std::vector<cv::Point> pts, contours) {
        if(pts.size() > largestContour.size()){
            largestContour = pts;
        }
    }

    if(!largestContour.size()){
        //no contour found!
        return;
    }

    cv::Rect bounds = cv::boundingRect(largestContour);

    std::vector<int> hullPointIndices(largestContour.size());
    std::vector<cv::Point> hullPoints(largestContour.size());
    std::vector<cv::Vec4i> defects;

    cv::convexHull(largestContour,hullPoints,false);
    cv::convexHull(largestContour,hullPointIndices,false);

    cv::approxPolyDP(hullPoints,hullPoints,18,true);


//    if(largestContour.size() > 3) {
//        cv::convexityDefects(largestContour, hullPointIndices, defects);
//        eliminateDefects(bounds)
//        //eliminate defects
//    }


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

//    QVector<QVector<int>> contourI;
//    QVector<QVector<cv::Point>> contourP;
//    QVector<QVector<cv::Vec4i>> defects;

//    contourI=vector<vector<int> >(contours.size());
//    contourP=vector<vector<Point> >(contours.size());
//    defects=vector<vector<Vec4i> > (contours.size());
}
