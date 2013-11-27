/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
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
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
*  02110-1301  USA
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

  float readFloat(DECFILELINE);
  double readDouble(DECFILELINE);
  std::string readString(DECFILELINE);
  int readInt(DECFILELINE);
  unsigned readUInt(DECFILELINE);

  std::string readLine(DECFILELINE);
  void read999(DECFILELINE);
  void read999(std::string mesg DECCFILELINE);
  void readEnd(DECFILELINE);

  void skipSection(DECFILELINE);
  std::string readSection(DECFILELINE);
  int lineNumber(){return m_lineNumber;}

  template <class T> std::vector<T> readSectionVector(DECFILELINEC std::string name=std::string());

  std::vector<int> readIntVector(DECFILELINEC bool terminated=false);
  //    std::vector<int> readIntStdVector(DECFILELINEC bool terminated=false);
  //    template <class T> QList<T*> readSectionPointers(DECFILELINEC std::string name=STRING_INIT);
  //    template <class T> QList<T> readSectionList(DECFILELINEC STRING name=STRING_INIT);
  //    template <class T, template <class T> class V> V<T> readSectionVector(DECFILELINEC STRING name);
  //    template <class T> QVector<T> readSectionQVector(DECFILELINEC STRING name=STRING_INIT);
  //    template <class T> std::vector<T> readSectionStdVector(DECFILELINEC STRING name=STRING_INIT);
  template <class T> QVector<QSharedPointer<T> > readElementVector(DECFILELINEC std::string name=std::string());

  template <class T> T read(DECFILELINE);
  template <class T> T readNumber(DECFILELINE);

private:
  QString readQString(DECFILELINE);
  std::string readStdString(DECFILELINE);
  QString readLineQString(DECFILELINE);

  QTextStream *stream;
  int m_lineNumber;
  bool allocated;
  QStringList entries;

  REGISTER_LOGGER("openstudio.contam.Reader");
};

template <class T> std::vector<T> Reader::readSectionVector(DECFILELINEC std::string name)
{
  int n = readInt(ARGFILELINE);
  std::vector<T> vector;
  for(int i=0;i<n;i++)
  {
    T value;
    value.read(*this);
    vector.push_back(value);
  }
  if(name.empty())
    read999("Failed to find section termination" ARGCFILELINE);
  else
    read999("Failed to find "+name+" section termination" ARGCFILELINE);
  return vector;
}

//template <class T> QList<T*> Reader::readSectionPointers(DECFILELINEC std::string name)
//{
//    QList<T*> list;
//    T *object;
//    int n = readInt(ARGFILELINE);
//    for(int i=0;i<n;i++)
//    {
//        object = new T();
//        list << object;
//        object->read(this);
//    }
//    if(IS_NULL(name))
//        read999("Failed to find section termination" ARGCFILELINE);
//    else
//        read999("Failed to find "+name+" section termination" ARGCFILELINE);
//    return list;
//}

//template <class T> QList<T> Reader::readSectionList(DECFILELINEC std::string name)
//{
//    QList<T> list;
//    int n = readInt(ARGFILELINE);
//    for(int i=0;i<n;i++)
//    {
//        T object;
//        object.read(this);
//        list << object;
//    }
//    if(IS_NULL(name))
//        read999("Failed to find section termination" ARGCFILELINE);
//    else
//        read999("Failed to find "+name+" section termination" ARGCFILELINE);
//    return list;
//}

//template <class T, template <class T> class V> V<T> Reader::readSectionVector(DECFILELINEC std::string name)
//{
//    int n = readInt(ARGFILELINE);
//    V<T> vector(n);
//    for(int i=0;i<n;i++)
//        vector[i].read(*this);
//    if(IS_NULL(name))
//        read999("Failed to find section termination" ARGCFILELINE);
//    else
//        read999("Failed to find "+name+" section termination" ARGCFILELINE);
//    return vector;
//}

//template <class T> QVector<T> Reader::readSectionQVector(DECFILELINEC std::string name)
//{
//    int n = readInt(ARGFILELINE);
//    QVector<T> vector(n);
//    for(int i=0;i<n;i++)
//        vector[i].read(*this);
//    if(IS_NULL(name))
//        read999("Failed to find section termination" ARGCFILELINE);
//    else
//        read999("Failed to find "+name+" section termination" ARGCFILELINE);
//    return vector;
//}

//template <class T> std::vector<T> Reader::readSectionStdVector(DECFILELINEC std::string name)
//{
//    int n = readInt(ARGFILELINE);
//    std::vector<T> vector;
//    for(int i=0;i<n;i++)
//    {
//        T v;
//        v.read(*this);
//        vector.push_back(v);
//    }
//    if(IS_NULL(name))
//        read999("Failed to find section termination" ARGCFILELINE);
//    else
//        read999("Failed to find "+name+" section termination" ARGCFILELINE);
//    return vector;
//}

template <class T> QVector<QSharedPointer<T> > Reader::readElementVector(DECFILELINEC std::string name)
{
  int n = readInt(ARGFILELINE);
  QVector<QSharedPointer<T> > vector(n);
  for(int i=0;i<n;i++)
  {
    // No reset in 4.8
    QSharedPointer<T> element(T::readElement(*this));
    vector[i].swap(element);
    //vector[i].reset(T::readElement(this));
  }
  if(name.empty())
    read999("Failed to find section termination" ARGCFILELINE);
  else
    read999("Failed to find "+name+" section termination" ARGCFILELINE);
  return vector;
}

} // contam
} // openstudio

#endif // PRJREADER_H
