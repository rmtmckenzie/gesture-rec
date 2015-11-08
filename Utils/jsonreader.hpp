#ifndef JSONREADER_H
#define JSONREADER_H

#include <QObject>
#include <QJsonDocument>

class JsonReader : public QJsonDocument
{
public:
    explicit JsonReader(QString path, bool is_binary, bool mIsReadonly);
    ~JsonReader();

private:
    QString path;
    QByteArray data;
    bool is_binary;
    bool is_readonly;
};

#endif // JSONREADER_H
