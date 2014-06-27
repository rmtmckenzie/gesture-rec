#ifndef HANDRECAPI_HPP
#define HANDRECAPI_HPP

#include <QObject>
#include "Processing/handrecthread.hpp"
#include <QThread>

class HandRecAPI : public QObject
{
    Q_OBJECT
public:
    explicit HandRecAPI(QObject *parent = 0);
    ~HandRecAPI();

    QVideoFrame::PixelFormat format();

    bool init(int camnum);

signals:
    void frameUpdate(QVideoFrame frame);

private:
    QThread runnerThread;
    HandRecThread* handrec;

    bool isInitialized;

public slots:
    void receiveFrame(QVideoFrame frame);

};

#endif // HANDRECAPI_HPP
