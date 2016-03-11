/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_BCL_BCLFILEREFERENCE_HPP
#define UTILITIES_BCL_BCLFILEREFERENCE_HPP

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../core/Compare.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

class QDomDocument;
class QDomElement;

namespace openstudio{

  /** BCLFileReference is a class for tracking files that come with BCL components and measures.
  **/
  class UTILITIES_API BCLFileReference {

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

    void writeValues(QDomDocument& doc, QDomElement& element) const;

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

} // openstudio

#endif // UTILITIES_BCL_BCLFILEREFERENCE_HPP
