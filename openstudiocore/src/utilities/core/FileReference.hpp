/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/core/Enum.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>

#include <utilities/time/DateTime.hpp>

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
    ((XML)(xml)) );

class UTILITIES_API FileReference {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FileReference(const openstudio::path& p);

  // ETH@20110712 Seems potentially really bad to have this exposed. Could move this class to
  // project, then use friendship to hide it.
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

  DateTime timestampCreate() const;

  DateTime timestampLast() const;

  std::string checksumCreate() const;

  std::string checksumLast() const;

  //@}
  /** @name Setters */
  //@{

  void setName(const std::string& newName);

  void setDisplayName(const std::string& newDisplayName);

  void setDescription(const std::string& newDescription);

  bool makePathAbsolute(const openstudio::path& searchDirectory);

  /** Save the path as relative to basePath, or just keep the file name and extension if 
   *  basePath.empty(). */
  bool makePathRelative(const openstudio::path& basePath=openstudio::path());

  // ETH@20110712 Behavior of timestampLast? Pulled from filesystem? Updated only if checksum
  // is different?
  /** Updates checksumLast. If file is located, returns true. */ 
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
};

/** \relates FileReference*/
typedef boost::optional<FileReference> OptionalFileReference;

/** \relates FileReference*/
typedef std::vector<FileReference> FileReferenceVector;

namespace detail {

  /** Places attribute's data in a QVariant for JSON serialization. */
  QVariant toVariant(const FileReference& fileReference);

  /** Deserializes json variant to FileReference. */
  FileReference toFileReference(const QVariant& variant, const VersionString& version);

}

} // openstudio

#endif // UTILITIES_CORE_FILEREFERENCE_HPP

