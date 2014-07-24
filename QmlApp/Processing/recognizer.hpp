#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include <QObject>

#include "defines.hpp"

#define BUFFERSIZE 100
#define LOOKUPSIZE 10

#define RECOGNIZER_STATE_DEFAULT 0
#define RECOGNIZER_STATE_LEFT_SWIPE 1
#define RECOGNIZER_STATE_RIGHT_SWIPE 2
#define RECOGNIZER_STATE_ROTATION 3



class Recognizer : public QObject
{
    Q_OBJECT
public:
    explicit Recognizer(QObject *parent = 0);
    int recognize(PARSED frameValues);

private:
    int GetHandPositionMean(int frameNumber, int begin);
    std::pair<int,int> GetFingersMean(int frameNumber, int begin);
    int RotationSaveFingers();

    PARSED buffer[BUFFERSIZE];
    int bufferCurrent = 0;

    int recognizerState = 0;
    int waitFrame = 0;
    int InitialXMean = 0;
    int InitialYMean = 0;

signals:
    void _swipeRight();
    void _swipeLeft();
    void _panLeft();
    void _panRight();
    void _rotateLeft();
    void _rotateRight();

public slots:

};

#endif // RECOGNIZER_HPP
