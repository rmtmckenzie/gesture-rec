#ifndef HANDRECAPI_HPP
#define HANDRECAPI_HPP

#include <QObject>
#include <QQuickItem>
#include "Processing/handrecprivate.hpp"
#include "Processing/handrecthread.hpp"
#include <QThread>
#include <QVideoFrame>

class HandRecAPI : public QQuickItem
{
    Q_OBJECT
    friend class HandRecPrivate;
public:
    explicit HandRecAPI(QQuickItem *parent = 0);

////////////////////////// QML API ///////////////////////////

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(unsigned int camNumber READ camNumber WRITE setCamNumber NOTIFY camNumberChanged)
    Q_PROPERTY(unsigned int stage READ stage WRITE setStage NOTIFY stageChanged)
//    Q_PROPERTY(float exposure READ exposure WRITE setExposure NOTIFY exposureChanged)

    Q_PROPERTY(QColor lowColor READ lowColor NOTIFY lowColorChanged)
    Q_PROPERTY(QColor highColor READ highColor NOTIFY highColorChanged)

    Q_INVOKABLE void resetHandColors(); //TODO replace with list of colours?
    Q_INVOKABLE void addHandColor(QColor colour);

    Q_INVOKABLE void invokeTakeBackgroundImage();

//////////////////////// REGULAR API /////////////////////////
signals:
    void frameUpdate(QVideoFrame frame);

public:
    void addHandColor(QRgb colour);


////////////////// OTHER METHODS AND ATTRS ///////////////////
public:

    static const QImage::Format IMAGE_FORMAT = QImage::Format_RGB888;
    static const QVideoFrame::PixelFormat FRAME_FORMAT = OPENCV_PIXEL_FORMAT;

private:
    void setLowColor(QColor c);
    void setHighColor(QColor c);

    HandRecPrivate priv;

private slots:
    void receiveFrame(QVideoFrame frame);

////////////////////// PROPERTY STUFF ///////////////////////
public:
        bool running() const;
        unsigned int camNumber() const;
        unsigned int stage() const;

        void setExposure(double a, double e);
        void OutputRotation();
        QColor lowColor();
        QColor highColor();
        float exposure() const;

public slots:
        void setRunning(bool r);
        void setCamNumber(unsigned int n);
        void setStage(unsigned int s);
//        void setExposure(float arg);

signals:
        void runningChanged(bool r);
        void camNumberChanged(unsigned int n);
        void stageChanged(unsigned int s);
        void lowColorChanged(QColor c);
        void highColorChanged(QColor c);
        void exposureChanged(float arg);

private:
        bool m_running;
        unsigned int m_camNum;
        unsigned int m_stage;
        QColor m_lowColor;
        QColor m_highColor;
        float m_exposure;
};

#endif // HANDRECAPI_HPP
