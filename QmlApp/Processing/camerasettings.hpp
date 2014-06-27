#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QVariant>
#include <QMutex>
#include "opencvcamerasource.hpp"
#include "filter.hpp"
#include "parser.hpp"
#include "recognizer.hpp"

class CameraSettings: public QObject
{
    Q_OBJECT

public:
    explicit CameraSettings(OpenCVCameraSource* s,
                            Filter* f,
                            Parser* p,
                            Recognizer* r,
                            QObject* parent = 0);

public slots:
    void fps(double f);
    void brightness(double b);
    void contrast(double c);
    void saturation(double s);
    void hue(double h);
    void gain(double g);
    void exposure(double e);

    void displayRes(int w, int h);
    void captureRes(int w, int h);

    void filterTintRange(double l, double h);

    void frameStage(int s);

public:
    //todo make into enum
    int stage();

private:
    OpenCVCameraSource* _camSource;
    Filter *_filter;
    Parser *_parser;
    Recognizer *_recognizer;
    int _stage;
};

#endif // CAMERASETTINGS_H
