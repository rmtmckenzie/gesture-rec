#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QVariant>
#include <QMutex>
#include "opencvcamerasource.hpp"
#include "filter.hpp"
#include "parser.hpp"
#include "recognizer.hpp"
#include <QRgb>

#include "../Utils/jsonreader.hpp"

class CameraSettings: public QObject
{
    Q_OBJECT

public:
    explicit CameraSettings(OpenCVCameraSource* s,
                            Filter* f,
                            Parser* p,
                            Recognizer* r,
                            QObject* parent = 0);
    ~CameraSettings();

public slots:
    void fps(double f);
    void brightness(double b);
    void contrast(double c);
    void saturation(double s);
    void hue(double h);
    void gain(double g);
    void exposure(double a,double e);

    void displayRes(int w, int h);
    void captureRes(int w, int h);

    void resetHandColors();
    void addHandColor(QRgb color);

    void frameStage(int s);

    void changeCam(unsigned int c);

public:
    //todo make into enum
    int stage();

private:
    OpenCVCameraSource* _camSource;
    Filter *_filter;
    Parser *_parser;
    Recognizer *_recognizer;
    int _stage;
    JsonReader settings;
};

#endif // CAMERASETTINGS_H
