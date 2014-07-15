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

public slots:

};

#endif // HANDRECPRIVATE_H
