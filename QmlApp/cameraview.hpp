#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QObject>
#include <QString>
#include <QQuickPaintedItem>
#include "camerasource.hpp"
#include "videosurface.hpp"

#include "handrecapi.hpp"

class CameraView : public QQuickPaintedItem
{
    Q_OBJECT
    friend class VideoSurface;

public:
    explicit CameraView(QQuickItem *parent = 0);

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)

//    Q_INVOKABLE QColor getColor(QPoint pos);

    QString name() const;
    void setName(QString n);

    bool playing() const;
    void setPlaying(bool p);

    void paint(QPainter *painter) Q_DECL_OVERRIDE;

signals:
    void nameChanged(QString name);
    void playingChanged(bool playing);

private:
//    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    bool m_playing;
    QString m_name;

    VideoSurface surface;
    CameraSource source;

    HandRecAPI handrec;

    QVideoFrame curframe;
    QImage::Format m_format;
signals:

protected slots:
    void receiveFrame(QVideoFrame frame);
    void updateFormat(const QVideoSurfaceFormat& format);
};



#endif // CAMERAVIEW_H
