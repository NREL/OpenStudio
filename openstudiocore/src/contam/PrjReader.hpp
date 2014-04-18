/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
#ifndef PRJREADER_H
#define PRJREADER_H

#include <QTextStream>
#include <QStringList>
#include <QVector>
#include <QSharedPointer>
#include <utilities/core/Logger.hpp>

#include "PrjDefines.hpp"

namespace openstudio {
namespace contam {

class Reader
{
public:
  explicit Reader(QTextStream *stream);
  explicit Reader(QString string, int starting=0);
  ~Reader();

  float readFloat();
  double readDouble();
  std::string readString();
  int readInt();
  unsigned readUInt();

  std::string readLine();
  void read999();
  void read999(std::string mesg);
  void readEnd();

  void skipSection();
  std::string readSection();
  int lineNumber(){return m_lineNumber;}

  template <class T> std::vector<T> readSectionVector(std::string name=std::string());

  std::vector<int> readIntVector(bool terminated=false);
  //    std::vector<int> readIntStdVector(bool terminated=false);
  //    template <class T> QList<T*> readSectionPointers(std::string name=STRING_INIT);
  //    template <class T> QList<T> readSectionList(STRING name=STRING_INIT);
  //    template <class T, template <class T> class V> V<T> readSectionVector(STRING name);
  //    template <class T> QVector<T> readSectionQVector(STRING name=STRING_INIT);
  //    template <class T> std::vector<T> readSectionStdVector(STRING name=STRING_INIT);
  template <class T> QVector<QSharedPointer<T> > readElementVector(std::string name=std::string());

  template <class T> T read();
  template <class T> T readNumber();

private:
  QString readQString();
  std::string readStdString();
  QString readLineQString();

  QTextStream *stream;
  int m_lineNumber;
  bool allocated;
  QStringList entries;

  REGISTER_LOGGER("openstudio.contam.Reader");
};

template <class T> std::vector<T> Reader::readSectionVector(std::string name)
{
  int n = readInt();
  std::vector<T> vector;
  for(int i=0;i<n;i++)
  {
    T value;
    value.read(*this);
    vector.push_back(value);
  }
  if(name.empty())
    read999("Failed to find section termination");
  else
    read999("Failed to find "+name+" section termination");
  return vector;
}

//template <class T> QList<T*> Reader::readSectionPointers(std::string name)
//{
//    QList<T*> list;
//    T *object;
//    int n = readInt();
//    for(int i=0;i<n;i++)
//    {
//        object = new T();
//        list << object;
//        object->read(this);
//    }
//    if(IS_NULL(name))
//        read999("Failed to find section termination");
//    else
//        read999("Failed to find "+name+" section termination");
//    return list;
//}

//template <class T> QList<T> Reader::readSectionList(std::string name)
//{
//    QList<T> list;
//    int n = readInt();
//    for(int i=0;i<n;i++)
//    {
//        T object;
//        object.read(this);
//        list << object;
//    }
//    if(IS_NULL(name))
//        read999("Failed to find section termination");
//    else
//        read999("Failed to find "+name+" section termination");
//    return list;
//}

//template <class T, template <class T> class V> V<T> Reader::readSectionVector(std::string name)
//{
//    int n = readInt();
//    V<T> vector(n);
//    for(int i=0;i<n;i++)
//        vector[i].read(*this);
//    if(IS_NULL(name))
//        read999("Failed to find section termination");
//    else
//        read999("Failed to find "+name+" section termination");
//    return vector;
//}

//template <class T> QVector<T> Reader::readSectionQVector(std::string name)
//{
//    int n = readInt();
//    QVector<T> vector(n);
//    for(int i=0;i<n;i++)
//        vector[i].read(*this);
//    if(IS_NULL(name))
//        read999("Failed to find section termination");
//    else
//        read999("Failed to find "+name+" section termination");
//    return vector;
//}

//template <class T> std::vector<T> Reader::readSectionStdVector(std::string name)
//{
//    int n = readInt();
//    std::vector<T> vector;
//    for(int i=0;i<n;i++)
//    {
//        T v;
//        v.read(*this);
//        vector.push_back(v);
//    }
//    if(IS_NULL(name))
//        read999("Failed to find section termination");
//    else
//        read999("Failed to find "+name+" section termination");
//    return vector;
//}

template <class T> QVector<QSharedPointer<T> > Reader::readElementVector(std::string name)
{
  int n = readInt();
  QVector<QSharedPointer<T> > vector(n);
  for(int i=0;i<n;i++)
  {
    // No reset in 4.8
    QSharedPointer<T> element(T::readElement(*this));
    vector[i].swap(element);
    //vector[i].reset(T::readElement(this));
  }
  if(name.empty())
    read999("Failed to find section termination");
  else
    read999("Failed to find "+name+" section termination");
  return vector;
}

} // contam
} // openstudio

#endif // PRJREADER_H
