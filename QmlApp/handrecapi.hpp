#ifndef HANDRECAPI_HPP
#define HANDRECAPI_HPP

#include <QObject>
#include "Processing/handrecprivate.hpp"
#include <QThread>

class HandRecAPI : public QObject
{
    Q_OBJECT
public:
    explicit HandRecAPI(QObject *parent = 0);
    ~HandRecAPI();

    QVideoFrame::PixelFormat format();

signals:
    void frameUpdate(QVideoFrame frame);

private:
    QThread runnerThread;
    HandRecPrivate* handrec;


public slots:
    void receiveFrame(QVideoFrame frame);

};

#endif // HANDRECAPI_HPP
