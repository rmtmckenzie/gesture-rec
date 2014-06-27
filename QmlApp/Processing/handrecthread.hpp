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
public:
    explicit HandRecThread(int cam_number, QObject *parent = 0);

    //called in thread by QThread::run
    void run();

    //called outside thread before run starts!
    bool init(HandRecAPI* api);

    //called outside thread; thread safe
    void stop();

private:
    OpenCVCameraSource cam;
    Filter filter;
    Parser parser;
    Recognizer recognizer;

    CameraSettings settings;

    bool keepRunning;

signals:
    void updateFrame(QVideoFrame f);
    void _stop();

private slots:
    void quit(){keepRunning = false;}
};

#endif // HANDRECPRIVATE_HPP
