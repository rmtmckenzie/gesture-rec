#ifndef VIDEOSURFACE_HPP
#define VIDEOSURFACE_HPP

#include <QAbstractVideoSurface>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSurface(QObject *parent = 0);
    bool present(const QVideoFrame &frame) Q_DECL_OVERRIDE;
    QList<QVideoFrame::PixelFormat>supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const Q_DECL_OVERRIDE;

private:


signals:
    void frameReceived(QVideoFrame frame);
public slots:

};

#endif // VIDEOSURFACE_HPP
