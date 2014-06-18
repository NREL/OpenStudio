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

#include "Compare.hpp"
#include "Logger.hpp"
#include "../idf/WorkspaceObject.hpp"
#include "../bcl/BCLComponent.hpp"
#include "Assert.hpp"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace openstudio {

IstringFind::IstringFind()
{
}

IstringFind::IstringFind(const std::string& target)
{
  addTarget(target);
}
void IstringFind::addTarget(const std::string& target)
{
  m_targets.push_back(target);
}

bool IstringFind::operator()(const std::string& other) const
{
  for (const std::string& target : m_targets){
    if (istringEqual(target, other)){
      return true;
    }
  }
  return false;
}

bool IstringPairCompare::operator()(const std::pair<std::string, std::string>& x,
                                    const std::pair<std::string, std::string>& y) const
{
  bool result(false);

  if (boost::iequals(x.first, y.first)) {
    result = boost::lexicographical_compare(x.second, y.second, boost::is_iless());
  }
  else {
    result = boost::lexicographical_compare(x.first, y.first, boost::is_iless());
  };

  return result;
}

VersionString::VersionString(const std::string& version)
  : m_str(version)
{
  boost::regex versionRegex("(\\d+)\\.(\\d+)(?:\\.(\\d+))?(?:\\.(\\d+))?");
  boost::smatch m;
  if (boost::regex_match(version,m,versionRegex)) {
    m_major = boost::lexical_cast<int>(std::string(m[1].first,m[1].second));
    m_minor = boost::lexical_cast<int>(std::string(m[2].first,m[2].second));
    int n = m.size();
    if (3 < n) {
      std::string temp(m[3].first,m[3].second);
      if (!temp.empty()) {
        m_patch = boost::lexical_cast<int>(temp);
      }
    }
    if (4 < n) {
      std::string temp(m[4].first,m[4].second);
      if (!temp.empty()) {
        m_build = boost::lexical_cast<int>(temp);
      }
    }
  }
  else {
    LOG_FREE_AND_THROW("openstudio.utilities.VersionString",
                       "Could not parse '" << version << "' as a version string.");
  }
}

VersionString::VersionString(int major,int minor)
  : m_major(major),
    m_minor(minor)
{
  std::stringstream ss;
  ss << major << "." << minor;
  m_str = ss.str();
}

VersionString::VersionString(int major,int minor,int patch)
  : m_major(major),
    m_minor(minor),
    m_patch(patch)
{
  std::stringstream ss;
  ss << major << "." << minor << "." << patch;
  m_str = ss.str();
}

VersionString::VersionString(int major,int minor,int patch,int build)
  : m_major(major),
    m_minor(minor),
    m_patch(patch),
    m_build(build)
{
  std::stringstream ss;
  ss << major << "." << minor << "." << patch << "." << build;
  m_str = ss.str();
}

std::string VersionString::str() const {
  return m_str;
}

int VersionString::major() const {
  return m_major;
}

int VersionString::minor() const {
  return m_minor;
}

boost::optional<int> VersionString::patch() const {
  return m_patch;
}

boost::optional<int> VersionString::build() const {
  return m_build;
}

bool VersionString::operator<(const VersionString& other) const {

  int meCurrent = major();
  int otherCurrent = other.major();

  if (meCurrent < otherCurrent) {
    return true;
  }
  else if (otherCurrent < meCurrent) {
    return false;
  }

  OS_ASSERT(meCurrent == otherCurrent);

  meCurrent = minor();
  otherCurrent = other.minor();

  if (meCurrent < otherCurrent) {
    return true;
  }
  else if (otherCurrent < meCurrent) {
    return false;
  }

  OS_ASSERT(meCurrent == otherCurrent);

  if (patch() && other.patch()) {
    meCurrent = patch().get();
    otherCurrent = other.patch().get();

    if (meCurrent < otherCurrent) {
      return true;
    }
    else if (otherCurrent < meCurrent) {
      return false;
    }

    OS_ASSERT(meCurrent == otherCurrent);

    if (build() && other.build()) {

      meCurrent = build().get();
      otherCurrent = other.build().get();

      if (meCurrent < otherCurrent) {
        return true;
      }
      else if (otherCurrent < meCurrent) {
        return false;
      }

      OS_ASSERT(meCurrent == otherCurrent);
    }
  }

  return false;
}

