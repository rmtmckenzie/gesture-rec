#include "videosurface.hpp"
#include <QDebug>

VideoSurface::VideoSurface(QObject *parent) :
    QAbstractVideoSurface(parent)
{
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    emit frameReceived(frame);
    return true;
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType)
//     QVideoFrame::format_
    QList<QVideoFrame::PixelFormat> formats;
    formats.append(QVideoFrame::Format_RGB32);
    return formats;
}
