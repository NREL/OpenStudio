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

#ifndef PROJECT_FILEREFERENCERECORD_HPP
#define PROJECT_FILEREFERENCERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {

class FileReference;
class FileReferenceType;

namespace project {

class AttributeRecord;
class TagRecord;

namespace detail {
  class FileReferenceRecord_Impl;
} // detail

/** \class FileReferenceRecordColumns
 *  \brief Column definitions for the FileReferenceRecords table.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(FileReferenceRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((parentDatabaseTableName)(TEXT)(8))
  ((parentRecordId)(INTEGER)(9))
  ((fileReferenceType)(INTEGER)(10))
  ((path)(TEXT)(11))
  ((checksumCreate)(TEXT)(12))
  ((checksumLast)(TEXT)(13))
);
 *  \endcode */
OPENSTUDIO_ENUM(FileReferenceRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((parentDatabaseTableName)(TEXT)(8))
  ((parentRecordId)(INTEGER)(9))
  ((fileReferenceType)(INTEGER)(10))
  ((path)(TEXT)(11))
  ((checksumCreate)(TEXT)(12))
  ((checksumLast)(TEXT)(13))
);

/** FileReferenceRecord is an ObjectRecord. It may be a top-level object, or it may be 
 *  parented by an AnalysisRecord, a DataPointRecord, or a DiscretePerturbationRecord. It may
 *  have child TagRecords or AttributeRecords. */
class PROJECT_API FileReferenceRecord : public ObjectRecord {
 public:

  typedef detail::FileReferenceRecord_Impl ImplType;
  typedef FileReferenceRecordColumns ColumnsType;
  typedef FileReferenceRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  /** Orphan FileReferenceRecord. Use with caution. */
  FileReferenceRecord(const FileReference& fileReference,
                      ProjectDatabase& database);

  FileReferenceRecord(const FileReference& fileReference,
                      ObjectRecord& parentRecord);

  /// constructor from query, throws if bad query
  FileReferenceRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~FileReferenceRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  static boost::optional<FileReferenceRecord> factoryFromQuery(const QSqlQuery& query, 
                                                               ProjectDatabase& database);

  static std::vector<FileReferenceRecord> getFileReferenceRecords(ProjectDatabase& database);

  static boost::optional<FileReferenceRecord> getFileReferenceRecord(
      int id, ProjectDatabase& database);

  static boost::optional<FileReferenceRecord> getFileReferenceRecord(
      const UUID& handle, ProjectDatabase& database);

  static boost::optional<FileReferenceRecord> getFileReferenceRecord(
      const openstudio::path& path, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Deserialize to FileReference. */
  FileReference fileReference() const;

  /** Get the file type. */
  FileReferenceType fileType() const;
  
  /// get the path
  openstudio::path path() const;         

  /// get the checksum at model creation
  std::string checksumCreate() const;     

  /// get the checksum at last edit
  std::string checksumLast() const;

  std::vector<AttributeRecord> attributeRecords() const;

  boost::optional<AttributeRecord> getAttributeRecord(const std::string& name) const;

  std::vector<TagRecord> tagRecords() const;

  boost::optional<TagRecord> getTagRecord(const std::string& name) const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class detail::Record_Impl;
  friend class ProjectDatabase;
  friend class detail::FileReferenceRecord_Impl;

  // from impl
  FileReferenceRecord(std::shared_ptr<detail::FileReferenceRecord_Impl> impl, 
                      ProjectDatabase database);

  explicit FileReferenceRecord(std::shared_ptr<detail::FileReferenceRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.FileReferenceRecord");
};

/** \relates FileReferenceRecord*/
typedef boost::optional<FileReferenceRecord> OptionalFileReferenceRecord;

/** \relates FileReferenceRecord*/
typedef std::vector<FileReferenceRecord> FileReferenceRecordVector;

} // project
} // openstudio

#endif // PROJECT_FILEREFERENCERECORD_HPP

