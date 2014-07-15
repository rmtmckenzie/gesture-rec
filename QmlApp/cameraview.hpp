#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QObject>
#include <QString>
#include <QQuickPaintedItem>
#include <QVideoSurfaceFormat>

#include "handrecapi.hpp"

class CameraView : public QQuickPaintedItem
{
    Q_OBJECT
    friend class VideoSurface;

public:
    explicit CameraView(QQuickItem *parent = 0);

    Q_PROPERTY(QQuickItem* api READ api WRITE setApi NOTIFY apiChanged)
    Q_PROPERTY(bool playing READ playing NOTIFY playingChanged)

    Q_INVOKABLE QColor getColor(int x, int y);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();

    QString name() const;
    void setName(QString n);

    void paint(QPainter *painter) Q_DECL_OVERRIDE;

    QQuickItem* api() const;
    void setApi(QQuickItem* api);

    bool playing() const;

signals:
    void apiChanged(QQuickItem* api);

    void nameChanged(QString name);
    void playingChanged(bool playing);

private:
//    QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    bool m_playing;
    QString m_name;

    QVideoFrame curframe;
    QImage::Format m_format;
    HandRecAPI* m_handrec;
signals:

protected slots:
    void receiveFrame(QVideoFrame frame);
};



#endif // CAMERAVIEW_H
