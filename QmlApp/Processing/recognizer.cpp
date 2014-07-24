#include "recognizer.hpp"
#include <QDebug>

Recognizer::Recognizer(QObject *parent) :
    QObject(parent)
{
}

void Recognizer::recognize(PARSED frameValues)
{
    //load into buffer
    //PARSED newParsed;
    //newParsed.handCenter = frameValues.handCenter;
    //buffer[bufferCurrent] = newParsed;
    buffer[bufferCurrent] = frameValues;
    bufferCurrent++;
    if (bufferCurrent >= BUFFERSIZE) bufferCurrent = 0;

    switch (recognizerState)
    {
    case 2:
        //IN RIGHT SWIPE
    case 1:
        //IN LEFT SWIPE
        recognizerState = 0;
        break;
    case 0:
    default:
        int mean = 0;
        int count = 0;
        int begin = bufferCurrent - LOOKUPSIZE;
        if (begin < 0) begin+=BUFFERSIZE;
        for (int i=begin; i!=bufferCurrent;i=(i+1)%BUFFERSIZE)
        {
            mean += buffer[i].handCenter.x;
        }
        mean = mean / LOOKUPSIZE;

        //SWIPE LEFT
        if (buffer[begin].handCenter.x > 300 && buffer[begin].handCenter.x < 320
                && mean < 250){
            recognizerState = 1;
            //PLACEHOLDER: emit state

//            qDebug() << "Begin X: " << buffer[begin].handCenter.x;
//            qDebug() << "Current X: " << buffer[bufferCurrent].handCenter.x;
//            qDebug() << "Count: " << count;

//            qDebug() << "Begin: " << begin;
//            qDebug() << "Buffer Current: " << bufferCurrent;
//            qDebug() << "Left Mean: " << mean;
            qDebug("Swipe Left Detected.");
            break;
        }

        //SWIPE RIGHT
        if (buffer[begin].handCenter.x > 0 && buffer[begin].handCenter.x < 20
                && mean > 70){
            recognizerState = 2;
            //PLACEHOLDER: emit state
            qDebug("Swipe Right Detected.");
            break;
        }
        break;

    }
}
