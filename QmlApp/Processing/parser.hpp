#ifndef PARSER_HPP
#define PARSER_HPP

#include <QObject>

#include "defines.hpp"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);

    PointArray getContour(cMat c);

    DefectArray getDefects(PointArray contour);
    PointArray getFingertips(DefectArray defects);
    DefectArray filterDefects(cRect bounds, DefectArray defects, PointArray contour);
    DefectArray filterEndpoints(cRect bounds, DefectArray defects, PointArray contour);

private:
signals:
public slots:


};

#endif // PARSER_HPP
