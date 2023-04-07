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

#ifndef UTILITIES_CORE_FINDER_HPP
#define UTILITIES_CORE_FINDER_HPP

#include "../UtilitiesAPI.hpp"
#include "String.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

/** Functor object for finding objects by name. Templated on T where T has method
   *  name(). Applies to T and to shared_ptr<T>. */
template <class T>
class NameFinder
{
 public:
  NameFinder(const std::string& name, bool caseSensitive = false) : m_name(name), m_caseSensitive(caseSensitive) {}

  bool operator()(const T& object) const {
    if (m_caseSensitive) {
      return (m_name == object.name());
    }
    return boost::iequals(m_name, object.name());
  }

  bool operator()(const std::shared_ptr<T>& object) const {
    if (m_caseSensitive) {
      return (m_name == object->name());
    }
    return boost::iequals(m_name, object->name());
  }

 private:
  std::string m_name;
  bool m_caseSensitive;
};

/** Template function for finding objects in a vector T by name
  */
template <class T>
boost::optional<T> findByName(const std::vector<T>& vec, const std::string& name, bool caseSensitive = false) {
  boost::optional<T> result;
  NameFinder<T> finder(name, caseSensitive);
  typename std::vector<T>::const_iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  if (it != vec.end()) {
    result = *it;
  };
  return result;
}

/** Template function for finding objects in a vector of shared_ptr<T> by name
  */
template <class T>
std::shared_ptr<T> findByName(const std::vector<std::shared_ptr<T>>& vec, const std::string& name, bool caseSensitive = false) {
  std::shared_ptr<T> result;
  NameFinder<T> finder(name, caseSensitive);
  typename std::vector<std::shared_ptr<T>>::const_iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  if (it != vec.end()) {
    result = *it;
  };
  return result;
}

/** Template function for finding index of object in a vector<T> by name. */
template <class T>
boost::optional<int> findIndexByName(const std::vector<T>& vec, const std::string& name, bool caseSensitive = false) {
  boost::optional<int> index;
  NameFinder<T> finder(name, caseSensitive);
  typename std::vector<T>::const_iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  if (it != vec.end()) {
    index = (int)(it - vec.begin());
  }
  return index;
}

/** Template function for finding index of object in a vector<T> by name. */
template <class T>
typename std::vector<T>::iterator findIteratorByName(std::vector<T>& vec, const std::string& name, bool caseSensitive = false) {
  boost::optional<int> index;
  NameFinder<T> finder(name, caseSensitive);
  typename std::vector<T>::iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  return it;
}

/** Functor object for finding objects by name. Templated on T where T has method
   *  name(). Applies to T and to shared_ptr<T>. */
template <class T>
class StructNameFinder
{
 public:
  StructNameFinder(const std::string& name, bool caseSensitive = false) : m_name(name), m_caseSensitive(caseSensitive) {}

  bool operator()(const T& object) const {
    if (m_caseSensitive) {
      return (m_name == object.name);
    }
    return boost::iequals(m_name, object.name);
  }

  bool operator()(const std::shared_ptr<T>& object) const {
    if (m_caseSensitive) {
      return (m_name == object->name);
    }
    return boost::iequals(m_name, object->name);
  }

 private:
  std::string m_name;
  bool m_caseSensitive;
};

/** Template function for finding objects in a vector T by name
  */
template <class T>
boost::optional<T> findStructByName(const std::vector<T>& vec, const std::string& name, bool caseSensitive = false) {
  boost::optional<T> result;
  StructNameFinder<T> finder(name, caseSensitive);
  typename std::vector<T>::const_iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  if (it != vec.end()) {
    result = *it;
  };
  return result;
}

/** Template function for finding objects in a vector of shared_ptr<T> by name
  */
template <class T>
std::shared_ptr<T> findStructByName(const std::vector<std::shared_ptr<T>>& vec, const std::string& name, bool caseSensitive = false) {
  std::shared_ptr<T> result;
  StructNameFinder<T> finder(name, caseSensitive);
  typename std::vector<std::shared_ptr<T>>::const_iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  if (it != vec.end()) {
    result = *it;
  };
  return result;
}

/** Template function for finding index of object in a vector<T> by name. */
template <class T>
boost::optional<int> findStructIndexByName(const std::vector<T>& vec, const std::string& name, bool caseSensitive = false) {
  boost::optional<int> index;
  StructNameFinder<T> finder(name, caseSensitive);
  typename std::vector<T>::const_iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  if (it != vec.end()) {
    index = it - vec.begin();
  }
  return index;
}

/** Template function for finding index of object in a vector<T> by name. */
template <class T>
typename std::vector<T>::iterator findStructIteratorByName(std::vector<T>& vec, const std::string& name, bool caseSensitive = false) {
  boost::optional<int> index;
  StructNameFinder<T> finder(name, caseSensitive);
  typename std::vector<T>::iterator it;
  it = find_if(vec.begin(), vec.end(), finder);
  return it;
}

/** Functor object for finding objects by value. Templated on T, which has method U value(). */
template <class T, typename U>
class ValueFinder
{
 public:
  ValueFinder(const U& value) : m_value(value){};

  bool operator()(const T& object) const {
    return (m_value == object.value());
  }

  bool operator()(const std::shared_ptr<T>& objectPointer) const {
    return (m_value == objectPointer->value());
  }

 private:
  U m_value;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_FINDER_HPP
