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
    m_format(HandRecAPI::IMAGE_FORMAT),
    m_handrec(NULL)
{
//    source.init();
//    connect(&surface,&VideoSurface::frameReceived,this,&CameraView::receiveFrame);
//    connect(&surface,&VideoSurface::surfaceFormatChanged,this,&CameraView::updateFormat);
//    source.start();

}

QColor CameraView::getColor(int x, int y)
{
    int xt = x*(curframe.width()/width());
    int yt = y*(curframe.height()/height());
    curframe.map(QAbstractVideoBuffer::ReadOnly);
    QImage im(
                curframe.bits(), curframe.width(), curframe.height(),
                curframe.bytesPerLine(), m_format);
    QColor toret(im.pixel(xt,yt));
    im.setPixel(xt,yt,qRgb(255,0,0));
    curframe.unmap();
    update();
    return toret;
}



bool CameraView::playing() const
{
    return m_playing;
}

void CameraView::play()
{
    m_playing = true;
    emit playingChanged(true);
}

void CameraView::pause()
{
    m_playing = false;
    emit playingChanged(false);
}

void CameraView::receiveFrame(QVideoFrame frame)
{
    if(m_playing){
        curframe = frame;
        update();
    }
}

void CameraView::paint(QPainter *painter)
{
    //TODO: replace this with QSG/OpenGL
    if(curframe.isValid()){
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
//        painter->fillRect(contentsBoundingRect(), Qt::red);
    }
}

QQuickItem *CameraView::api() const
{
    return (QQuickItem*)m_handrec;
}

void CameraView::setApi(QQuickItem *api)
{
    if(m_handrec){
        disconnect(m_handrec,&HandRecAPI::frameUpdate,this,&CameraView::receiveFrame);
    }

    if(api){
        HandRecAPI* hr = qobject_cast<HandRecAPI*>(api);
        if(hr){
            m_handrec = hr;
            qDebug() << "Cameraview connected...";
            connect(m_handrec,&HandRecAPI::frameUpdate,this,&CameraView::receiveFrame);
        } else {
            qWarning("Invalid QQuickItem set to CameraView.api");
            m_handrec = NULL;
        }
    }
    emit apiChanged((QQuickItem*)m_handrec);
}



