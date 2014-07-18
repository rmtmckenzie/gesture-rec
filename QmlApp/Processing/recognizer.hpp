#ifndef RECOGNIZER_HPP
#define RECOGNIZER_HPP

#include <QObject>

#include "defines.hpp"

class Recognizer : public QObject
{
    Q_OBJECT
public:
    explicit Recognizer(QObject *parent = 0);

signals:

public slots:

};

#endif // RECOGNIZER_HPP
