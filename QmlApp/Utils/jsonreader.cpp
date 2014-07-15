#include "jsonreader.hpp"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

JsonReader::JsonReader(QString mpath, bool mIsBinary, bool mIsReadonly):
    path(mpath),
    is_binary(mIsBinary),
    is_readonly(mIsReadonly)
{

  QFile f(mpath);
  if ( !f.open(QIODevice::ReadOnly) ) {
     //ERROR
      qDebug() << "ERROR OPENING FILE (read) " << mpath;
      qDebug() << f.errorString();
      return;
  }
  data = f.readAll();
  f.close();

  QJsonDocument doc;
  QJsonParseError error;
  if(is_binary){
      doc = QJsonDocument::fromBinaryData(data);
  } else {
      doc = QJsonDocument::fromJson(data,&error);
  }

  if(doc.isNull()){
      qDebug() << "ERROR LOADING FILE " << mpath;
      if(!is_binary){
          qDebug() << "Error Code: " << error.errorString() << " \n";
      }
  }

  if(doc.isArray()){
      setArray(doc.array());
  } else if (doc.isObject()){
      setObject(doc.object());
  }
}

JsonReader::~JsonReader()
{
  //write back out only if there is an object
  // and the file is not read-only.
  if(!(isNull() || is_readonly)){
      QFile f(path);

      if(!f.open(QIODevice::WriteOnly)){
          qDebug() << "Error opening file for writing: " << path;
          return;
      }

      QByteArray ba;
      if(is_binary){
          ba = toBinaryData();
      } else {
          ba = toJson();
      }

      f.write(ba);
      f.close();
  }
}
