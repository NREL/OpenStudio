/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    std::vector<T> readSectionVector(std::string name = std::string());

    std::vector<int> readIntVector(bool terminated = false);
    //    std::vector<int> readIntStdVector(bool terminated=false);
    //    template <class T> QList<T*> readSectionPointers(std::string name=STRING_INIT);
    //    template <class T> QList<T> readSectionList(STRING name=STRING_INIT);
    //    template <class T, template <class T> class V> V<T> readSectionVector(STRING name);
    //    template <class T> QVector<T> readSectionQVector(STRING name=STRING_INIT);
    //    template <class T> std::vector<T> readSectionStdVector(STRING name=STRING_INIT);
    template <class T>
    std::vector<std::shared_ptr<T>> readElementVector(std::string name = std::string());
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
  std::vector<T> Reader::readSectionVector(std::string name) {
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
  std::vector<std::shared_ptr<T>> Reader::readElementVector(std::string name) {
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
