#include "recognizer.hpp"
#include <QDebug>

Recognizer::Recognizer(QObject *parent) :
    QObject(parent)
{
}

/*
 * Calculate the mean handCenter position of the most recent frames
 */
int Recognizer::GetHandPositionMean(int frameNumber, int begin){
    int mean = 0;
    for (int i=begin; i!=frameNumber;i=(i+1)%BUFFERSIZE)
    {
        mean += buffer[i].handCenter.x;
    }
    mean = mean / LOOKUPSIZE;

    return mean;
}

/*
 * Calculate the mean of the fingers position
 *
 * return: Fingers X Mean, Fingers Y Mean
 */
std::pair<int,int> Recognizer::GetFingersMean(int frameNumber, int begin)
{
    int fingersXMean = 0;
    int fingersYMean = 0;
    std::pair <int,int> fingersMean;
    if (buffer[frameNumber].fingers.size())
    {
//        for (int i=begin; i!=frameNumber;i=(i+1)%BUFFERSIZE)
//        {
            for(PointArIter s = buffer[frameNumber].fingers.begin(),
                e = buffer[frameNumber].fingers.end(); s != e; s++)
            {
                fingersYMean += (*s).y;
                fingersXMean += (*s).x;
            }

            fingersXMean = fingersXMean / buffer[frameNumber].fingers.size();
            fingersYMean = fingersYMean / buffer[frameNumber].fingers.size();
//        }

        fingersMean = std::make_pair(fingersXMean,fingersYMean);
    }
    return fingersMean;
}

/*
 * Save the position of the hand in the center of the screen
 *
 * return 1 if successful
 */

int Recognizer::RotationSaveFingers()
{
    if (buffer[bufferCurrent].fingers.size())
    {

        InitialYMean = 0;
        InitialXMean = 0;
        for(PointArIter s = buffer[bufferCurrent].fingers.begin(),
            e = buffer[bufferCurrent].fingers.end(); s != e; s++)
        {
            InitialYMean += (*s).y;
            InitialXMean += (*s).x;
        }

        InitialXMean = InitialXMean / buffer[bufferCurrent].fingers.size();
        InitialYMean = InitialYMean / buffer[bufferCurrent].fingers.size();
    }
    else
    {
       return 0;
    }
    qDebug() << "Finger Center: " << InitialXMean;
    return 1;
}

int Recognizer::recognize(PARSED frameValues)
{
    int begin = bufferCurrent - LOOKUPSIZE;
    if (begin < 0) begin+=BUFFERSIZE;
    int mean = GetHandPositionMean(bufferCurrent, begin);

    //load into buffer
    buffer[bufferCurrent] = frameValues;
    bufferCurrent = (bufferCurrent + 1) % BUFFERSIZE;

    std::pair<int,int> fMean;

    switch (recognizerState)
    {
    case 3:
        //ROTATION
        //Find hand orientation
        //if (waitFrame == bufferCurrent) {

            fMean = GetFingersMean(bufferCurrent, begin);
            //Hand is pointed up
            if(InitialXMean > fMean.first + 50)
            {

                qDebug() << "Up Rotate Left";
                qDebug() << "Current frame: " << bufferCurrent;
                return 1;
                //waitFrame = (bufferCurrent + 5) %BUFFERSIZE;
                //recognizerState = 0;
            }
            else if(InitialXMean + 50 < fMean.first)
            {

                qDebug() << "Up Rotate Right";
                qDebug() << "Current frame: " << bufferCurrent;
                return 2;
                //waitFrame = (bufferCurrent + 5) %BUFFERSIZE;
                //recognizerState = 0;
            }
        //}

        break;
    case 2:
        //IN RIGHT SWIPE
        if(bufferCurrent == waitFrame) recognizerState = 0;

        if (buffer[begin].handCenter.x > 120 && buffer[begin].handCenter.x < 200
                && mean < 180 && mean > 140){

            waitFrame = (bufferCurrent + 1) % BUFFERSIZE;
            if(RotationSaveFingers()) recognizerState = 3;
        }
        break;
    case 1:
        //IN LEFT SWIPE
        if(bufferCurrent == waitFrame) recognizerState = 0;

        if (buffer[begin].handCenter.x > 120 && buffer[begin].handCenter.x < 200
                && mean < 180 && mean > 140){

            waitFrame = (bufferCurrent + 1) % BUFFERSIZE;
            if(RotationSaveFingers()) recognizerState = 3;
        }

        break;
    case 0:
    default:
        //SWIPE LEFT
        if (buffer[begin].handCenter.x > 300 && buffer[begin].handCenter.x < 320
                && mean < 250){
            recognizerState = 1;
            waitFrame = (bufferCurrent + 15) % BUFFERSIZE;

            qDebug("Swipe Left Detected.");
            break;
        }

        //SWIPE RIGHT
        if (buffer[begin].handCenter.x > 0 && buffer[begin].handCenter.x < 20
                && mean > 70){
            recognizerState = 2;
            waitFrame = (bufferCurrent + 15) % BUFFERSIZE;

            qDebug("Swipe Right Detected.");

            break;
        }

        //ROTATION
        if (buffer[begin].handCenter.x > 120 && buffer[begin].handCenter.x < 200
                && mean < 180 && mean > 140){
            waitFrame = (bufferCurrent + 1) % BUFFERSIZE;

            if(RotationSaveFingers()) recognizerState = 3;
        }
        break;

    }
}
