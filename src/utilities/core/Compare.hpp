/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_COMPARE_HPP
#define UTILITIES_CORE_COMPARE_HPP

#include "../UtilitiesAPI.hpp"

#include <utility>  // for std::pair
#include <iostream>
#include <vector>
#include <string>

#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>

// In the GNU C Library, "minor" is defined
// by <sys/sysmacros.h>. For historical compatibility, it is
// currently defined by <sys/types.h> as well, but we plan to
// remove this soon. To use "minor", include <sys/sysmacros.h>
// directly. If you did not intend to use a system-defined macro
// "minor", you should undefine it after including <sys/types.h>.
// int minor() const;
#ifdef major
#  undef major
#endif
#ifdef minor
#  undef minor
#endif

namespace openstudio {

class BCLComponent;
class WorkspaceObject;

/** Small functor object for case insensitive char compare.
 *   Used in istringEqual. */
struct UTILITIES_API IcharCompare
{
  bool operator()(char cA, char cB) const {
    return toupper(cA) == toupper(cB);
  };
};

/** Test equality between two strings without regard to case. */
inline UTILITIES_API bool istringEqual(const std::string& x, const std::string& y) {
  return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin(), IcharCompare());
};

/** Small functor object for case insensitive std::string equality. */
struct UTILITIES_API IstringEqual
{
  bool operator()(const std::string& x, const std::string& y) const {
    return boost::iequals(x, y);
    ;
  };
};

/** Small functor object for case insensitive std::string equality. */
struct UTILITIES_API IstringFind
{
  IstringFind();
  IstringFind(const std::string& target);
  void addTarget(const std::string& target);
  bool operator()(const std::string& other) const;

 private:
  std::vector<std::string> m_targets;
};

/** Compare two strings without regard to case. */
inline UTILITIES_API bool istringLess(const std::string& x, const std::string& y) {
  return boost::lexicographical_compare(x, y, boost::is_iless());
};

/** Small functor object for case insensitive std::string compare.
 *   Used in maps of name to objects. */
struct UTILITIES_API IstringCompare
{
  bool operator()(const std::string& x, const std::string& y) const {
    return boost::lexicographical_compare(x, y, boost::is_iless());
  };
};

/** Small functor object for case insensitive std::string compare in pairs of std::string,
 *   tests first strings then seconds. Used in maps of type and name to objects. */
struct UTILITIES_API IstringPairCompare
{
  bool operator()(const std::pair<std::string, std::string>& x, const std::pair<std::string, std::string>& y) const;
};

class UTILITIES_API VersionString
{
 public:
  explicit VersionString(const std::string& version);

  // Default constructor, assumes the current OpenStudio::openStudioLongVersion is passed
  // For use in std::pair<VersionString, std::string> otherwise C# complains
  VersionString();

  VersionString(int major, int minor);

  VersionString(int major, int minor, int patch);

  VersionString(int major, int minor, int patch, const std::string& patchString);

  VersionString(int major, int minor, int patch, const std::string& patchString, const std::string& buildString);

  std::string str() const;

  /// major verion must be a positve integer, e.g. returns 3 for '3.1.0-rc1'
  int major() const;

  /// minor version must be a positive integer, e.g. returns 1 for '3.1.0-rc1'
  int minor() const;

  /// returns the integer portion of patch version, e.g. returns 0 for '3.1.0-rc1'
  boost::optional<int> patch() const;

  /// returns the string portion of patch version, e.g. returns 'rc1' for '3.1.0-rc1'
  std::string patchString() const;

  /// build version can be any string, e.g. returns 'build2' for '3.1.0-rc1+build2'
  std::string buildString() const;

  /// deprecated will always return empty
  //boost::optional<int> build() const;

  bool operator<(const VersionString& other) const;

  bool operator>(const VersionString& other) const;

  bool operator==(const VersionString& other) const;

  bool operator!=(const VersionString& other) const;

  bool operator<=(const VersionString& other) const;

  bool operator>=(const VersionString& other) const;

  //bool fidelityEqual(const VersionString& other) const;

  /** Returns true if it is plausible for nextVersionCandidate to be the
   *  next version after this one. */
  bool isNextVersion(const VersionString& nextVersionCandidate) const;

