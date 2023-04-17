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

// clang-format off

/** \class FileReferenceType
 *  \brief Type of files we expect to handle with FileReference.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(FileReferenceType,
  ((Unknown)(unknown))
  ((CSV)(csv))
  ((EPW)(epw))
  ((IDF)(idf))
  ((OSC)(osc))
  ((OSM)(osm))
  ((RB)(rb))
  ((SQL)(sql))
  ((XML)(xml))
  ((OSSR)(ossr))
);
 *  \endcode */
OPENSTUDIO_ENUM(FileReferenceType,
  ((Unknown)(unknown))
  ((CSV)(csv))
  ((EPW)(epw))
  ((IDF)(idf))
  ((OSC)(osc))
  ((OSM)(osm))
  ((RB)(rb))
  ((SQL)(sql))
  ((XML)(xml))
  ((OSSR)(ossr))
);

// clang-format on

class UTILITIES_API FileReference
{
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FileReference(const openstudio::path& p);

  /** De-serialization constructor. Not for general use. */
  FileReference(const openstudio::UUID& uuid, const openstudio::UUID& versionUUID, const std::string& name, const std::string& displayName,
                const std::string& description, const openstudio::path& p, const FileReferenceType& fileType, const DateTime& timestampLast,
                const std::string& checksumCreate, const std::string& checksumLast);

  virtual ~FileReference() = default;

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
  bool makePathRelative(const openstudio::path& basePath = openstudio::path());

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
  DateTime m_timestampLast;
  std::string m_checksumCreate;
  std::string m_checksumLast;

  REGISTER_LOGGER("openstudio.utilities.FileReference");
};

/** \relates FileReference*/
using OptionalFileReference = boost::optional<FileReference>;

/** \relates FileReference*/
using FileReferenceVector = std::vector<FileReference>;

}  // namespace openstudio

#endif  // UTILITIES_CORE_FILEREFERENCE_HPP
