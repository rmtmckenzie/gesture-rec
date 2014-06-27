#include "cameraview.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QPainter>
#include <QVideoSurfaceFormat>
#include <QQuickWindow>

#include <QSGNode>
#include <QSGTexture>

CameraView::CameraView(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_playing(true),
    surface(this),
    source(&surface,this),
    m_format(QImage::Format_Invalid)
{
//    source.init();
//    connect(&surface,&VideoSurface::frameReceived,this,&CameraView::receiveFrame);
//    connect(&surface,&VideoSurface::surfaceFormatChanged,this,&CameraView::updateFormat);
//    source.start();

    bool result = handrec.init(1);
    m_format = QImage::Format_RGB888;
    if(result){
        connect(&handrec,&HandRecAPI::frameUpdate,this,&CameraView::receiveFrame);
        qDebug() << "Image Format:" << m_format;
        qDebug() << "Frame Format:" << handrec.format();
    }
}

QString CameraView::name() const
{
    return m_name;
}

void CameraView::setName(QString n)
{
    m_name = n;
    emit nameChanged(m_name);
}

bool CameraView::playing() const
{
    return m_playing;
}

void CameraView::setPlaying(bool p)
{
    m_playing = p;
    emit playingChanged(p);
}

void CameraView::receiveFrame(QVideoFrame frame)
{
    if(m_playing){
        curframe = frame;
        update();
    }
}

void CameraView::updateFormat(const QVideoSurfaceFormat &format)
{
    qDebug() << "SURFACE FORMAT PIXEL:" << format.pixelFormat();
    m_format = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
}

void CameraView::paint(QPainter *painter)
{
    //TODO: replace this with QSG/OpenGL

//    qDebug() << "Repaint...";

    if(m_format != QImage::Format_Invalid){
        curframe.map(QAbstractVideoBuffer::ReadOnly);
        QImage im(
                    curframe.bits(), curframe.width(), curframe.height(),
                    curframe.bytesPerLine(), m_format);
        painter->drawImage(
                    QRect(0,0,width(),height()),
                    im,
                    QRect(0,0,im.width(),im.height()));
        curframe.unmap();
    } else {
        //TODO: Replace with loading image.
        painter->fillRect(contentsBoundingRect(), Qt::red);
    }
}


