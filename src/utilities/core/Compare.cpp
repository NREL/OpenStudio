/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Compare.hpp"
#include "../idf/WorkspaceObject.hpp"
#include "../bcl/BCLComponent.hpp"
#include "Assert.hpp"
#include <OpenStudio.hxx>

#include <boost/lexical_cast.hpp>

namespace openstudio {

IstringFind::IstringFind() {}

IstringFind::IstringFind(const std::string& target) {
  addTarget(target);
}
void IstringFind::addTarget(const std::string& target) {
  m_targets.push_back(target);
}

bool IstringFind::operator()(const std::string& other) const {
  for (const std::string& target : m_targets) {
    if (istringEqual(target, other)) {
      return true;
    }
  }
  return false;
}

bool IstringPairCompare::operator()(const std::pair<std::string, std::string>& x, const std::pair<std::string, std::string>& y) const {
  bool result(false);

  if (boost::iequals(x.first, y.first)) {
    result = boost::lexicographical_compare(x.second, y.second, boost::is_iless());
  } else {
    result = boost::lexicographical_compare(x.first, y.first, boost::is_iless());
  };

  return result;
}

bool VersionString::parseVersionString(const std::string& version) {
  // regex from https://semver.org/
  const boost::regex semverRegex("^(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(?:-((?:0|[1-9]\\d*|\\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\\.(?:0|[1-9]\\d*|"
                                 "\\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\\+([0-9a-zA-Z-]+(?:\\.[0-9a-zA-Z-]+)*))?$");
  const boost::regex oldRegex("(\\d+)[\\.-](\\d+)(?:[\\.-](\\d+))?(?:[\\.-](\\d+))?");
  boost::smatch m;
  if (boost::regex_match(version, m, semverRegex)) {
    m_major = boost::lexical_cast<int>(std::string(m[1].first, m[1].second));
    m_minor = boost::lexical_cast<int>(std::string(m[2].first, m[2].second));
    int n = m.size();
    if (3 < n) {
      std::string temp(m[3].first, m[3].second);
      if (!temp.empty()) {
        m_patch = boost::lexical_cast<int>(temp);
      }
    }
    if (4 < n) {
      m_patchString = std::string(m[4].first, m[4].second);
    }
    if (5 < n) {
      m_buildString = std::string(m[5].first, m[5].second);
    }
  } else if (boost::regex_match(version, m, oldRegex)) {
    m_major = boost::lexical_cast<int>(std::string(m[1].first, m[1].second));
    m_minor = boost::lexical_cast<int>(std::string(m[2].first, m[2].second));
    int n = m.size();
    if (3 < n) {
      std::string temp(m[3].first, m[3].second);
      if (!temp.empty()) {
        m_patch = boost::lexical_cast<int>(temp);
      }
      // old regex does not support patch string
    }
    if (4 < n) {
      m_buildString = std::string(m[4].first, m[4].second);
    }
  } else {
    return false;
  }

  return true;
}

VersionString::VersionString() : m_str(openStudioLongVersion()) {
  if (!parseVersionString(m_str)) {
    // This should never happen, there's a specific test in Compare_GTest.cpp for this
    OS_ASSERT(false);
  }
}

VersionString::VersionString(const std::string& version) : m_str(version) {
  if (!parseVersionString(m_str)) {
    LOG_FREE_AND_THROW("openstudio.utilities.VersionString", "Could not parse '" << version << "' as a version string.");
  }
}

VersionString::VersionString(int major, int minor) : m_major(major), m_minor(minor) {
  std::stringstream ss;
  ss << major << "." << minor;
  m_str = ss.str();
}

VersionString::VersionString(int major, int minor, int patch) : m_major(major), m_minor(minor), m_patch(patch) {
  std::stringstream ss;
  ss << major << "." << minor << "." << patch;
  m_str = ss.str();
}

VersionString::VersionString(int major, int minor, int patch, const std::string& patchString)
  : m_major(major), m_minor(minor), m_patch(patch), m_patchString(patchString) {
  std::stringstream ss;
  ss << major << "." << minor << "." << patch << "-" << patchString;
  m_str = ss.str();
}

VersionString::VersionString(int major, int minor, int patch, const std::string& patchString, const std::string& buildString)
  : m_major(major), m_minor(minor), m_patch(patch), m_patchString(patchString), m_buildString(buildString) {
  std::stringstream ss;
  ss << major << "." << minor << "." << patch << "-" << patchString << "+" << buildString;
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

std::string VersionString::patchString() const {
  return m_patchString;
}

//boost::optional<int> VersionString::build() const {
//  LOG_FREE("openstudio.utilities.VersionString", Warn, "VersionString::build is deprecated, use buildString instead.");
//  return boost::none;
//}

std::string VersionString::buildString() const {
  return m_buildString;
}

bool VersionString::operator<(const VersionString& other) const {

  int meCurrent = major();
  int otherCurrent = other.major();

  if (meCurrent < otherCurrent) {
    return true;
  } else if (otherCurrent < meCurrent) {
    return false;
  }

  OS_ASSERT(meCurrent == otherCurrent);

  meCurrent = minor();
  otherCurrent = other.minor();

  if (meCurrent < otherCurrent) {
    return true;
  } else if (otherCurrent < meCurrent) {
    return false;
  }

  OS_ASSERT(meCurrent == otherCurrent);

  if (patch() && other.patch()) {
    meCurrent = patch().get();
    otherCurrent = other.patch().get();

    if (meCurrent < otherCurrent) {
      return true;
    } else if (otherCurrent < meCurrent) {
      return false;
    }

    OS_ASSERT(meCurrent == otherCurrent);
  }

  // When major, minor, and patch are equal, a pre-release version has lower precedence than a normal version
  std::string otherPatchString = other.patchString();
  if (m_patchString.empty() && otherPatchString.empty()) {
    return false;
  } else if (m_patchString.empty() && !otherPatchString.empty()) {
    return false;
  } else if (!m_patchString.empty() && otherPatchString.empty()) {
    return true;
  } else if (m_patchString < otherPatchString) {
    return true;
  } else if (m_patchString > otherPatchString) {
    return false;
  }

  OS_ASSERT(m_patchString == otherPatchString);

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

//bool VersionString::fidelityEqual(const VersionString& other) const
//{
//  LOG_FREE("openstudio.utilities.VersionString", Warn, "VersionString::fidelityEqual is deprecated.");
//  return true;
//}

bool VersionString::isNextVersion(const VersionString& nextVersionCandidate) const {
  VersionString variantOnThis(str());
  VersionString variantOnCandidate = nextVersionCandidate;

  if (variantOnCandidate <= variantOnThis) {
    return false;
  }

  // now know nextVersionCandidate > this

  // now have major.minor or major.minor.patch for both

  if (variantOnThis.patch()) {
    if (variantOnCandidate.patch()) {
      VersionString thisIncremented(major(), minor(), patch().get() + 1);
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
  // cppcheck-suppress duplicateCondition
  if (variantOnThis.patch()) {
    variantOnThis = VersionString(variantOnThis.major(), variantOnThis.minor());
  }
  if (variantOnCandidate.patch()) {
    variantOnCandidate = VersionString(variantOnCandidate.major(), variantOnCandidate.minor());
  }

  // minor increment
  VersionString thisIncremented(variantOnThis.major(), variantOnThis.minor() + 1);
  if (variantOnCandidate == thisIncremented) {
    return true;
  }

  // major increment
  thisIncremented = VersionString(variantOnThis.major() + 1, 0);
  if (variantOnCandidate == thisIncremented) {
    return true;
  }

  return false;
}

std::ostream& operator<<(std::ostream& os, const VersionString& version) {
  os << version.str();
  return os;
}

bool WorkspaceObjectNameLess::operator()(const WorkspaceObject& a, const WorkspaceObject& b) const {
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname) {
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname) {
    bname = *obname;
  }
  return istringLess(aname, bname);
}

bool WorkspaceObjectNameGreater::operator()(const WorkspaceObject& a, const WorkspaceObject& b) const {
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname) {
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname) {
    bname = *obname;
  }
  return istringLess(bname, aname);
}

bool BCLComponentNameLess::operator()(const BCLComponent& a, const BCLComponent& b) const {
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname) {
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname) {
    bname = *obname;
  }
  return istringLess(aname, bname);
}

bool BCLComponentNameGreater::operator()(const BCLComponent& a, const BCLComponent& b) const {
  std::string aname;
  boost::optional<std::string> oaname = a.name();
  if (oaname) {
    aname = *oaname;
  }
  std::string bname;
  boost::optional<std::string> obname = b.name();
  if (obname) {
    bname = *obname;
  }
  return istringLess(bname, aname);
}

}  // namespace openstudio
