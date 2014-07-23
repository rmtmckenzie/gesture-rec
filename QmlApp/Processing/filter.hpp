#ifndef FILTER_HPP
#define FILTER_HPP

#include <QObject>

#include "defines.hpp"
#include <QRgb>

class Filter : public QObject
{
    Q_OBJECT

    friend class CameraSettings;
public:
    explicit Filter(QObject *parent = 0);

    void resetColors();
    void addColor(QRgb c);

    cMat filter(const cMat inmat);
    void setBackground(const cMat inmat);
    cMat getBackground();
    cMat getBackDiff(const cMat inmat);

    void printColors();

    cMat sharpen(const cMat inmat);
private:
    unsigned char lh,ll,ls, hh,hl,hs;
    cColor lowColor;
    cColor highColor;

    cMat backgroundImage;
    cMat backgroundFiltered;

    void updateScalars();

signals:

public slots:

};

#endif // FILTER_HPP
