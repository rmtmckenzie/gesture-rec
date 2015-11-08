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

    void edgeDetect(const cMat inmat);

    void printColors();

    cMat sharpen(const cMat inmat);
private:
    int blur;

    unsigned char l1,l2,l3, h1,h2,h3;
    cColor lowColor;
    cColor highColor;

    cMat backgroundImage;
    cMat backgroundFiltered;

    void updateColors();

signals:
    void lowColorChanged(QRgb c);
    void highColorChanged(QRgb c);

public slots:

};

#endif // FILTER_HPP