 private:
  bool parseVersionString(const std::string& version);
  std::string m_str;
  int m_major;
  int m_minor;
  boost::optional<int> m_patch;
  std::string m_patchString;
  std::string m_buildString;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const VersionString& version);

using OptionalVersionString = boost::optional<VersionString>;

// sorts WorkspaceObjects by name
struct UTILITIES_API WorkspaceObjectNameLess
{
  bool operator()(const WorkspaceObject& a, const WorkspaceObject& b) const;
};

// sorts WorkspaceObjects by name
struct UTILITIES_API WorkspaceObjectNameGreater
{
  bool operator()(const WorkspaceObject& a, const WorkspaceObject& b) const;
};

// sorts BCLComponents by name
struct UTILITIES_API BCLComponentNameLess
{
  bool operator()(const BCLComponent& a, const BCLComponent& b) const;
};

// sorts BCLComponents by name
struct UTILITIES_API BCLComponentNameGreater
{
  bool operator()(const BCLComponent& a, const BCLComponent& b) const;
};

/** Test equality between objects in two vectors of pointers */
template <class T>
bool checkPtrVecEqual(const std::vector<std::shared_ptr<T>>& a, const std::vector<std::shared_ptr<T>>& b) {
  bool result((a.size()) == (b.size()));
  if (result) {
    typename std::vector<std::shared_ptr<T>>::const_iterator aIt;
    typename std::vector<std::shared_ptr<T>>::const_iterator bIt;
    typename std::vector<std::shared_ptr<T>>::const_iterator aItend;
    for (aIt = a.begin(), bIt = b.begin(), aItend = a.end(); aIt < aItend; ++aIt, ++bIt) {
      if ((*aIt) && (*bIt) && (**aIt == **bIt)) {
      } else {
        result = false;
        break;
      }
    }
  }
  return result;
}

/** Test equality of provided value with first element in a pair. */
template <class T, class U>
bool firstOfPairEqual(const std::pair<T, U>& pair, const T& value) {
  return pair.first == value;
}

/** Test equality of provided value with second element in a pair. */
template <class T, class U>
bool secondOfPairEqual(const std::pair<T, U>& pair, const U& value) {
  return pair.second == value;
}

/** Test istringEqual of provided string with first element in a pair. */
template <class T>
bool firstOfPairIStringEqual(const std::pair<std::string, T>& pair, const std::string& value) {
  return istringEqual(pair.first, value);
}

/** Test equality of provided value with second element in a pair. */
template <class T>
bool secondOfPairIStringEqual(const std::pair<T, std::string>& pair, const std::string& value) {
  return istringEqual(pair.second, value);
}

template <class T>
struct FirstOfPairLess
{
  bool operator()(const T& left, const T& right) const {
    return (left.first < right.first);
  }
};

template <class T>
struct SecondOfPairLess
{
  bool operator()(const T& left, const T& right) const {
    return (left.second < right.second);
  }
};

template <class T, class U>
struct GetFirstOfPair
{
  T operator()(const std::pair<T, U>& pair) const {
    return pair.first;
  }
};

template <class T, class U>
struct GetSecondOfPair
{
  U operator()(const std::pair<T, U>& pair) const {
    return pair.second;
  }
};

template <class T>
struct IndexLess
{
  bool operator()(const T& left, const T& right) const {
    return (left.index < right.index);
  }
};

template <class T, class U>
bool uuidsEqual(const T& left, const U& right) {
  return (!left.uuid().isNull()) && (left.uuid() == right.uuid());
}

template <class T, class U>
bool uuidsAndVersionsEqual(const T& left, const U& right) {
  return (!left.uuid().isNull()) && (left.uuid() == right.uuid()) && (!left.versionUUID().isNull()) && (left.versionUUID() == right.versionUUID());
}

template <class T, class U>
bool uuidEquals(const T& object, const U& uuid) {
  return !uuid.isNull() && (object.uuid() == uuid);
}

template <class T, class U>
bool handleEquals(const T& object, const U& handle) {
  return !handle.isNull() && (object.handle() == handle);
}

};  // namespace openstudio

#endif  // UTILITIES_CORE_COMPARE_HPP
