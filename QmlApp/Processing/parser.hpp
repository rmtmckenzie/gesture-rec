#ifndef PARSER_HPP
#define PARSER_HPP

#include <QObject>

#include "defines.hpp"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);

    PointArray parse(cMat c);

    PointArray getContour(cMat c);

    DefectArray getDefects(PointArray contour);
    PointArray getFingertips(DefectArray defects, PointArray contour);
    DefectArray filterDefects(cRect bounds, DefectArray defects, PointArray contour);
    DefectArray filterEndpoints(cRect bounds, DefectArray defects, PointArray contour);
    
    cRect getBounds(PointArray contour);
    PointArray getSingleFingertip(DefectArray defects, PointArray contour);

    void drawFingertips(cMat c);
    void drawContour(cMat c);
    void drawHand(cMat c);
private:
    PointArray contour;
    DefectArray defects;
    PointArray fingers;
    cRect bounds;

signals:
public slots:


};

#endif // PARSER_HPP
