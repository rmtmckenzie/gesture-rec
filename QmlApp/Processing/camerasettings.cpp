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
        QJsonValue c; QRgb r; cv::Scalar s;

        qDebug() << "Settings found.";

        c = o["lowColor"];
        r = (unsigned int)c.toInt();
        _filter->lh = qRed(r);
        _filter->ll = qGreen(r);
        _filter->ls = qBlue(r);

        c = o["lowColor"];
        r = (unsigned int)c.toInt();
        _filter->hh = qRed(r);
        _filter->hl = qGreen(r);
        _filter->hs = qBlue(r);

        _filter->updateScalars();
        _filter->printColors();
    }

    //Setup Camera
    //exposure(1,100);
}

CameraSettings::~CameraSettings()
{
    QJsonObject o = settings.object();
    cv::Scalar s = _filter->lowColor;
    QRgb r = qRgb(_filter->lh,_filter->ll,_filter->ls);

    o.insert("lowColor",QJsonValue((int)r));

    s = _filter->highColor;
    r = qRgb(_filter->hh,_filter->hl,_filter->hs);
    o.insert("highColor",QJsonValue((int)r));
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
    _camSource->set(CV_CAP_PROP_BRIGHTNESS, b);
}

void CameraSettings::contrast(double c)
{
    rangeRestrictOne(&c);
    _camSource->set(CV_CAP_PROP_CONTRAST, c);
}

void CameraSettings::saturation(double s)
{
    rangeRestrictOne(&s);
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
    rangeRestrictOne(&a);
    rangeRestrictOne(&e);
    _camSource->set(CV_CAP_PROP_AUTO_EXPOSURE, a);
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

int CameraSettings::stage()
{
    return _stage;
}
