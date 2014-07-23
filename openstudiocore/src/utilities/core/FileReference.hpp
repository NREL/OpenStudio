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

#ifndef UTILITIES_CORE_FILEREFERENCE_HPP
#define UTILITIES_CORE_FILEREFERENCE_HPP

#include "../UtilitiesAPI.hpp"

#include "Enum.hpp"
#include "Path.hpp"
#include "UUID.hpp"

#include "../time/DateTime.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

/** \class FileReferenceType
 *  \brief Type of files we expect to handle with FileReference. 
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro 
 *  call is: 
 *  \code
OPENSTUDIO_ENUM( FileReferenceType,
    ((Unknown)(unknown))
    ((CSV)(csv))
    ((EPW)(epw))
    ((IDF)(idf))
    ((OSC)(osc))
    ((OSM)(osm))
    ((RB)(rb))
    ((SQL)(sql))
    ((XML)(xml)) );
 *  \endcode */
OPENSTUDIO_ENUM( FileReferenceType,
    ((Unknown)(unknown))
    ((CSV)(csv))
    ((EPW)(epw))
    ((IDF)(idf))
    ((OSC)(osc))
    ((OSM)(osm))
    ((RB)(rb))
    ((SQL)(sql))
    ((XML)(xml))
    ((OSSR)(ossr)) );

class UTILITIES_API FileReference {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FileReference(const openstudio::path& p);

  /** De-serialization constructor. Not for general use. */
  FileReference(const openstudio::UUID& uuid,
                const openstudio::UUID& versionUUID,
                const std::string& name,
                const std::string& displayName,
                const std::string& description,
                const openstudio::path& p,
                const FileReferenceType& fileType,
                const DateTime& timestampCreate,
                const DateTime& timestampLast,
                const std::string& checksumCreate,
                const std::string& checksumLast);

  virtual ~FileReference() {}

  FileReference clone() const;

  //@}
  /** @name Getters */
  //@{

  openstudio::UUID uuid() const;

  openstudio::UUID versionUUID() const;

  std::string name() const;

  std::string displayName() const;

  std::string description() const;

  openstudio::path path() const;

  FileReferenceType fileType() const;

  /** Time this object was created or time file was created, depending on
   *  whether path exists at time of construction. */
  DateTime timestampCreate() const;

  /** Last modified time of this file, or time object was created if path
   *  does not actually exist on the file system. */
  DateTime timestampLast() const;

  /** Checksum at time this object was created, if file exists. Otherwise
   *  "00000000". */
  std::string checksumCreate() const;

  std::string checksumLast() const;

  //@}
  /** @name Setters */
  //@{

  void setName(const std::string& newName);

  void setDisplayName(const std::string& newDisplayName);

  void setDescription(const std::string& newDescription);

  void setPath(const openstudio::path& newPath);

  bool makePathAbsolute(const openstudio::path& searchDirectory);

  /** Save the path as relative to basePath, or just keep the file name and extension if 
   *  basePath.empty(). */
  bool makePathRelative(const openstudio::path& basePath=openstudio::path());

  /** Returns true and updates timestampLast and checksumLast if file is located. */
  bool update(const openstudio::path& searchDirectory);

  //@}
 private:
  openstudio::UUID m_uuid;
  openstudio::UUID m_versionUUID;
  std::string m_name;
  std::string m_displayName;
  std::string m_description;
  openstudio::path m_path;
  FileReferenceType m_fileType;
  DateTime m_timestampCreate;
  DateTime m_timestampLast;
  std::string m_checksumCreate;
  std::string m_checksumLast;

  REGISTER_LOGGER("openstudio.utilities.FileReference");

  /** Sets timestamps and checksums. If lastOnly, does not try to
   *  determine timestampCreate. */
  bool update(const openstudio::path& searchDirectory,bool lastOnly);
};

/** \relates FileReference*/
typedef boost::optional<FileReference> OptionalFileReference;

/** \relates FileReference*/
typedef std::vector<FileReference> FileReferenceVector;

namespace detail {

  /** Places attribute's data in a QVariant for JSON serialization. */
  UTILITIES_API QVariant toVariant(const FileReference& fileReference);

  /** Deserializes json variant to FileReference. */
  UTILITIES_API FileReference toFileReference(const QVariant& variant, const VersionString& version);

}

} // openstudio

#endif // UTILITIES_CORE_FILEREFERENCE_HPP

