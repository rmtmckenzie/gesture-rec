#ifndef HANDRECPRIVATE_H
#define HANDRECPRIVATE_H

#include <QObject>
#include <QRgb>
#include <QThread>
#include "handrecthread.hpp"

class HandRecAPI;

class HandRecPrivate : public QObject
{
    Q_OBJECT
    friend class HandRecAPI;

public:
    explicit HandRecPrivate(HandRecAPI *parent);
    ~HandRecPrivate();

    void resetHandColors();
    void addHandColor(QRgb color);
    void setCamNum(unsigned int n);
    void setStage(unsigned int s);
    void setExposure(double a, double e);

    void setBlur(unsigned int b);

    void TakeBackgroundImage();
    void ReturnBackgroundImage();
    void OutputRotation();

public slots:
    void lowColorChanged(QRgb c);
    void highColorChanged(QRgb c);

private:
    QThread runnerThread;
    HandRecThread* handrec;
    HandRecAPI* api;

    void setupConnections();

signals:
    void _addHandColor(QRgb color);
    void _resetHandColors();
    void _setCamNum(unsigned int n);
    void _setStage(unsigned int s);
    void _setExposure(double a, double e);
    void _takeBackgroundImage();
    void _setBlur(unsigned int b);

public slots:

};

#endif // HANDRECPRIVATE_H
