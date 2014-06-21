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
    surface(this),
    source(&surface,this),
    m_format(QImage::Format_Invalid)
{
//    source.init();
//    connect(&surface,&VideoSurface::frameReceived,this,&CameraView::receiveFrame);
//    connect(&surface,&VideoSurface::surfaceFormatChanged,this,&CameraView::updateFormat);
//    source.start();

    connect(&handrec,&HandRecAPI::frameUpdate,this,&CameraView::receiveFrame);
//    m_format = QVideoFrame::imageFormatFromPixelFormat(handrec.format());
    m_format = QImage::Format_RGB888;
    qDebug() << "Image Format:" << m_format;
    qDebug() << "Frame Format:" << handrec.format();
}

QString CameraView::name() const
{
    return m_name;
}

void CameraView::setName(QString name)
{
    m_name = name;
    emit nameChanged(m_name);
}

void CameraView::receiveFrame(QVideoFrame frame)
{
    qDebug() << "CameraView received frame!";
    curframe = frame;
    update();
}

void CameraView::updateFormat(const QVideoSurfaceFormat &format)
{
    qDebug() << "SURFACE FORMAT PIXEL:" << format.pixelFormat();
    m_format = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
}

void CameraView::paint(QPainter *painter)
{
    //TODO: replace this with QSG/OpenGL

    qDebug() << "Repaint...";

    if(m_format != QImage::Format_Invalid){
        curframe.map(QAbstractVideoBuffer::ReadOnly);
        QImage im(
                   curframe.bits(), curframe.width(), curframe.height(),
                   curframe.bytesPerLine(), m_format);
        painter->drawImage(0,0,im);
        curframe.unmap();
    } else {
        //TODO: Replace with loading image.
        painter->fillRect(contentsBoundingRect(), Qt::red);
    }
}


