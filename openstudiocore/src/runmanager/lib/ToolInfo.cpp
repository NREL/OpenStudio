/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "ToolInfo.hpp"
#include <boost/thread.hpp>

namespace openstudio {
namespace runmanager {

  ToolVersion::ToolVersion()
  {
  }

  ToolVersion::ToolVersion(int t_major)
    : m_major(t_major)
  {
  }

  ToolVersion::ToolVersion(int t_major, int t_minor)
    : m_major(t_major), m_minor(t_minor)
  {
  }

  ToolVersion::ToolVersion(int t_major, int t_minor, int t_build)
    : m_major(t_major), m_minor(t_minor), m_build(t_build)
  {
  }

  ToolVersion::ToolVersion(int t_major, int t_minor, int t_build, std::string t_tag)
    : m_major(t_major), m_minor(t_minor), m_build(t_build), m_tag(std::move(t_tag))
  {
  }

  ToolVersion::ToolVersion(boost::optional<int> t_major, boost::optional<int> t_minor, boost::optional<int> t_build,
      boost::optional<std::string> t_tag)
    : m_major(std::move(t_major)), m_minor(std::move(t_minor)), m_build(std::move(t_build)), m_tag(std::move(t_tag))
  {
  }

  bool ToolVersion::empty() const
  {
    return !m_major && !m_minor && !m_build;
  }

  /// Operator overload for streaming ToolVersion to ostream
  RUNMANAGER_API std::ostream &operator<<(std::ostream &os, const ToolVersion &tv)
  {
    return (os << tv.toString());
  }

  ToolVersion ToolVersion::fromString(const std::string &t_val) 
  {
    boost::optional<int> major;
    boost::optional<int> minor;
    boost::optional<int> build;
    boost::optional<std::string> tag;

    std::vector<std::string> parts;
    boost::split(parts, t_val, boost::is_any_of("."), boost::algorithm::token_compress_on);

    if (parts.size() > 0 && parts[0] != "-")
    {
      major = atoi(parts[0].c_str());
    }

    if (parts.size() > 1 && parts[1] != "-")
    {
      minor = atoi(parts[1].c_str());
    }

    if (parts.size() > 2 && parts[2] != "-")
    {
      build = atoi(parts[2].c_str());
    }

    if (parts.size() > 3 && parts[3] != "-")
    {
      tag = parts[3];
    }

    return ToolVersion(major, minor, build, tag);
  }

  std::string ToolVersion::toString() const
  {
    std::stringstream ss;

    if (m_major)
    {
      ss << *m_major;
    } else {
      ss << "-";
    }

    ss << ".";

    if (m_minor)
    {
      ss << *m_minor;
    } else {
      ss << "-";
    }

    ss << ".";

    if (m_build)
    {
      ss << *m_build;
    } else {
      ss << "-";
    }

    ss << ".";

    if (m_tag)
    {
      ss << *m_tag;
    } else {
      ss << "-";
    }

    return ss.str();
  }

  bool ToolVersion::operator==(const ToolVersion &rhs) const
  {
    return m_major == rhs.m_major
      && m_minor == rhs.m_minor
      && m_build == rhs.m_build
      && m_tag == rhs.m_tag;
  }

  bool ToolVersion::operator<(const ToolVersion &rhs) const
  {
    return m_major < rhs.m_major
      || (m_major == rhs.m_major && m_minor < rhs.m_minor)
      || (m_major == rhs.m_major && m_minor == rhs.m_minor && m_build < rhs.m_build)
      || (m_major == rhs.m_major && m_minor == rhs.m_minor && m_build == rhs.m_build && m_tag < rhs.m_tag);
  }

  bool ToolVersion::matches(const ToolVersion &requirements) const
  {
    // Updated: if the version number of the Tools is not known - call it a match
    // The most specific version that matches any *set* value will be found first
    //
    // Tag, if specified, must be an exact match
    return (m_major == requirements.m_major || !requirements.m_major || !m_major)
        && (m_minor == requirements.m_minor || !requirements.m_minor || !m_minor)
        && (m_build == requirements.m_build || !requirements.m_build || !m_build)
        && (m_tag  == requirements.m_tag  || !requirements.m_tag);
  }

  boost::optional<int> ToolVersion::getMajor() const
  {
    return m_major;
  }

  boost::optional<int> ToolVersion::getMinor() const
  {
    return m_minor;
  }

  boost::optional<int> ToolVersion::getBuild() const
  {
    return m_build;
  }

  boost::optional<std::string> ToolVersion::getTag() const
  {
    return m_tag;
  }


  ToolInfo Tools::getLastByName(const std::string &t_name) const
  {
    return getLast(std::bind(&Tools::nameCompare, t_name, std::placeholders::_1));
  }

  Tools Tools::getAllByName(const std::string &t_name) const
  {
    return getAll(std::bind(&Tools::nameCompare, t_name, std::placeholders::_1));
  }

  ToolInfo Tools::getLastByVersion(const ToolVersion &t_version) const
  {
    return getLast(std::bind(&Tools::versionCompare, t_version, std::placeholders::_1));
  }

  Tools Tools::getAllByVersion(const ToolVersion &t_version) const
  {
    return getAll(std::bind(&Tools::versionCompare, t_version, std::placeholders::_1));
  }

}
}

