#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QObject>
#include <QString>
#include <QQuickPaintedItem>
#include "camerasource.hpp"
#include "videosurface.hpp"

class CameraView : public QQuickPaintedItem
{
    Q_OBJECT
    friend class VideoSurface;

public:
    explicit CameraView(QQuickItem *parent = 0);

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    QString name() const;
    void setName(QString name);

    void paint(QPainter *painter) Q_DECL_OVERRIDE;
signals:
    void nameChanged(QString name);

//    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
private:
    QString m_name;
    VideoSurface surface;
    CameraSource source;
    QVideoFrame curframe;
    QImage::Format m_format;
signals:

protected slots:
    void receiveFrame(QVideoFrame frame);
    void updateFormat(const QVideoSurfaceFormat& format);
};



#endif // CAMERAVIEW_H
