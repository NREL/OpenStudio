/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_BCL_BCLFILEREFERENCE_HPP
#define UTILITIES_BCL_BCLFILEREFERENCE_HPP

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../core/Compare.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

namespace pugi {
class xml_node;
}

namespace openstudio {

/** BCLFileReference is a class for tracking files that come with BCL components and measures.
  **/
class UTILITIES_API BCLFileReference
{

 public:
  /** @name Constructors */
  //@{

  /// Constructor from file path.
  explicit BCLFileReference(const openstudio::path& path, const bool setMembers = false);

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~BCLFileReference();

  //@}
  /** @name Getters */
  //@{

  /// Returns path to file.
  openstudio::path path() const;

  /// Returns the last recorded checksum.
  std::string checksum() const;

  std::string softwareProgram() const;

  std::string softwareProgramVersion() const;

  boost::optional<VersionString> minCompatibleVersion() const;

  boost::optional<VersionString> maxCompatibleVersion() const;

  std::string fileName() const;

  std::string fileType() const;

  std::string usageType() const;

  //@}
  /** @name Setters */
  //@{

  void setChecksum(const std::string& checksum);

  void setSoftwareProgram(const std::string& softwareProgram);

  void setSoftwareProgramVersion(const std::string& softwareProgramVersion);

  void setMinCompatibleVersion(const VersionString& minCompatibleVersion);

  void resetMinCompatibleVersion();

  void setMaxCompatibleVersion(const VersionString& maxCompatibleVersion);

  void resetMaxCompatibleVersion();

  void setUsageType(const std::string& usageType);

  //@}
  /** @name Operators */
  //@{

  void writeValues(pugi::xml_node& element) const;

  /// Check if the file has been updated and return if so.  Will update checksum.
  bool checkForUpdate();

  //@}

 private:
  // configure logging
  REGISTER_LOGGER("utilities.bcl.BCLFileReference");

  openstudio::path m_path;
  std::string m_checksum;
  std::string m_softwareProgram;
  std::string m_softwareProgramVersion;
  boost::optional<VersionString> m_minCompatibleVersion;
  boost::optional<VersionString> m_maxCompatibleVersion;
  std::string m_fileName;
  std::string m_fileType;
  std::string m_usageType;
};

/** Prints BCLFileReference to os. \relates BCLFileReference */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const BCLFileReference& file);

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLFILEREFERENCE_HPP
