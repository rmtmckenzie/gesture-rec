#include "recognizer.hpp"
#include <QDebug>

Recognizer::Recognizer(QObject *parent) :
    QObject(parent),
    bufferCurrent(0),
    recognizerState(0),
    waitFrame(0),
    InitialXMean(0),
    InitialYMean(0)
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
//    qDebug() << "Finger Center: " << InitialXMean;
    return 1;
}

int Recognizer::recognize(PARSED frameValues)
{
    int returnValue = 0;
    int begin = bufferCurrent - LOOKUPSIZE;
    if (begin < 0) begin+=BUFFERSIZE;
    int mean = GetHandPositionMean(bufferCurrent, begin);

    //load into buffer
    buffer[bufferCurrent] = frameValues;

    std::pair<int,int> fMean;
    //int xMean = 0;

    switch (recognizerState)
    {     
    case 3:
//        if (waitFrame == bufferCurrent) {
        if (buffer[bufferCurrent].fingers.size()){
            returnValue = 3;
            fMean = GetFingersMean(bufferCurrent, begin);

            //ROTATION
            if(InitialXMean > (fMean.first + 50) && buffer[bufferCurrent].handCenter.x < buffer[begin].handCenter.x + 30
                    && buffer[bufferCurrent].handCenter.x > buffer[begin].handCenter.x - 30)
            {
                //ROTATE LEFT
                emit _rotateLeft();
                returnValue = 4;
//                recognizerState = 0;
            }
            else if((InitialXMean + 50) < fMean.first && buffer[bufferCurrent].handCenter.x < buffer[begin].handCenter.x + 30
                    && buffer[bufferCurrent].handCenter.x > buffer[begin].handCenter.x - 30)
            {
                //ROTATE RIGHT
                emit _rotateRight();
                returnValue = 5;
                //recognizerState = 0;
            }

            //PAN
            if(InitialXMean > (fMean.first + 70) && buffer[bufferCurrent].handCenter.x + 70 < buffer[begin].handCenter.x )
            {
                //PAN LEFT
                emit _panLeft();
                returnValue = 4;
                //waitFrame = (bufferCurrent + 5) %BUFFERSIZE;
//                recognizerState = 0;
            }
            else if((InitialXMean + 70) < fMean.first && buffer[bufferCurrent].handCenter.x > buffer[begin].handCenter.x + 70)
            {
                //PAN RIGHT
                emit _panRight();
                returnValue = 5;
                //waitFrame = (bufferCurrent + 5) %BUFFERSIZE;
//                recognizerState = 0;
            }
//            //DOESNT WORK DUE TO ARM BEING DETECTED
//            else if((InitialYMean + 50) < fMean.second && buffer[bufferCurrent].handCenter.y > buffer[begin].handCenter.y + 70)
//            {
//                //PAN DOWN
//                emit _panDown();
//                recognizerState = 0;
//            }
//            else if((InitialYMean + 50) < fMean.second && buffer[bufferCurrent].handCenter.y > buffer[begin].handCenter.y + 50)
//            {
//                //PAN UP
//                emit _panUp();
//                recognizerState = 0;
//            }
        } else {
            recognizerState = 0;
        }

        break;
    case 2:
        //IN RIGHT SWIPE
        if(bufferCurrent == waitFrame) recognizerState = 0;
        returnValue = 2;
        if (buffer[begin].handCenter.x > 120 && buffer[begin].handCenter.x < 200
                && mean < 180 && mean > 140){

            waitFrame = (bufferCurrent + 5) % BUFFERSIZE;
            if(RotationSaveFingers()) recognizerState = 3;
        }
        break;
    case 1:
        //IN LEFT SWIPE
        if(bufferCurrent == waitFrame) recognizerState = 0;
        returnValue = 1;

        if (buffer[begin].handCenter.x > 120 && buffer[begin].handCenter.x < 200
                && mean < 180 && mean > 140){

            waitFrame = (bufferCurrent + 5) % BUFFERSIZE;
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
            emit _swipeLeft();
            break;
        }

        //SWIPE RIGHT
        if (buffer[begin].handCenter.x > 0 && buffer[begin].handCenter.x < 20
                && mean > 70){
            recognizerState = 2;
            waitFrame = (bufferCurrent + 15) % BUFFERSIZE;
            emit _swipeRight();
            break;
        }

        //ROTATION
        if (buffer[begin].handCenter.x > 120 && buffer[begin].handCenter.x < 200
                && mean < 180 && mean > 140){
            waitFrame = (bufferCurrent + 5) % BUFFERSIZE;

            if(RotationSaveFingers()) recognizerState = 3;
        }
        break;

    }

    //Increment buffer counter
    bufferCurrent = (bufferCurrent + 1) % BUFFERSIZE;

    //Return
    return returnValue;

}
