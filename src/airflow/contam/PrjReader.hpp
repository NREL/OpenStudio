/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef AIRFLOW_CONTAM_PRJREADER_HPP
#define AIRFLOW_CONTAM_PRJREADER_HPP

#include <sstream>
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Filesystem.hpp"

#include "PrjDefines.hpp"

namespace openstudio {
namespace contam {

  class Reader
  {
   public:
    explicit Reader(openstudio::filesystem::ifstream& file);
    explicit Reader(const std::string& string, int starting = 0);
    ~Reader() = default;

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
    int lineNumber() const {
      return m_lineNumber;
    }

    template <class T>
    std::vector<T> readSectionVector(const std::string& name = std::string());

    std::vector<int> readIntVector(bool terminated = false);
    //    std::vector<int> readIntStdVector(bool terminated=false);
    //    template <class T> QList<T*> readSectionPointers(std::string name=STRING_INIT);
    //    template <class T> QList<T> readSectionList(STRING name=STRING_INIT);
    //    template <class T, template <class T> class V> V<T> readSectionVector(STRING name);
    //    template <class T> QVector<T> readSectionQVector(STRING name=STRING_INIT);
    //    template <class T> std::vector<T> readSectionStdVector(STRING name=STRING_INIT);
    template <class T>
    std::vector<std::shared_ptr<T>> readElementVector(const std::string& name = std::string());
    template <class T>
    T read();
    template <class T>
    T readNumber();

   private:
    std::string readStdString();
    std::string readLineString();

    std::stringstream m_stream;
    int m_lineNumber;
    std::list<std::string> m_entries;

    REGISTER_LOGGER("openstudio.contam.Reader");
  };

  template <class T>
  std::vector<T> Reader::readSectionVector(const std::string& name) {
    int n = readInt();
    std::vector<T> vector;
    for (int i = 0; i < n; i++) {
      T value;
      value.read(*this);
      vector.push_back(value);
    }
    if (name.empty()) {
      read999("Failed to find section termination");
    } else {
      read999("Failed to find " + name + " section termination");
    }
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

  template <class T>
  std::vector<std::shared_ptr<T>> Reader::readElementVector(const std::string& name) {
    int n = readInt();
    std::vector<std::shared_ptr<T>> vector;
    for (int i = 0; i < n; i++) {
      vector.push_back(std::shared_ptr<T>(T::readElement(*this)));
    }
    if (name.empty()) {
      read999("Failed to find section termination");
    } else {
      read999("Failed to find " + name + " section termination");
    }
    return vector;
  }

  /*
template <class T> QVector<QSharedPointer<T> > Reader::readElementQVector(std::string name)
{
  int n = readInt();
  QVector<QSharedPointer<T> > vector(n);
  for(int i = 0; i<n; i++)
  {
    // No reset in 4.8
    QSharedPointer<T> element(T::readElement(*this));
    vector[i].swap(element);
    //vector[i].reset(T::readElement(this));
  }
  if(name.empty())
    read999("Failed to find section termination");
  else
    read999("Failed to find " + name + " section termination");
  return vector;
}
*/

}  // namespace contam
}  // namespace openstudio

#endif  // AIRFLOW_CONTAM_PRJREADER_HPP
