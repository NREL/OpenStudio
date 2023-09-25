/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
