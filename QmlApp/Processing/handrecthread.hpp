#ifndef HANDRECPRIVATE_HPP
#define HANDRECPRIVATE_HPP

#include <QObject>
#include <QVideoFrame>

#include "opencvcamerasource.hpp"
#include "camerasettings.hpp"
#include "filter.hpp"
#include "parser.hpp"
#include "recognizer.hpp"

class HandRecAPI;

#define OPENCV_PIXEL_FORMAT QVideoFrame::Format_RGB24

class HandRecThread : public QObject
{
    Q_OBJECT
    friend class HandRecPrivate;
public:

    explicit HandRecThread(QObject *parent = 0);

    //called in thread by QThread::run
    void run();

    //called outside thread; thread safe
    void stop();

private:
    OpenCVCameraSource cam;
    Filter filter;
    Parser parser;
    Recognizer recognizer;

    CameraSettings settings;

    bool keepRunning;

    void process();
signals:
    void updateFrame(QVideoFrame f);
    void _stop();

protected:
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;

private slots:
    void quit(){keepRunning = false;}
};

#endif // HANDRECPRIVATE_HPP