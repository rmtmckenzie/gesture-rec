#include "camerasettings.hpp"

#include <QDebug>
#include <QJsonObject>

void rangeRestrictOne(double* d){
    if((*d) > 1){
        (*d) = 1;
    } else if ((*d) < 0){
        (*d) = 0;
    }
}

CameraSettings::CameraSettings(OpenCVCameraSource* s, Filter *f, Parser *p,
                               Recognizer *r, QObject* parent):
    QObject(parent),
    _camSource(s),
    _filter(f),
    _parser(p),
    _recognizer(r),
    _stage(0),
    settings("settings.set",false,false)
{
    if(settings.isNull()){
        settings.setObject(QJsonObject());
        qDebug() << "No settings file";
    } else {
        QJsonObject o = settings.object();
        QString st;

        qDebug() << "Settings found.";

        st = o["lowColor"].toString();

        unsigned int h,s,l;

        QTextStream(&st) >> h >> l >> s;

        _filter->l1 = h;
        _filter->l2 = l;
        _filter->l3 = s;

        st = o["highColor"].toString();
        QTextStream(&st) >> h >> l >> s;

        _filter->h1 = h;
        _filter->h2 = l;
        _filter->h3 = s;

        _filter->updateColors();
        _filter->printColors();
    }

    //Setup Camera
    //exposure(1,100);
}

CameraSettings::~CameraSettings()
{
    QJsonObject o = settings.object();
    QString format("%1 %2 %3");

    qDebug() << "Saving:";
    _filter->printColors();

    o.insert("lowColor",QJsonValue(format.arg(
                                        QString::number((uint)_filter->l1),
                                        QString::number((uint)_filter->l2),
                                        QString::number((uint)_filter->l3))));
    o.insert("highColor",QJsonValue(format.arg(
                                        QString::number((uint)_filter->h1),
                                        QString::number((uint)_filter->h2),
                                        QString::number((uint)_filter->h3))));

    settings.setObject(o);
}


void CameraSettings::fps(double f)
{
    rangeRestrictOne(&f);
    _camSource->set(CV_CAP_PROP_FPS, f);
}

void CameraSettings::brightness(double b)
{
    rangeRestrictOne(&b);
    //Range 30->255
    _camSource->set(CV_CAP_PROP_BRIGHTNESS, b);
}

void CameraSettings::contrast(double c)
{
    rangeRestrictOne(&c);
    //Range 0->10
    _camSource->set(CV_CAP_PROP_CONTRAST, c);
}

void CameraSettings::saturation(double s)
{
    rangeRestrictOne(&s);
    //Range 0->200
    _camSource->set(CV_CAP_PROP_SATURATION, s);
}

void CameraSettings::hue(double h)
{
    rangeRestrictOne(&h);
    _camSource->set(CV_CAP_PROP_HUE, h);
}

void CameraSettings::gain(double g)
{
    rangeRestrictOne(&g);
    _camSource->set(CV_CAP_PROP_GAIN, g);
}

void CameraSettings::exposure(double a, double e)
{
    qDebug("Setting Exposure");
    //rangeRestrictOne(&a);
    //rangeRestrictOne(&e);

    //Can't seem to get auto exposure to turn back on
    // 0 - Auto, 1 - Manual
    //_camSource->set(CV_CAP_PROP_AUTO_EXPOSURE, a);

    //Range: -11 -> 1
    _camSource->set(CV_CAP_PROP_EXPOSURE,e);
}

void CameraSettings::displayRes(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
    //TODO
}

void CameraSettings::captureRes(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
    //TODO
}

void CameraSettings::resetHandColors()
{
    _filter->resetColors();
}

void CameraSettings::addHandColor(QRgb color)
{
    _filter->addColor(color);
}

void CameraSettings::frameStage(int s)
{
    Q_UNUSED(s)
    _stage = s;
}

void CameraSettings::changeCam(unsigned int c)
{
    _camSource->switchCamera(c);
}

void CameraSettings::blur(unsigned int i)
{
    _filter->blur = i;
}

int CameraSettings::stage()
{
    return _stage;
}
