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