bool VersionString::operator>(const VersionString& other) const {
  return other < *this;
}

bool VersionString::operator==(const VersionString& other) const {
  return (!(*this < other)) && (!(other < *this));
}

bool VersionString::operator!=(const VersionString& other) const {
  return !(*this == other);
}

bool VersionString::operator<=(const VersionString& other) const {
  return (!(other < *this));
}

bool VersionString::operator>=(const VersionString& other) const {
  return (!(*this < other));
}

bool VersionString::fidelityEqual(const VersionString& other) const {
  if (patch()) {
    if (!other.patch()) {
      return false;
    }
    if (build()) {
      if (!other.build()) {
        return false;
      }
    }
    else if (other.build()) {
      return false;
    }
  }
  else if (other.patch()) {
    return false;
  }

  return true;
}

bool VersionString::isNextVersion(const VersionString& nextVersionCandidate) const {
  VersionString variantOnThis(str());
  VersionString variantOnCandidate = nextVersionCandidate;

  if (variantOnCandidate <= variantOnThis) {
    return false;
  }
 
  // now know nextVersionCandidate > this

  if (build() && fidelityEqual(nextVersionCandidate)) {
    // true if versions except for build number are equal
    variantOnThis = VersionString(major(),minor(),patch().get());
    variantOnCandidate = VersionString(nextVersionCandidate.major(),
                                       nextVersionCandidate.minor(),
                                       nextVersionCandidate.patch().get());
    if (variantOnCandidate == variantOnThis) {
      return true;
    }
  }
  else {
    // strip out build numbers as needed, because that is not the level at which the 
    // versions differ
    if (build()) {
      variantOnThis = VersionString(major(),minor(),patch().get());
    }
    if (nextVersionCandidate.build()) {
      variantOnCandidate = VersionString(nextVersionCandidate.major(),
                                         nextVersionCandidate.minor(),
                                         nextVersionCandidate.patch().get());
    }
  }

  // now have major.minor or major.minor.patch for both

  if (variantOnThis.patch()) {
    if (variantOnCandidate.patch()) {
      VersionString thisIncremented(major(),minor(),patch().get() + 1);
      if (variantOnCandidate == thisIncremented) {
        return true;
      }
      // candidate has patch, but is not next patch version. to be next version,
      // must have patch == 0
      if (variantOnCandidate.patch().get() != 0) {
        return false;
      }
    }
  }

  // now major.minor.patch v. major.minor or major.minor.patch v. major.minor.0
  // strip out patch numbers

  if (variantOnThis.patch()) {
    variantOnThis = VersionString(variantOnThis.major(),variantOnThis.minor());
  }
  if (variantOnCandidate.patch()) {
    variantOnCandidate = VersionString(variantOnCandidate.major(),variantOnCandidate.minor());
  }

  // minor increment
  VersionString thisIncremented(variantOnThis.major(),variantOnThis.minor() + 1);
  if (variantOnCandidate == thisIncremented) {
    return true;
  }

  // major increment
  thisIncremented = VersionString(variantOnThis.major() + 1,0);
  if (variantOnCandidate == thisIncremented) {
    return true;
  }

  return false;
}

std::ostream& operator<<(std::ostream& os,const VersionString& version) {
  os << version.str();
  return os;
}

bool WorkspaceObjectNameLess::operator()(const WorkspaceObject& a, const WorkspaceObject& b) const
{
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname){
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname){
    bname = *obname;
  }
  return istringLess(aname, bname);
}

bool WorkspaceObjectNameGreater::operator()(const WorkspaceObject& a, const WorkspaceObject& b) const
{
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname){
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname){
    bname = *obname;
  }
  return istringLess(bname, aname);
}

bool BCLComponentNameLess::operator()(const BCLComponent& a, const BCLComponent& b) const
{
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname){
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname){
    bname = *obname;
  }
  return istringLess(aname, bname);
}

bool BCLComponentNameGreater::operator()(const BCLComponent& a, const BCLComponent& b) const
{
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname){
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname){
    bname = *obname;
  }
  return istringLess(bname, aname);
}


} // openstudio
